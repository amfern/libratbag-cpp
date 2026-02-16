#pragma once

#include <cstdint>
#include <format>
#include <string_view>
#include <vector>

#include "hidapi.h"
#include "ratbag/lib/hidapi/detail/hid_bus_type.hpp"

namespace ratbag {
namespace lib {
namespace hidapi {

using HIDPath = std::string_view;
using ProductID = uint16_t;
using VendorID = uint16_t;

class DeviceID {

public:
  VendorID vid() const;
  ProductID pid() const;

private:
  VendorID vid_;
  ProductID pid_;

  explicit DeviceID(ProductID vid, VendorID pid);

  friend std::ostream &operator<<(std::ostream &os, const DeviceID &di);
  friend class HIDDeviceInfo;
};

class HIDDeviceInfo;
  
using SerialNumber = std::string_view;
using ReleaseNumber = uint16_t;
using UsagePage = uint16_t;
using Usage = uint16_t;
using InterfaceNumber = int32_t;
using HIDDeviceInfoList = std::vector<HIDDeviceInfo>;
using HidBusType = ratbag::lib::hidapi::detail::HidBusType;

class HIDDeviceInfo {

public:

  static const HIDDeviceInfoList enumerate_hid_devices();

  /** Platform-specific device path */
  HIDPath path() const;
  DeviceID device_id() const;
  SerialNumber serial_number() const;
  /** Device Release Number in binary-coded decimal,
          also known as Device Version Number */
  ReleaseNumber release_number() const;
  std::string_view manufacturer_string() const;
  std::string_view product_string() const;
  /** Usage Page for this Device/Interface
          (Windows/Mac/hidraw only) */
  UsagePage usage_page() const;
  /** Usage for this Device/Interface
          (Windows/Mac/hidraw only) */

  Usage usage() const;
  InterfaceNumber interface_number() const;
  HidBusType bus_type() const;

  ~HIDDeviceInfo(); // destructor

  HIDDeviceInfo(const HIDDeviceInfo &other) = delete; // copy constructor
  HIDDeviceInfo(HIDDeviceInfo &&other) noexcept;      // move constructor
  HIDDeviceInfo &operator=(const HIDDeviceInfo &rhs) = delete; // copy operator
  HIDDeviceInfo &operator=(HIDDeviceInfo &&rhs) noexcept;      // move operator

  friend std::wostream &operator<<(std::wostream &os,
                                   const HIDDeviceInfo &info);
private:
  explicit HIDDeviceInfo(hid_device_info &device_info);

  struct hid_device_info *device_info_;

  HIDPath HIDPath_;
  DeviceID DeviceID_;
  std::string SerialNumber_;
  std::string ManufacturerString_;
  std::string ProductString_;
};

} // namespace hidapi

} // namespace lib

} // namespace ratbag


template <>
struct std::formatter<ratbag::lib::hidapi::DeviceID> {
  constexpr auto parse(auto &ctx) {
    // TODO: what is this function needed for? what does it do?
    return ctx.begin();
  }

  auto format(const ratbag::lib::hidapi::DeviceID &id, auto &ctx) const {
    return std::format_to(ctx.out(), "DeviceID(vid: {:#06x}, pid: {:#06x})",
                          id.vid(), id.pid());
  }
};

template <>
struct std::formatter<ratbag::lib::hidapi::HIDDeviceInfo> {
  constexpr auto parse(auto &ctx) { return ctx.begin(); }

  // Format the Point object.
  auto format(const ratbag::lib::hidapi::HIDDeviceInfo &info, auto &ctx) const {
      return std::format_to(
          ctx.out(),
          "HIDDeviceInfo(path: {}, deviceId: {},serial_number: {}, "
          "manufacturer_string: {}, product_string = {}, usage_page "
          "= {}, usage = {}, interface_number = {}, bus_type = {})",
          info.path(), info.device_id(), info.serial_number(),
          info.manufacturer_string(), info.product_string(), info.usage_page(),
          info.usage(), info.interface_number(), info.bus_type());
  }
};
