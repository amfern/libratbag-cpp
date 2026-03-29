#include <format>
#include <iostream>

#include "ratbag/lib/libratbag.hpp"

using ratbag::lib::Device;
using ratbag::lib::hidapi::HIDDeviceInfo;

int main() {
  auto deviceInfos = HIDDeviceInfo::enumerate_hid_devices();
  std::cout << "List of HID devices" << std::endl;

  for (auto &info : deviceInfos) {
    std::cout << std::format("Available: {}.", info) << std::endl;
  }

  // std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // 2. Open Device with corresponding driver to the HID based on HID
  // description and Path
  // Device<Driver>::Open();
  std::vector<Device> devices;
  for (auto &info : deviceInfos) {
    auto device = Device::open(info);
    if (device.has_value()) {
      std::cout << std::format("found device {}.", info) << std::endl;      
      devices.push_back(std::move(device.value()));
    }
  }

  return 0;
}
