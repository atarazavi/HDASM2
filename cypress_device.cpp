#include "cypress_device.h"
#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>
#include <stdlib.h>
cypress_device::cypress_device(libusb_device *dev) {
  this->device = dev;
  // this->bus_number=libusb_get_bus_number(dev);
  // this->device_number=libusb_get_device_address(dev);
  libusb_get_device_descriptor(this->device, &this->device_descriptor);
  if (this->device_descriptor.idProduct == 0x1003) {
    this->output_endpoint = 0x02;
    this->input_endpoint = 0x86;
      this->device_type="USB2";
  } else if (this->device_descriptor.idProduct == 0x00f1) {
    this->output_endpoint = 0x01;
    this->input_endpoint = 0x81;
      this->device_type="USB3";
  }
}
void cypress_device::bulk_send(unsigned char *input) {
  const int buff_size = 512;
  unsigned char send[buff_size] = {0x00};
  for (int i = 0; i < buff_size; i++) {
    send[i] = *(input + i);
  }

  if (sizeof(send) != 512) {
    std::cout << "size is not 512\n";
    exit(1);
  }
  int err = 0;
  int actual_length = 0;
  //err = libusb_bulk_transfer(this->device_handle, this->output_endpoint, send,
  //                           sizeof(send), &actual_length, 1000);
  if(this->device_type=="USB3"){
  err = libusb_bulk_transfer(this->device_handle, this->output_endpoint, send,
                               8, &actual_length, 1000);
  }else{
      err = libusb_bulk_transfer(this->device_handle, this->output_endpoint, send,
                                 sizeof(send), &actual_length, 1000);
  }
  if (err) {
    std::cout << "cypress_send(): something went wrong in setting\n";
  }
}

void cypress_device::open_device() {
    auto status = libusb_open(this->device, &this->device_handle);
    if (!status) {
      std::cout << "device has been opend\n";
    } else {
      std::cout << "something went wrong in opening the device\n";
      exit(1);
    }
  libusb_set_auto_detach_kernel_driver(this->device_handle, 1);
  auto status_claim = libusb_claim_interface(this->device_handle, 0);
  if (status_claim != LIBUSB_SUCCESS){
      libusb_close(this->device_handle);
      std::cout<<"could not claim device\n";
      exit(1);
  }
}


void cypress_device::set() {
  const int buff_size = 512;
  unsigned char control[buff_size] = {0x67, 0x62, 0x86, 0x19,
                                      0x01, 0xff, 0x0a, 0x00};
  // unsigned char control[buff_size]={0x67,0x62,0x86,0x19,0x01,0xff,0x08,0x00};
  unsigned char init[buff_size] = {0x67, 0x62, 0x89, 0x19,
                                   0x00, 0x00, 0x00, 0x00};
  for (int i = 7; i < buff_size; i++) {
    control[i] = 0x00;
    init[i] = 0x00;
  }
  this->bulk_send(init);
  std::chrono::milliseconds timespan(3000);
  std::this_thread::sleep_for(timespan);
  this->bulk_send(control);
}
void cypress_device::start() {
  const int buff_size = 512;
  unsigned char start[buff_size] = {0x67, 0x62, 0x88, 0x19,
                                    0x00, 0x00, 0x00, 0x00};
  for (int i = 7; i < buff_size; i++) {
    start[i] = 0x00;
  }
  this->bulk_send(start);
}
void cypress_device::stop() {
  const int buff_size = 512;
  unsigned char stop[buff_size] = {0x67, 0x62, 0x87, 0x19,
                                   0x00, 0x00, 0x00, 0x00};
  for (int i = 7; i < buff_size; i++) {
    stop[i] = 0x00;
  }
  this->bulk_send(stop);
}

void cypress_device::close_device(){
    libusb_release_interface(this->device_handle, 0);
    libusb_close(this->device_handle);
}

cypress_device::~cypress_device(){
    this->close_device();
}

std::vector<unsigned char>
cypress_device::bulk_recive(int num_frame) {
  auto id = this->thread_id;
  this->thread_id++;
  const unsigned int TBs = 512 * num_frame;
  unsigned char *temp_output;
  auto allocation_status = (unsigned char *) calloc (TBs,sizeof(unsigned char *));
    if(!allocation_status){
        std::cout<<"allocation error\n";
        exit(1);
    }
  std::vector<unsigned char> output;
  int transfered_size = 0;
  while (!transfered_size) {
    transfered_size = 0;
    auto t1 = std::chrono::high_resolution_clock::now();
    libusb_bulk_transfer(this->device_handle, this->input_endpoint, temp_output,
                         sizeof(temp_output), &transfered_size, 2000);
    std::cout << "transfered: " << transfered_size << "\n";
    auto t2 = std::chrono::high_resolution_clock::now();
    float transfer_speed =
        (transfered_size * 1000000.000) /
        (std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1)
             .count() *
         1000 * 1000);
    unsigned int transfer_length =
        std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    std::stringstream out_temp;
    out_temp << "string_output    id: " << id << "   ~" << transfer_speed
             << "~  MB/s"
             << "\n";
    this->stat.push_back(
        std::make_tuple(id, transfered_size, transfer_speed, transfer_length));
    this->output_string.push_back(out_temp.str());
  }
  for (unsigned int i = 0; i < TBs; i++) {
    output.push_back(temp_output[i]);
  }
  return output;
}

/*std::vector<data_frame>
cypress_device::get_data_frames(int num_frame){
    std::vector<std::shared_ptr< data_frame> > output.
    auto char_buffer=this->bulk_recive(num_frame);
    auto splited_buffer=m_split_buffer(char_buffer, 512);
    for(auto temp_buffer:splited_buffer){
        data_frame temp{temp_buffer};

    }

}
*/
