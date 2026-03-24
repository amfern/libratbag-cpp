#pragma once

#include "hidapi.h"
#include "ratbag/lib/hidapi/device_info.hpp"

namespace ratbag {
namespace lib {
namespace hidapi {

// TODO: Create proper wrapper class for hid_device
// https://github.com/libusb/hidapi/blob/657b9fa147722ad59d045965e625d3972fa1264c/hidapi/hidapi.h#L284
class HIDDevice {

public:
  static HIDDevice open(HIDDeviceInfo &device_info) {
    return HIDDevice(device_info);
  };

private:
  explicit HIDDevice(HIDDeviceInfo &device_info);
};

} // namespace hidapi

} // namespace lib

} // namespace ratbag
