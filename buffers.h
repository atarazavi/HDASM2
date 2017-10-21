#ifndef CHANNEL_BUFFER_H
#define CHANNEL_BUFFER_H
#include "data_frame.h"
#include "info_frame.h"
//#include "cypress_device.h"
#include <memory>
#include <set>
#include <vector>
#include <QDataStream>
class buffers {
public:
  static const int num_bin=4096;
  std::set<info_frame> buffer[8];
  unsigned long long passed_time_offset[8]={{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};
  bool offset_is_set[8]={{false},{false},{false},{false},{false},{false},{false},{false}};
  int histograms[8][num_bin] = {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};
  unsigned long long real_time[8] = {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};
  unsigned long long dead_time[8] = {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};
  unsigned long long sample_count[8] = {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};
  unsigned long long passed_time[8] = {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};
  buffers();
  void insert_info_frames(info_frame info);
  void insert_info_frames(std::vector<std::shared_ptr<info_frame>> info);
  void insert_info_frames(std::vector<data_frame> data);
  void insert_info_frames(data_frame data);
  void print_histograms();
  void print_buffers_info_frames(int channel);
  void print_buffers_info_frames();
  void clear_buffer();
  int ** buffer_2D(int channel_1, int channel_2,unsigned long long dustance);
};
QDataStream &operator<<(QDataStream &out, const buffers &data_buffer);
QDataStream &operator>>(QDataStream &in,  std::set<info_frame> &set);
QDataStream &operator<<(QDataStream &out, const buffers &data_buffer);
QDataStream &operator>>(QDataStream &in, buffers &data_buffer);
#endif // CHANNEL_BUFFER_H
