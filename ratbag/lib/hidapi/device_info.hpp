#include <cstdint>
#include <vector>

#include "hidapi.h"

namespace ratbag {
namespace lib {
namespace hidapi {

using HIDPath = std::string_view;
using ProductID = uint16_t;
using VendorID = uint16_t;
using DeviceID = std::tuple<const VendorID&, const ProductID&>; 
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
  // TODO: is possible to pu this function as an global static and not part of an class?
  // TODO: maybe it's better to use std::span with custom iterator that iterates over "struct hid_device_info *next;"
  static const std::vector<HIDDeviceInfo> enumerate_hid_devices();

  /** Platform-specific device path */
  const HIDPath& path();
  const DeviceID& device_id();
  const SerialNumber& serial_number();
  /** Device Release Number in binary-coded decimal,
	  also known as Device Version Number */
  const ReleaseNumber& release_number();
  const std::wstring_view& manufacturer_string();
  const std::wstring_view& product_string();
  /** Usage Page for this Device/Interface
	  (Windows/Mac/hidraw only) */
  const UsagePage& usage_page();
  /** Usage for this Device/Interface
	  (Windows/Mac/hidraw only) */
  const Usage& usage();
  const InterfaceNumber& interface_number();
  const HidBusType& bus_type();

  // TODO: this function shouldn't be called by the user, user can't invent hid devices. only enumerate_hid_devices
  explicit HIDDeviceInfo(hid_device_info &device_info);
  ~HIDDeviceInfo();                           // destructor

  HIDDeviceInfo(const HIDDeviceInfo& other) = delete;           // copy constructor
  HIDDeviceInfo(HIDDeviceInfo &&other) noexcept;                // move constructor
  HIDDeviceInfo& operator=(const HIDDeviceInfo& rhs);           // copy assignment
  HIDDeviceInfo& operator=(HIDDeviceInfo&& rhs) noexcept;       // move operator

private:
  struct hid_device_info &device_info_;

  const HIDPath HIDPath_;
  const DeviceID DeviceID_;
  const SerialNumber SerialNumber_;
  const std::wstring_view ManufacturerString_;
  const std::wstring_view ProductString_;
};

} // namespace hid

} // namespace lib

} // namespace ratbag
