#include <cstdint>
#include <tuple>

#include "ratbag/lib/hidapi/device_info.hpp"

namespace ratbag {
namespace lib {
namespace hidapi {

// TODO: the idea is to wrap around the memory and not copy it...
//       should i use std::span with custom itereator that uses the
//       cur_dev->next?
const std::vector<HIDDeviceInfo> HIDDeviceInfo::enumerate_hid_devices() {
  struct hid_device_info *devs, *cur_dev;
  devs = hid_enumerate(0, 0); // 0,0 = find all devices

  // count the devices to prepare the capacity for the vector, to avoid
  // reallocation
  // TODO: we can use std::list and convert it later to array. however there
  // would be more move opereations.
  cur_dev = devs;
  auto capacity = 0;
  while (cur_dev) {
    cur_dev = cur_dev->next;
    capacity++;
  }

  std::vector<HIDDeviceInfo> deviceInfos;
  deviceInfos.reserve(capacity);

  cur_dev = devs;
  while (cur_dev) {
    deviceInfos.emplace_back(*cur_dev);
    cur_dev = cur_dev->next;
  }

  return deviceInfos;
}

HIDDeviceInfo::HIDDeviceInfo(hid_device_info &device_info)
    : device_info_(device_info), HIDPath_(device_info_.path),
      DeviceID_{device_info_.vendor_id,
                device_info_.product_id}, // TODO: What is this syntax {...  } ,
                                          // is the {} overriden?
      SerialNumber_(device_info_.serial_number),
      ManufacturerString_(device_info_.manufacturer_string),
      ProductString_(device_info_.product_string) {

  // TODO: It seems like the strings are sometimes null pointers and cannot be
  // freed, how can i assert in the constructor if it has null string and just
  // not allow to construct info.
  // TODO: static assert if device_info_.serial_number is nullpointer
  //       Or just don't create
}

HIDDeviceInfo::~HIDDeviceInfo() {

  // TODO: Crash here, pointer freed was not allocated.... is it because i am
  // using delete instead of free()?
  delete HIDPath_.data();
  delete SerialNumber_.data();
  delete ManufacturerString_.data();
  delete ProductString_.data();

  // // TODO: no matching function for call to 'free' no known conversion from
  // 'const_pointer' (aka 'const wchar_t *') to 'void *' for 1st argument
  // free(const_cast<char*>(HIDPath_.data()));
  // free(const_cast<wchar_t*>(SerialNumber_.data()));
  // free(const_cast<wchar_t*>(ManufacturerString_.data()));
  // free(const_cast<wchar_t*>(ProductString_.data()));

  // TODO: do i also need to free the other members?
  // free(HIDPath_);
  // free(SerialNumber_);
  // free(ManufacturerString_);
  // free(ProductString_);

  // TODO: If move is called, this destructor will be called for object it was
  // moved from. Which is not good... How can i nullify the reference to
  // device_info_, to avoid the destructor from being called.
  // free(device_info_.path);
  // free(device_info_.serial_number);
  // free(device_info_.manufacturer_string);
  // free(device_info_.product_string);
  // free(&device_info_);
}

// move constructor
HIDDeviceInfo::HIDDeviceInfo(HIDDeviceInfo &&other) noexcept
    : device_info_(other.device_info_), HIDPath_(std::move(other.HIDPath_)),
      DeviceID_(std::move(other.DeviceID_)),
      SerialNumber_(std::move(other.SerialNumber_)),
      ManufacturerString_(std::move(other.ManufacturerString_)),
      ProductString_(std::move(other.ProductString_)) {

  // nullify the other string
  other.HIDPath_ = HIDPath("");
  uint16_t trash = 0;
  auto tempDevId = DeviceID{trash, trash};
  other.SerialNumber_ = SerialNumber(L"");
  other.ManufacturerString_ = std::wstring_view(L"");
  other.ProductString_ = std::wstring_view(L"");

  // TODO: should i just call the move operator from the move constructor,
  // because it the same logic. *this = std::move(other); // Calls the move
  // assignment operator
}

HIDDeviceInfo &HIDDeviceInfo::operator=(HIDDeviceInfo &&rhs) noexcept {
  if (this != &rhs) {
    device_info_ = rhs.device_info_;
    HIDPath_ = std::move(rhs.HIDPath_);
    DeviceID_ = std::move(rhs.DeviceID_);
    SerialNumber_ = std::move(rhs.SerialNumber_);
    ManufacturerString_ = std::move(rhs.ManufacturerString_);
    ProductString_ = std::move(rhs.ProductString_);
  }
  return *this;
}

const HIDPath &HIDDeviceInfo::path() { return HIDPath_; }

const DeviceID &HIDDeviceInfo::device_id() { return DeviceID_; }

const SerialNumber &HIDDeviceInfo::serial_number() { return SerialNumber_; }

const ReleaseNumber &HIDDeviceInfo::release_number() {
  // return device_info_.release_number;

  // TODO:  device_info_.release_number is unsigned short and const
  // ReleaseNumber is uint16_t, should i do an static_cast here?
  //        ideally i want to return the number by reference and not copy it.
  // is this correct?
  return static_cast<const ReleaseNumber &>(device_info_.release_number);
}

const std::wstring_view &HIDDeviceInfo::manufacturer_string() {
  return ManufacturerString_;
}

const std::wstring_view &HIDDeviceInfo::product_string() {
  return ProductString_;
}

const UsagePage &HIDDeviceInfo::usage_page() {
  return static_cast<const UsagePage &>(device_info_.usage_page);
}

const Usage &HIDDeviceInfo::usage() {
  return static_cast<const Usage &>(device_info_.usage);
}

const InterfaceNumber &HIDDeviceInfo::interface_number() {
  return static_cast<const InterfaceNumber &>(device_info_.interface_number);
}

const HidBusType &HIDDeviceInfo::bus_type() { return device_info_.bus_type; }

} // namespace hidapi
} // namespace lib
} // namespace ratbag
