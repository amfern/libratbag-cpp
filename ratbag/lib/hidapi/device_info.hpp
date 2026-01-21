#include <cstdint>
#include <format>
#include <string_view>
#include <vector>

#include "hidapi.h"

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

  friend std::wostream &operator<<(std::wostream &os, const DeviceID &di);
  friend class HIDDeviceInfo;
};

using SerialNumber = std::wstring_view;
using ReleaseNumber = uint16_t;
using UsagePage = uint16_t;
using Usage = uint16_t;
using InterfaceNumber = int32_t;

class HIDDeviceInfo {

public:
  // TODO(ASK): should i return here a vector or be more vague/abstract and
  // return some "container" type?
  static const std::vector<HIDDeviceInfo> enumerate_hid_devices();

  /** Platform-specific device path */
  HIDPath path() const;
  DeviceID device_id() const;
  SerialNumber serial_number() const;
  /** Device Release Number in binary-coded decimal,
          also known as Device Version Number */
  ReleaseNumber release_number() const;
  std::wstring_view manufacturer_string() const;
  std::wstring_view product_string() const;
  /** Usage Page for this Device/Interface
          (Windows/Mac/hidraw only) */
  UsagePage usage_page() const;
  /** Usage for this Device/Interface
          (Windows/Mac/hidraw only) */

  Usage usage() const;
  InterfaceNumber interface_number() const;
  hid_bus_type bus_type() const;

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
  std::wstring_view ManufacturerString_;
  std::wstring_view ProductString_;
};

} // namespace hidapi

} // namespace lib

} // namespace ratbag

// template <class E> requires std::is_enum_v<E>
// auto format_as(E e) { return std::to_underlying(e); }

// template <typename T>
// using format_as_type = decltype(format_as(std::declval<T>()));

// template <class T>
//     requires requires { typename format_as_type<T>; }
// struct formatter<T> : formatter<format_as_type<T>> {
//     auto format(T e, auto& ctx) const {
//         return formatter<format_as_type<T>>::format(format_as(e), ctx);
//     }
// };

// auto format_as(hid_bus_type e) { return std::to_underlying(e); }

template <typename CharT> struct std::formatter<hid_bus_type, CharT> {
  constexpr auto parse(auto &ctx) {
    // TODO: what is this function needed for? what does it do?
    return ctx.begin();
  }

  auto format(const hid_bus_type &bus_type, auto &ctx) const {
    if constexpr (std::is_same_v<CharT, char>) {
      // TODO: i don't want to be implementing the switch case bellow for utf8
      // and wchar_t types...
    } else if constexpr (std::is_same_v<CharT, wchar_t>) {
      std::wstring_view name = L"unknown";

      switch (bus_type) {
      case HID_API_BUS_USB:
        name = L"USB";
        break;
      case HID_API_BUS_BLUETOOTH:
        name = L"Bluetooth";
        break;
      case HID_API_BUS_I2C:
        name = L"I2C";
        break;
      case HID_API_BUS_SPI:
        name = L"SPI";
        break;
      case HID_API_BUS_UNKNOWN:
        break;
      }
      return std::format_to(ctx.out(), L"{}", name);
    }
  }
};

template <typename CharT>
struct std::formatter<ratbag::lib::hidapi::DeviceID, CharT> {
  constexpr auto parse(auto &ctx) {
    // TODO: what is this function needed for? what does it do?
    return ctx.begin();
  }

  auto format(const ratbag::lib::hidapi::DeviceID &id, auto &ctx) const {
    if constexpr (std::is_same_v<CharT, char>) {
      return std::format_to(ctx.out(), "DeviceID(vid: {:#06x}, pid: {:#06x})",
                            id.vid(), id.pid());
    } else if constexpr (std::is_same_v<CharT, wchar_t>) {
      return std::format_to(ctx.out(), L"DeviceID(vid: {:#06x}, pid: {:#06x})",
                            id.vid(), id.pid());
    }
  }
};

template <typename CharT>
struct std::formatter<ratbag::lib::hidapi::HIDDeviceInfo, CharT> {
  constexpr auto parse(auto &ctx) { return ctx.begin(); }

  // Format the Point object.
  auto format(const ratbag::lib::hidapi::HIDDeviceInfo &info, auto &ctx) const {
    if constexpr (std::is_same_v<CharT, char>) {
      // TODO(ask): it is very in convenient to work with wchar_t...
      //       I have to convert path to wstring, and because of hidapi decision
      //       to use wchar, it forces forces my code to use wide chars every
      //       where. also it make this situation impossible, and i can't print
      //       into std::cout... It's better to fix the issue at the root, and
      //       prevent the hidapi library from convert utf8 to wchar_t with. eg:
      //       https://github.com/libusb/hidapi/blob/a7587175f35ddcbdfb1ce5a1e4dd7a268699ba7f/linux/hid.c#L736
      //       And let the c++ pick the correct char encoding fo the platform.
      //       It will make the library abit more cross platform.
    } else if constexpr (std::is_same_v<CharT, wchar_t>) {
      std::wstring wpath(info.path().length(),
                         L' '); // Make room for characters
      std::copy(info.path().begin(), info.path().end(), wpath.begin());

      return std::format_to(
          ctx.out(),
          L"HIDDeviceInfo(path: {}, deviceId: {},serial_number: {}, "
          L"manufacturer_string: {}, product_string = {}, usage_page "
          L"= {}, usage = {}, interface_number = {}, bus_type = {})",
          wpath, info.device_id(), info.serial_number(),
          info.manufacturer_string(), info.product_string(), info.usage_page(),
          info.usage(), info.interface_number(), info.bus_type());
    }
  }
};
