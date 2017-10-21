#include "buffers.h"
#include <iostream>
#include <iterator>

QDataStream &operator<<(QDataStream &out, const std::set<info_frame> &set){
    out<<static_cast<int>(set.size());
    for(auto i:set){
        out<<i;
    }
    return out;
}

QDataStream &operator>>(QDataStream &in,  std::set<info_frame> &set){
    int set_size;
    in>>set_size;
    for(int i=0;i<set_size;i++){
        info_frame temp;
        in>>temp;
        set.insert(temp);
    }
    return in;
}

QDataStream &operator<<(QDataStream &out, const buffers &data_buffer){
    for(int i=0;i<8;i++){
        out<<data_buffer.buffer[i];
    }
    for(int i=0;i<8;i++){
        out<<data_buffer.passed_time_offset[i];
        out<<data_buffer.offset_is_set[i];
        out<<data_buffer.real_time[i];
        out<<data_buffer.dead_time[i];
        out<<data_buffer.sample_count[i];
        out<<data_buffer.passed_time[i];
    }
    out<<data_buffer.num_bin;
    for(int i=0;i<8;i++){
        for(int j=0;j<data_buffer.num_bin;j++){
            out<<data_buffer.histograms[i][j];
        }
    }

}

QDataStream &operator>>(QDataStream &in,buffers &data_buffer){
    for(int i=0;i<8;i++){
        in>>data_buffer.buffer[i];
    }
    for(int i=0;i<8;i++){
        in>>data_buffer.passed_time_offset[i];
        in>>data_buffer.offset_is_set[i];
        in>>data_buffer.real_time[i];
        in>>data_buffer.dead_time[i];
        in>>data_buffer.sample_count[i];
        in>>data_buffer.passed_time[i];
    }
    //in>>data_buffer.num_bin;
    for(int i=0;i<8;i++){
        for(int j=0;j<data_buffer.num_bin;j++){
            in>>data_buffer.histograms[i][j];
        }
    }

}






bool operator<(const info_frame &left, const info_frame &right) {
  return left.Realtime.time < right.Realtime.time;
}
buffers::buffers() {}

void buffers::insert_info_frames(info_frame info) {
  //std::cout<<"in inset_info_frames(info frame)\n";
  int ch = info.channel_number - 1;
  if (ch >= 0 && ch < 8) {
    if (!this->offset_is_set[ch]) {
      this->passed_time_offset[ch] = info.Realtime.time;
      this->offset_is_set[ch] = true;
    }
    //std::cout<<"before offset"<< info.Realtime.time<<"\n";
    //info.Realtime.time -= this->passed_time_offset[ch];
    //std::cout<<"after offset"<< info.Realtime.time<<"\n";
    this->buffer[ch].insert(info);
    auto bin = info.data.data;
    this->histograms[ch][bin]++;
    this->dead_time[ch] += info.deadtime.time;
    this->sample_count[ch]++;
    //info.print();
    if (info.Realtime.time > this->passed_time[ch]) {
      this->passed_time[ch] = info.Realtime.time;
      this->real_time[ch] = this->passed_time[ch] - this->dead_time[ch] -
                            this->passed_time_offset[ch];
    }
  }
}
void buffers::insert_info_frames(
    std::vector<std::shared_ptr<info_frame>> info) {
  //std::cout<<"in inset_info_frames(vector info_frames)\n";
  for (const auto i : info) {
    insert_info_frames(*(i));
  }
}

void buffers::insert_info_frames(data_frame data) {
  //std::cout<<"in inset_info_frames(data_frame)\n";
  insert_info_frames(data.info_frames);
}

void buffers::insert_info_frames(std::vector<data_frame> data) {
  //std::cout<<"in inset_info_frames(vector data_frame)\n";
  for (const auto i : data) {
    insert_info_frames(i);
  }
}

void buffers::print_histograms() {
  std::cout << "printing histograms \n";
  for (int i = 0; i < 8; i++) {
    std::cout << "histogram of channel " << i << ": \n";
    for (int j = 0; j < this->num_bin; j++) {
      std::cout << this->histograms[i][j] << " ";
    }
    std::cout << "\n";
  }
}

void buffers::print_buffers_info_frames(int channel) {
  if (channel < 8 && channel > 0) {
    std::cout << "printing frame " << channel << "\n";
    for (auto frame : this->buffer[channel]) {
      frame.print();
    }
  }
}

void buffers::print_buffers_info_frames() {
  for (int i = 0; i < 8; i++) {
    print_buffers_info_frames(i);
  }
}

int** buffers::buffer_2D(int channel_1, int channel_2, unsigned long long distance){
    int ** histograms_2D;
    histograms_2D=new int*[this->num_bin];
    for(int i=0;i<this->num_bin;i++){
        histograms_2D[i]=new int[this->num_bin]{0};
    }
    for(auto point:this->buffer[channel_1]){
        info_frame low_num=point;
        low_num.Realtime.time-=distance;
        info_frame high_num=point;
        high_num.Realtime.time+=distance;
        auto it_low=this->buffer[channel_2].lower_bound(low_num);
        auto it_high=this->buffer[channel_2].upper_bound(high_num);
        if(std::distance(it_low,it_high)>=1){
            int ch_1_value=point.data.data;
            it_low++;
            int ch_2_value=it_low->data.data;
            histograms_2D[ch_1_value][ch_2_value]++;
            histograms_2D[ch_2_value][ch_1_value]++;
        }

    }
    return histograms_2D;
}

void buffers::clear_buffer(){
    for(int i=0;i<8;i++){
            this->buffer[i].clear();
            this->passed_time_offset[i]=0;
            this->offset_is_set[i]={false};
            for(int j=0;j<this->num_bin;j++)
                this->histograms[i][j]=0;
            this->real_time[i]=0;
            this->dead_time[i]=0;
            this->sample_count[i]=0;
            this->passed_time[i]=0;
    }
//    std::cout<<"completed!\n"<<std::endl;
}
