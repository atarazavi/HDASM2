#ifndef INFO_FRAME_H
#define INFO_FRAME_H
#include <vector>
#include <QDataStream>
class info_frame {
public:
  unsigned char raw_data[12] = {0x00};
  union {
    unsigned long long time;
    unsigned char byte[8] = {0x00};
  } Realtime;
  union {
    unsigned short time;
    unsigned char byte[2] = {0x00};
  } deadtime;
  union {
    short ch_no;
    unsigned char byte[2] = {0x00};
  } channel;
  int channel_number = 0;
  union {
    unsigned short data;
    unsigned char byte[2] = {0x00};
  } data;
  info_frame(std::vector<unsigned char> input);
  info_frame();
  void print();
  void channel_init();
  void print_raw();
};
QDataStream &operator<<(QDataStream &out, const info_frame &info);
QDataStream &operator>>(QDataStream &in, info_frame &info);
#endif // INFO_FRAME_H
