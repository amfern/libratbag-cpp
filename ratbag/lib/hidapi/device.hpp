#pragma once

#include "hidapi.h"
#include "ratbag/lib/hidapi/device_info.hpp"
#include <cstddef>
#include <vector>
#include <chrono>
#include <span>

namespace ratbag {
namespace lib {
namespace hidapi {

using namespace std::chrono_literals;

// using HIDDeviceHandle = hid_device;

using HIDBuffer = std::vector<std::byte>;
using ReadTimeoutMilli = std::chrono::milliseconds;
using ReportID = std::byte;

class HIDReport {
friend class HIDDevice;
private:
  // TODO: use std::array instead of std::vector
  std::vector<std::byte> data_;
    
public:
  ReportID &report = data_[0];
  std::span<std::byte> data = std::span<std::byte>{data_}.subspan(1);

  HIDReport(ReportID report, std::size_t count) : data_(count + 1) {
    this->report = report; 
  }
};

// TODO: Create proper wrapper class for hid_device
// https://github.com/libusb/hidapi/blob/657b9fa147722ad59d045965e625d3972fa1264c/hidapi/hidapi.h#L284
class HIDDevice {
 
public:

  // TODO(ask): inside this function i will call std::move(device_info).
  //            Which can caught the user of this function off guard, because the device_info moved.
  //            So is it good to explicity ask for rvalue(HIDDeviceInfo &&device_info)?
  //            or should i use "static HIDDevice popen(HIDDeviceInfo device_info) {"
  // TODO: return const & is okay, but never return const because it will lead to wierd things and copy constructor to be called.
  // TODO: read https://github.com/xuchen-tech/Books/blob/main/C%2B%2B%20Templates%20The%20Complete%20Guide%2C%202nd%20Edition%20[BooxRack].pdf
  // ussually we copy pass by const &, but
  // clang tidy catches use after move
  template<typename T>
  static HIDDevice open(T device_info) {

    hid_device* handle = hid_open_path(device_info.path().data());
	if (handle == nullptr) {
        throw std::runtime_error("Unable to open device");
	}

    return HIDDevice(handle, std::forward<T>(device_info));
  };

  void write(HIDBuffer buf);
  std::optional<HIDBuffer> read(std::size_t length, ReadTimeoutMilli timeout = ReadTimeoutNone);
  
  void send_feature_report(HIDReport report);
  std::optional<HIDReport> receive_feature_report(ReportID report_id, std::size_t length);

  ~HIDDevice(); // destructor

  HIDDevice(const HIDDevice &other) = delete; // copy constructor
  HIDDevice(HIDDevice &&other) noexcept;      // move constructor
  HIDDevice &operator=(const HIDDevice &rhs) = delete; // copy operator
  HIDDevice &operator=(HIDDevice &&rhs) noexcept;      // move operator

  // TODO(ask): what do you thin about such magic constant
  static constexpr ReadTimeoutMilli ReadTimeoutBlock{-1};
  static constexpr ReadTimeoutMilli ReadTimeoutNone{0};

private:
  explicit HIDDevice(hid_device* handle, HIDDeviceInfo device_info);

  hid_device* handle_;
  HIDDeviceInfo device_info_;
};

} // namespace hidapi

} // namespace lib

} // namespace ratbag
