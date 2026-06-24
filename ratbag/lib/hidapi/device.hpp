#pragma once

#include "hidapi.h"
#include "ratbag/lib/hidapi/device_info.hpp"

namespace ratbag {
namespace lib {
namespace hidapi {

// using HIDDeviceHandle = hid_device;

// TODO: Create proper wrapper class for hid_device
// https://github.com/libusb/hidapi/blob/657b9fa147722ad59d045965e625d3972fa1264c/hidapi/hidapi.h#L284
class HIDDevice {

public:

  // TODO(ask): inside this function i will call std::move(device_info).
  //            Which can caught the user of this function off guard, because the device_info moved.
  //            So is it good to explicity ask for rvalue(HIDDeviceInfo &&device_info)?
  //            or should i use "static HIDDevice popen(HIDDeviceInfo device_info) {"
  template<typename T>
  static HIDDevice open(T &&device_info) {
    auto vid = device_info.device_id().vid();
    auto pid = device_info.device_id().pid();
    auto serial_number = device_info.serial_number();

    hid_device* handle = hid_open(vid, pid, serial_number.data());
	if (handle == nullptr) {
        throw std::runtime_error("Unable to open device");
	}

    return HIDDevice(handle, std::forward<T>(device_info));
  };

  // std::array<std::byte, 3> byteArray{ std::byte{0x01}, std::byte{0x0A}, std::byte{0xFF} };
  // TODO(ask): should i use span here? or vector?
  //           with span i assume the storage is managed by the caller, the caller can create alot of coppies by not using std::move
  // void hid_write(std::vector<std::byte> bytes);
  // void hid_read();

  ~HIDDevice(); // destructor

  HIDDevice(const HIDDevice &other) = delete; // copy constructor
  HIDDevice(HIDDevice &&other) noexcept;      // move constructor
  HIDDevice &operator=(const HIDDevice &rhs) = delete; // copy operator
  HIDDevice &operator=(HIDDevice &&rhs) noexcept;      // move operator

private:
  explicit HIDDevice(hid_device* handle, HIDDeviceInfo device_info);

  hid_device* handle_;
  HIDDeviceInfo device_info_;
};

} // namespace hidapi

} // namespace lib

} // namespace ratbag
