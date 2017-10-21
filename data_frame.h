#ifndef DATA_FRAME_H
#define DATA_FRAME_H
#include "info_frame.h"
#include <bitset>
#include <memory>
#include <vector>
class data_frame {
  std::vector<unsigned char> raw_data;

public:
  std::vector<std::shared_ptr<info_frame>> info_frames;
  data_frame(std::vector<unsigned char> input);
  union {
    unsigned int number;
    unsigned char byte[4] = {0x00};
  } number;
  union {
    unsigned short number;
    unsigned char byte[4] = {0x00};
  } end;
  bool validate(std::vector<unsigned char> input);
  void print_infoframes();
  void raw_print();
};

#endif // DATA_FRAME_H
