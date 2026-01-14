#include <cstdint>
#include <string_view>
#include <vector>

#include "hidapi.h"

namespace ratbag {
namespace lib {
namespace hidapi {

using HIDPath = std::string_view;
using ProductID = uint16_t;
using VendorID = uint16_t;
// TODO: using DeviceID = std::tuple<const VendorID&, const ProductID&>; doesn't
// work in the move operator using DeviceID = std::tuple<const VendorID&, const
// ProductID&>;
using DeviceID = std::tuple<VendorID &, ProductID &>;
using SerialNumber = std::wstring_view;
using ReleaseNumber = uint16_t;
using UsagePage = uint16_t;
using Usage = uint16_t;
using InterfaceNumber = int32_t;

// TODO: how to wrap c-style typedef enum with C++ enum?
using HidBusType = hid_bus_type;

class HIDDeviceInfo {

public:
  // TODO: is it possible to use array here without knowing the size upfront?
  // TODO: is possible to pu this function as an global static and not part of
  // an class?
  // TODO: maybe it's better to use std::span with custom iterator that iterates
  static const std::vector<HIDDeviceInfo> enumerate_hid_devices();
  // over "struct hid_device_info *next;"

  /** Platform-specific device path */
  const HIDPath &path() const;
  const DeviceID &device_id() const;
  const SerialNumber &serial_number() const;
  /** Device Release Number in binary-coded decimal,
          also known as Device Version Number */
  const ReleaseNumber &release_number() const;
  // TODO: we should have the wstring, only when compiled for windows...
  const std::wstring_view &manufacturer_string() const;
  const std::wstring_view &product_string() const;
  /** Usage Page for this Device/Interface
          (Windows/Mac/hidraw only) */
  const UsagePage &usage_page() const;
  /** Usage for this Device/Interface
          (Windows/Mac/hidraw only) */
  const Usage &usage() const;
  const InterfaceNumber &interface_number() const;
  const HidBusType &bus_type() const;

  ~HIDDeviceInfo(); // destructor

  // TODO: create copy constructor
  HIDDeviceInfo(const HIDDeviceInfo &other) = delete; // copy constructor
  HIDDeviceInfo(HIDDeviceInfo &&other) noexcept;      // move constructor
  HIDDeviceInfo &operator=(const HIDDeviceInfo &rhs) = delete; // copy operator
  HIDDeviceInfo &operator=(HIDDeviceInfo &&rhs) noexcept;      // move operator

private:
  // TODO: this function shouldn't be called by the user, user can't invent hid
  // devices. only enumerate_hid_devices
  explicit HIDDeviceInfo(hid_device_info *device_info);

  // TODO: I want it to be reference because it gives me the assurance it's not
  // going to be nullptr DeviceInfo class cannot exist without hid_device_info.
  // However, the other side is that i can't move HIDDeviceInfo class.
  // Why can't i have both, being able to tell c++ that this object has to be
  // always valid. And also being able to move the objects... Is it up to me to
  // enforce??? is my class internal implementation details. so it's okay to not
  // have device_info_ reference?
  //
  // return static_cast<const Usage &>(device_info_->usage);
  // No difference between pointer and reference in terms of performance...
  // refernce is an c++ syntatic sugar, can't be null.
  // Compiler cannot optimize reference more than an pointer.
  // Underlying architecture still passes raw pointer even if it's reference.
  // Reference is just to limit the programmer and communicate that it can't be
  // null Using reference as class member tights instance of the class to the
  // object unrevocably
  struct hid_device_info *device_info_;

  // TODO: i want those members to be const because i don't expect them to
  // change, but it creates an issue with move operator, i cannot assign to
  // const members... is the best practice to not have const members?
  // Ans: Having const members is a bad idea in C++, just have them private and
  // expose accessor through const & functions
  HIDPath HIDPath_;
  DeviceID DeviceID_;
  SerialNumber SerialNumber_;
  std::wstring_view ManufacturerString_;
  std::wstring_view ProductString_;
};

} // namespace hidapi

} // namespace lib

} // namespace ratbag
