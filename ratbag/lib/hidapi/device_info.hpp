#include <cstdint>
#include <string_view>
#include <vector>
#include <format>


#include "hidapi.h"



namespace ratbag {
namespace lib {
namespace hidapi {

using HIDPath = std::string_view;
using ProductID = uint16_t;
using VendorID = uint16_t;

// TODO: convert it to class with move operator
// using DeviceID =

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
  // TODO(ASK): should i return here a vector or be more vague/abstract. and
  // return an iterator....?
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
  // regatrding that "reference is just an pointer with limitation",
  // does it mean that if i do "const &" it will return an pointer.
  //            And the caller will receive and pointer that it will have to
  //            dereference? Usage is just an uint16_t, should i just return by
  //            value and let it be copied?

  // yes! the reason for using reference
  // 1. avoid copying the thing if the thing is expansive to copy
  // 2. To make sure the returned values stays synced if something changes whithin the class
  // string_view is an alredy and reference, so we cna return it by value. It's
  // cheap to copy. cheaper than dereferencing an address
  Usage usage() const;
  InterfaceNumber interface_number() const;
  hid_bus_type bus_type() const;

  ~HIDDeviceInfo(); // destructor

  // TODO: create copy constructor
  HIDDeviceInfo(const HIDDeviceInfo &other) = delete; // copy constructor
  HIDDeviceInfo(HIDDeviceInfo &&other) noexcept;      // move constructor
  HIDDeviceInfo &operator=(const HIDDeviceInfo &rhs) = delete; // copy operator
  HIDDeviceInfo &operator=(HIDDeviceInfo &&rhs) noexcept;      // move operator

  friend std::wostream &operator<<(std::wostream &os,
                                   const HIDDeviceInfo &info);

private:
  explicit HIDDeviceInfo(hid_device_info &device_info);

  // No difference between pointer and reference in terms of performance...
  // refernce is an c++ syntatic sugar, can't be null.
  // Compiler cannot optimize reference more than an pointer.
  // Underlying architecture still passes raw pointer even if it's reference.
  // Reference is just to limit the programmer and communicate that it can't be
  // null Using reference as class member tights instance of the class to the
  // object unrevocably

  // I want it to be reference because it gives me the assurance it's
  // not going to be nullptr DeviceInfo class cannot exist without
  // hid_device_info. However, the other side is that i can't move HIDDeviceInfo
  // class. Why can't i have both, being able to tell c++ that this object has
  // to be always valid. And also being able to move the objects... Is it up to
  // me to enforce??? is my class internal implementation details. so it's okay
  // to not have device_info_ reference?
  //
  // It feels like c++ gives me the means to implement memory safety, but
  // doesn't implement it by it self out of the box. non_null.hpp

  struct hid_device_info *device_info_;
  // TODO(ASK): can you recommend me an C++ project that i can treat as an best
  // practice reference for modern C++, so i can siphon ideas and check how
  // things should be done. BOOST - abit mixed bag, with legacy and modern
  // https://github.com/ericniebler/range-v3 - abit complicated, things i really
  // don't need to know about this point, full of modern yet complicated bits.
  // https://github.com/fmtlib/fmt - modern C++

  HIDPath HIDPath_;
  DeviceID DeviceID_;
  SerialNumber SerialNumber_;
  std::wstring_view ManufacturerString_;
  std::wstring_view ProductString_;
};

} // namespace hidapi

} // namespace lib

} // namespace ratbag


template <typename CharT>
struct std::formatter<ratbag::lib::hidapi::DeviceID, CharT> {
    constexpr auto parse(auto& ctx) {
        // TODO: what is this function needed for?
        return ctx.begin();
    }

    // Format the Point object.
    auto format(const ratbag::lib::hidapi::DeviceID& id, auto& ctx) const {
        if constexpr (std::is_same_v<CharT, char>) {
            return std::format_to(ctx.out(), "DeviceID(vid: {:#06x}, pid: {:#06x})", id.vid(), id.pid());
        } else if constexpr (std::is_same_v<CharT, wchar_t>) {
            return std::format_to(ctx.out(), L"DeviceID(vid: {:#06x}, pid: {:#06x})", id.vid(), id.pid());
        }
    }
};


template <typename CharT>
struct std::formatter<ratbag::lib::hidapi::HIDDeviceInfo, CharT> {
    constexpr auto parse(auto& ctx) {
        // TODO: what is this function needed for?
        return ctx.begin();
    }

    // Format the Point object.
    auto format(const ratbag::lib::hidapi::HIDDeviceInfo& info, auto& ctx) const {
        if constexpr (std::is_same_v<CharT, char>) {
            // return std::format_to(ctx.out(), "DeviceID(vid: {:#06x}, pid: {:#06x})", id.vid(), id.pid());
        } else if constexpr (std::is_same_v<CharT, wchar_t>) {

          // TODO: it is very in convenient to work with wchar_t...
          //       I have to convert path to wstring and also it forces my code to use wide chars every where.
          std::wstring wpath(info.path().length(), L' '); // Make room for characters
          std::copy(info.path().begin(), info.path().end(), wpath.begin());

          return std::format_to(
              ctx.out(),
              L"HIDDeviceInfo(path: {}, deviceId: {},serial_number: {}, "
              L"manufacturer_string: {}, product_string = {}, usage_page "
              L"= {}, usage = {}, interface_number = {})",
              wpath, info.device_id(), info.serial_number(),
              info.manufacturer_string(), info.product_string(),
              info.usage_page(), info.usage(), info.interface_number());
        }
    }
};


