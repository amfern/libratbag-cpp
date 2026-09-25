#include "ratbag/lib/libratbag.hpp"

// #include <format>
#include <iostream>

// using ratbag::lib::Device;
using hidapi::HIDDeviceInfo;

int main() {
  auto deviceInfos = HIDDeviceInfo::enumerate_hid_devices();
  std::println("List of HID devices");

  for (auto &info : deviceInfos) {
    std::println("Available: {}.", info);
  }

  // auto device = HIDDevice::open(std::move(deviceInfos[1]));
  // auto read_res = device.read();

  // std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // 2. Open Device with corresponding driver to the HID based on HID
  // description and Path
  // Device<Driver>::Open();

  return 0;
}
