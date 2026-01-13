#include <iostream>

#include "ratbag/lib/libratbag.hpp"

using ratbag::lib::Device;
using ratbag::lib::hidapi::HIDDeviceInfo;

int main() {
  auto deviceInfos = HIDDeviceInfo::enumerate_hid_devices();
  for (auto &info : deviceInfos) {
    auto [vid, pid] = info.device_id();
    std::cout << "vid = " << vid << "pid = " << pid;
  }

  // std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // 2. Open Device with corresponding driver to the HID based on HID
  // description and Path
  // Device<Driver>::Open();

  std::cout << "Hello, Modern C++ World!" << std::endl;

  return 0;
}
