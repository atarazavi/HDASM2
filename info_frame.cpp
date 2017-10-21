#include "info_frame.h"
#include <bitset>
#include <iostream>

QDataStream &operator<<(QDataStream &out, const info_frame &info){
    out<<info.Realtime.time<<info.deadtime.time<<info.channel.ch_no<<info.data.data;
    return out;
}
/*QDataStream &operator>>(QDataStream &in, unsigned long long int &input){
    union{
        unsigned long long int data;
        unsigned char byte [8]={0x00};
    }data;
    for(int i=0;i<8;i++){
        unsigned char temp;
        in>>temp;
        data.byte[i]=temp;
    }
    input=data.data;
    return in;
}
*/
QDataStream &operator>>(QDataStream &in, info_frame &info){
    in>>info.Realtime.time>>info.deadtime.time>>info.channel.ch_no>>info.data.data;
    return in;
}

info_frame::info_frame(){

}



info_frame::info_frame(std::vector<unsigned char> input) {
  if (input.size() != 12) {
    std::cout << "info frame input size is not 12 instead it is "
              << input.size() << "\n";
    exit(1);
  }
  for (int i = 0; i < 12; i++)
    this->raw_data[i] = input[i];

  this->channel.byte[0] = input[1];
  this->channel.byte[1] = input[0];
  this->channel_init();

  this->data.byte[0] = input[3];
  this->data.byte[1] = input[2];

  this->Realtime.byte[5] = input[8];
  this->Realtime.byte[4] = input[9];
  this->Realtime.byte[3] = input[6];
  this->Realtime.byte[2] = input[7];
  this->Realtime.byte[1] = input[4];
  this->Realtime.byte[0] = input[5];

  this->deadtime.byte[0] = input[10];
  this->deadtime.byte[1] = input[11];
}

void info_frame::print() {
  std::cout << "chNo " << (int)this->channel_number << " Rtime "
            //<<std::bitset<8>(this->Realtime.byte[0])<<" "
            //<<std::bitset<8>(this->Realtime.byte[1])<<" "
            //<<std::bitset<8>(this->Realtime.byte[2])<<" "
            //<<std::bitset<8>(this->Realtime.byte[3])<<" "
            //<<std::bitset<8>(this->Realtime.byte[4])<<" "
            //<<std::bitset<8>(this->Realtime.byte[5])<<" "
            //<<std::bitset<8>(this->Realtime.byte[6])<<" "
            //<<std::bitset<8>(this->Realtime.byte[7])<<" ="
            << this->Realtime.time << " Data " << this->data.data
            << " Deadtime " << this->deadtime.time << "\n";

  this->print_raw();
}

void info_frame::channel_init() {
  if (this->channel.byte[1] == 0) {
    // std::cout<<"correct channel\n";
    this->channel_number = (int)this->channel.byte[0];
  }
}

void info_frame::print_raw() {
  std::cout << "info_frame raw_data:\n";
  for (int i = 0; i < 12; i++)
    std::cout << std::bitset<8>(this->raw_data[i]) << " ";
  std::cout << "\n\n\n";
}
