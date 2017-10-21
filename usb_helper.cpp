#include "cypress_device.h"
#include "data_frame.h"
#include <QString>
#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <libusb-1.0/libusb.h>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include <set>
#include "buffers.h"
#include<future>
//#include<QByteArray>
// libusb_device **devs;
std::vector<std::string> string_output;
#define USB libusb
QString dec2Hex4(int decimal) {
  return QString("%1").arg(decimal, 4, 16, QLatin1Char('0'));
}
std::vector<std::vector<unsigned char>>
split_buffer(std::vector<unsigned char> input, int cut_size) {
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

libusb_device **initialize_my_usb() {
  int err;
  ssize_t cnt;          // USB device count
  libusb_device **devs; // USB devices
  err = libusb_init(NULL);
  if (err) {
    std::cout << "\n\t The libusb library failed to initialise, returning";
    devs = 0;
    return devs;
  }
  cnt = libusb_get_device_list(NULL, &devs);
  std::cout << "number of usb devices " << cnt << '\n';
  return devs;
}

std::vector<QString> get_info(libusb_device **devs) {
  int i = 0;
  libusb_device *device;
  std::vector<QString> output;
  while ((device = devs[i++]) != NULL) {
    struct libusb_device_descriptor desc;
    int r = libusb_get_device_descriptor(device, &desc);
    if (r >= 0) {
      QString test = "";
      // printf("Bus: %03d Device: %03d: Device Id: %04x:%04x\n",
      // libusb_get_bus_number(device),libusb_get_device_address(device),
      // desc.idVendor, desc.idProduct );
      test = "BusNo: " + QString::number(libusb_get_bus_number(device)) +
             " DevNo: " + QString::number(libusb_get_device_address(device)) +
             "  VendorId:ProductId  " + dec2Hex4(desc.idVendor) + ":" +
             dec2Hex4(desc.idProduct);

      output.push_back(test);
    }
  }

  return output;
}
QString get_device_info(int busNo, int devNo, libusb_device **devs) {
  int j = 0;
  libusb_device *device;
  QString output;
  while ((device = devs[j++]) != NULL) {
    struct libusb_device_descriptor desc;
    libusb_get_device_descriptor(device, &desc);
    if (busNo == libusb_get_bus_number(device)) {
      if (devNo == libusb_get_device_address(device)) {
        // printf("\nYou have selected USB device : %04x:%04x:%04x\n\n",
        // desc.idVendor, desc.idProduct,desc.bcdUSB);
        output =
            "BCD device : " + QString::number(desc.bcdDevice) + "\n" +
            "BCD USB : " + QString::number(desc.bcdUSB) + "\n" +
            +"B descriptor type : " + QString::number(desc.bDescriptorType) +
            "\n" + "Device class : " + QString::number(desc.bDeviceClass) +
            "\n" + "Device protocol : " +
            QString::number(desc.bDeviceProtocol) + "\n" +
            "Device subclass : " + QString::number(desc.bDeviceSubClass) +
            "\n" + "Blength : " + QString::number(desc.bLength) + "\n" +
            "MaxPacket size : " + QString::number(desc.bMaxPacketSize0) + "\n" +
            "Num Configuretion : " + QString::number(desc.bNumConfigurations) +
            "\n" + "Product ID : " + QString::number(desc.idProduct) + "\n" +
            "Vendor ID : " + QString::number(desc.idVendor) + "\n" +
            "Manufacturer : " + QString::number(desc.iManufacturer) + "\n" +
            "Product : " + QString::number(desc.iProduct) + "\n" +
            "Serial Number : " + QString::number(desc.iSerialNumber) + "\n";
        return output;
      }
    }
  }
  return output;
}

std::vector<std::shared_ptr<cypress_device>>
initialize_and_select_cypress_devices() {
  libusb_init(NULL);
  //libusb_set_debug(NULL, 4);
  std::vector<std::shared_ptr<cypress_device>> cypress_devices;
  auto devs = initialize_my_usb();
  int i = 0;
  libusb_device *device_temp;
  std::vector<QString> output;
  while ((device_temp = devs[i++]) != NULL) {
    struct libusb_device_descriptor desc;
    int r = libusb_get_device_descriptor(device_temp, &desc);
    if (r >= 0) {

      if (desc.idVendor == 0x04b4) {
        auto temp = std::make_shared<cypress_device>(device_temp);
        // temp->device_descriptor=desc;
        cypress_devices.push_back(temp);
      }
    }
  }
  return cypress_devices;
}

/*libusb_device_handle *initialize_and_select_cypress() {
  libusb_init(NULL);
  // libusb_set_debug(NULL,4);
  libusb_device_handle *dev;
  // dev=libusb_open_device_with_vid_pid(NULL,0x04b4,0x1003);
  dev = libusb_open_device_with_vid_pid(NULL, 0x04b4, 0x00f1);
  if (!dev) {
    return dev;
  } else {
    std::cout << "issue selecting cypress"
              << "\n";
    exit(1);
  }
}
*/
std::vector<data_frame> get_data_from_cypress(
   std::shared_ptr<cypress_device> dev, int id,
    std::vector<std::tuple<unsigned int, unsigned int, float, unsigned int>>
        &stat) {
  const unsigned int TBs = 512 * 400;
  unsigned char output[TBs] = {0x00};
  // auto output_final=std::make_shared<unsigned char []> (TBs);
  std::vector<unsigned char> final_output;
  // auto output = std::make_shared<std::array<unsigned char, TBs>>();
  // auto output_shared=std::make_shared<unsigned char *>;
  int actual_length = 0;
  while (!actual_length) {
    actual_length = 0;
    auto t1 = std::chrono::high_resolution_clock::now();
    //libusb_bulk_transfer(dev, 0x86, output, sizeof(output), &actual_length,
    //                     2000);
     libusb_bulk_transfer(dev->device_handle, dev->input_endpoint, output, sizeof(output), &actual_length,
                         2000);
    std::cout << "transfered: " << actual_length << "\n";
    auto t2 = std::chrono::high_resolution_clock::now();
    float transfer_speed =
        (actual_length * 1000000.000) /
        (std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1)
             .count() *
         1000 * 1000);
    unsigned int transfer_length =
        std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    std::stringstream out_temp;
    out_temp << "string_output    id: " << id << "   ~" << transfer_speed
             << "~  MB/s"
             << "\n";
    stat.push_back(
        std::make_tuple(id, actual_length, transfer_speed, transfer_length));
    string_output.push_back(out_temp.str());
  }
  for (unsigned int i = 0; i < TBs; i++) {
    final_output.push_back(output[i]);
  }
  // std::cout<<"final_output size"<<final_output.size()<<"\n";
  auto splited_buffer = split_buffer(final_output, 512);
  // std::cout<<"splited_buffer size "<<splited_buffer.size()<<"\n";
  std::vector<data_frame> output_data_frames;
  for (auto data_i : splited_buffer) {
    data_frame temp(data_i);
    output_data_frames.push_back(temp);
  }
  // std::cout<<"before return get_data_from_cypress \n"
  //        <<"output_data_frames size is "<<output_data_frames.size()<<" \n";
  return output_data_frames;
}

