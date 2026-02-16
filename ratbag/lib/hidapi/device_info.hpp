#pragma once

#include <cstdint>
#include <format>
#include <string_view>
#include <vector>
#include <codecvt>

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
  
// TODO:
// By default, the HIDAPI library uses wchar_t in its API for returning strings.
// This choice was primarily made to align with the Windows API, which natively uses wchar_t.
// However, this design introduces several issues for downstream users:
// - On Linux and macOS, it requires converting UTF-8 strings to wchar_t, adding unnecessary conversion overhead (for example, see the implementation in the HIDAPI Linux backend).
// - It forces the use of wchar_t throughout the codebase, propagating the same requirement downstream to dependent projects.
// - It complicates otherwise simple scenarios—for instance, using std::cout is no longer sufficient and must be replaced with std::wcout. Additionally, many C++ libraries and containers do not fully support wchar_t.
// 
// For a cross-platform library, it’s generally better to avoid enforcing either wchat_t or UTF-8(char*) at the API level.
// Instead, the API should align with the native string type of the target operating system, 
// since most UI frameworks and system libraries expect the platform’s default encoding
// (UTF-8 on Unix-like systems, wide strings on Windows).
// This approach avoids unnecessary allocations and string re-encoding.
// A better design for HIDAPI would be to use C++ templates to select the appropriate character type per platform,
// or alternatively use conditional compilation (#ifdef) in C to adjust the return type based on the target OS.
// Adopting this strategy would make HIDAPI more natural and efficient to use in cross-platform projects.
using HIDAPIString = std::wstring_view;

using SerialNumber = HIDAPIString;
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
  HIDAPIString manufacturer_string() const;
  HIDAPIString product_string() const;
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
  SerialNumber SerialNumber_;
  HIDAPIString ManufacturerString_;
  HIDAPIString ProductString_;
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
struct std::formatter<ratbag::lib::hidapi::HIDAPIString> {
  constexpr auto parse(auto &ctx) { return ctx.begin(); }

  // Format the Point object.
  auto format(const ratbag::lib::hidapi::HIDAPIString &hidapi_string, auto &ctx) const {
    // Before C++26, the recommended approach for UTF-8 conversions remains
    // std::codecvt_utf8
    // (https://en.cppreference.com/w/cpp/locale/codecvt_utf8), even though it
    // is deprecated. Use the converter’s from_bytes() function to convert
    // wchar_t to a UTF-8 string. Note that this is not ideal, as it requires
    // additional memory allocation.
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string string_utf8 = converter.to_bytes(hidapi_string.data());

    return std::format_to(ctx.out(), "{}", string_utf8);
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
        info.path(), info.device_id(), info.serial_number(), info.manufacturer_string(),
        info.product_string(), info.usage_page(), info.usage(),
        info.interface_number(), info.bus_type());
  }
};
