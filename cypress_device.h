#ifndef CYPRESS_DEVICE_H
#define CYPRESS_DEVICE_H
#include "data_frame.h"
#include <QString>
#include <atomic>
#include <libusb-1.0/libusb.h>
#include <memory>
#include <vector>
class cypress_device {
public:
  libusb_device_handle *device_handle = nullptr;
  libusb_device *device;
  libusb_device_descriptor device_descriptor;
  uint16_t device_pid;
  uint16_t manufacture_pid;
  QString device_type;
  int device_number;
  int bus_number;
  int thread_id = 0;
  std::vector<std::string> output_string;
  std::vector<std::tuple<unsigned int, unsigned int, float, unsigned int>> stat;
  unsigned char input_endpoint;
  unsigned char output_endpoint;
  bool print_recived_buffer=false;
  cypress_device(libusb_device *dev);
  ~cypress_device();
  void open_device();
  void close_device();
  void set();
  void start();
  void stop();
  void bulk_send(unsigned char *send);
  std::vector<unsigned char> bulk_recive(int num_frame);
  std::vector<data_frame> get_data_frames(int num_frame);
};

#endif // DEVICE_H