void print_stat(
    std::vector<std::tuple<unsigned int, unsigned int, float, unsigned int>>
        stat) {
  unsigned int full_transfer = 0;
  for (auto rec : stat) {
    unsigned int id;
    unsigned int transfer_size;
    float transfer_speed;
    float transfer_time;
    std::tie(id, transfer_size, transfer_speed, transfer_time) = rec;
    std::cout << "thread :" << id << "   TSpeed :" << transfer_speed << " MB/s"
              << "  TSize: " << transfer_size << "   TTime: " << transfer_time
              << "\n";
    full_transfer += transfer_size;
  }
  std::cout << "_____________________________________________________________"
            << "\n";
  // std::cout<<full_transfer<<" Byte has been transfered in : "
  // <<full_transfer_time*1.0/1000000<<" s \n";
  // std::cout<<"average_transfer_speed: "<<
  // ((full_transfer*1000000.000)/(full_transfer_time*1000000)) << "  MB/s \n";
}

void populate_buffer(
    std::vector<std::shared_future<std::vector<data_frame>>> &data,
    buffers &data_buffer) {
  for (int i = 0; i < data.size(); i++) {
    //std::cout << i << "\n";
    std::vector<data_frame> temp = data[i].get();
    //std::cout << "data_get " << temp.size() << "\n";
    //std::cout<<"data_buffer "<<data_buffer.passed_time[1]<<"\n";
    data_buffer.insert_info_frames(temp);
  }
}

void get_x_data_samples(buffers &data_buffer,std::shared_ptr<cypress_device> dev,unsigned long long n_sample){
    std::vector<std::tuple<unsigned int, unsigned int, float, unsigned int>> stat;
    while(data_buffer.sample_count[0]<n_sample){
        int num_thread=4;
        std::vector<std::shared_future<std::vector<data_frame>>> data;
        std::vector<data_frame> final_data;
        std::cout << "going to transfer"
                  << "\n";
        for (int i = 1; i <= num_thread; i++) {
          data.push_back(std::async(std::launch::async, get_data_from_cypress,
                                    dev, i, std::ref(stat)));
          std::async(std::launch::async,populate_buffer,std::ref(data),std::ref(data_buffer));
        }
    }
}
