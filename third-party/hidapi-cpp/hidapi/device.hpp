#pragma once

// prefer header include with full prefix, long explicit paths
#include "hidapi/device_info.hpp"
#include "hidapi/hid_report.hpp"

// C headers after project specific ones before c++ std one, and just use IWYU
// to lint it all
#include "hidapi.h"

// it's better to include after the project .hpp files, to avoid accidently
// making thing build even though other header didn't include eg. <vector>
#include <chrono>
#include <cstddef>
#include <span>
#include <vector>

namespace hidapi {

using ReadTimeoutMilli = std::chrono::duration<uint64_t, std::milli>;

class HIDDevice {

public:
  // TODO: inside this function i will call std::move(device_info).
  //            Which can caught the user off guard, because the device_info
  //            moved. So is it good to explicity ask for rvalue(HIDDeviceInfo
  //            &&device_info)? or should i use "static HIDDevice
  //            popen(HIDDeviceInfo device_info) {"
  // TODO: return const & is okay, but never return const because it will lead
  // to wierd things and copy constructor to be called.
  // TODO: read
  // https://github.com/xuchen-tech/Books/blob/main/C%2B%2B%20Templates%20The%20Complete%20Guide%2C%202nd%20Edition%20[BooxRack].pdf
  // ussually we copy pass by const &, but
  // clang tidy catches use after move
  template <typename T> static HIDDevice open(T device_info) {
    hid_device *handle = hid_open_path(device_info.path().data());
    if (handle == nullptr) {
      HIDAPIString err(hid_error(nullptr));
      throw std::runtime_error(err.toString());
    }

    return HIDDevice(handle, std::forward<T>(device_info));
  };

  HIDDevice(const HIDDevice &other) = delete;          // copy constructor
  HIDDevice(HIDDevice &&other) noexcept;               // move constructor
  HIDDevice &operator=(const HIDDevice &rhs) = delete; // copy operator
  HIDDevice &operator=(HIDDevice &&rhs) noexcept;      // move operator

  ~HIDDevice(); // destructor

  bool isValid() const;

  HIDDeviceInfo &deviceInfo();

  // - read max_length
  // - read untile timeout is reached
  // - timeout of 0, will read max_length and exit imidiatly
  // note: We dont' support block from wait C hipapi, as it may result in
  // deadlock and not something an api should get into even by mistake
  std::optional<HIDBuffer> read(std::size_t max_length,
                                ReadTimeoutMilli timeout = ReadTimeoutMilli{0});
  void write(HIDBuffer buf);

  std::optional<HIDReport> receive_feature_report(ReportID report_id,
                                                  std::size_t length);
  void send_feature_report(HIDReport report);

private:
  explicit HIDDevice(hid_device *handle, HIDDeviceInfo device_info);

  hid_device *handle_;
  HIDDeviceInfo device_info_;
};

} // namespace hidapi
