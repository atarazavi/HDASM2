#include "data_frame.h"
#include <iostream>
#include <vector>

std::vector<std::vector<unsigned char>>
m_split_buffer(std::vector<unsigned char> input, int cut_size) {
  std::vector<std::vector<unsigned char>> output;
  // std::cout<<"input.size /cut_size "<<input.size()/cut_size<<"\n";
  for (unsigned int buffer = 0; buffer < (input.size() / cut_size); buffer++) {
    std::vector<unsigned char> temp_buff;
    for (int pos = 0; pos < cut_size; pos++) {
      temp_buff.push_back(input[buffer * cut_size + pos]);
    }
    // std::cout<<"temp_buff size"<<temp_buff.size()<<"\n";
    output.push_back(temp_buff);
  }
  return output;
}

data_frame::data_frame(std::vector<unsigned char> input) {
//  if (this->validate(input)) {
    if (input.size()==512) {
    this->raw_data = input;
    this->number.byte[0] = input[2];
    this->number.byte[1] = input[3];
    this->number.byte[2] = input[0];
    this->number.byte[3] = input[1];
    this->end.byte[0] = input[511];
    this->end.byte[1] = input[510];
    // std::cout<<"end is" <<this->end.number<<"\n";
    this->raw_data = input;
    input.erase(input.begin(), input.begin() + 3);
    input.erase(input.end() - 3, input.end());
    auto infos = m_split_buffer(input, 12);
    for (auto info : infos) {
      auto temp = std::make_shared<info_frame>(info);
      this->info_frames.push_back(temp);
    }
    // std::cout<<"now input size is "<<input.size()<<"\n";
  }else{
      std::cout<<"error in data_frame\n";
      exit(0);
   }
}

bool data_frame::validate(std::vector<unsigned char> input) {
  if (input.size() == 512 && input[511] == 0xff && input[510] == 0xff) {
    return true;
  } else {
    return false;
  }
}

void data_frame::print_infoframes() {
  for (auto info : this->info_frames) {
    info->print();
  }
}

void data_frame::raw_print() {
  std::cout << "\n raw data_frame: \n";
  for (int i = 0; i < 512; i++) {
    std::cout << std::bitset<8>(this->raw_data[i]) << " ";
  }
  std::cout << "\n";
}
