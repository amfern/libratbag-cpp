#include "ratbag/lib/hidapi/device_info.hpp"

#include <codecvt>
#include <format>
#include <ostream>
#include <string_view>

namespace ratbag {
namespace lib {
namespace hidapi {

DeviceID::DeviceID(ProductID vid, VendorID pid) : vid_(vid), pid_(pid) {}

ProductID DeviceID::pid() const { return pid_; }

VendorID DeviceID::vid() const { return vid_; }

std::ostream &operator<<(std::ostream &os, const DeviceID &di) {
  os << std::format("{}", di);
  return os;
}

std::string HIDAPIString::toString() const {
  std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
  return converter.to_bytes(data());
};

std::ostream &operator<<(std::ostream &os, const HIDAPIString &s) {
  os << std::format("{}", s);
  return os;
}

const HIDDeviceInfoList HIDDeviceInfo::enumerate_hid_devices() {
  struct hid_device_info *cur_dev;
  cur_dev = hid_enumerate(0, 0); // 0,0 = find all devices

  HIDDeviceInfoList deviceInfos;

  while (cur_dev) {
    deviceInfos.emplace_back(HIDDeviceInfo(cur_dev));
    cur_dev = cur_dev->next;
  }

  return deviceInfos;
}

// What is this syntax {...  }, is the {} overriden? NO,
// 1. if no constructor in the class it's an aggregate initialization, the class
// memeber initilzation
// 2. If constrctuion has std::initializer_list<T>, the the list in the curly
// braces will be elements.
// 3. MyClass{} will call constructor as if ()
// 4. {} It's a C++ things, uniform initialization feature.
HIDDeviceInfo::HIDDeviceInfo(hid_device_info *device_info)
    : device_info_(device_info), hid_path_(device_info_->path),
      device_id_(DeviceID{device_info_->vendor_id, device_info_->product_id}),
      serial_number_(device_info_->serial_number),
      manufacturer_string_(device_info_->manufacturer_string),
      product_string_(device_info_->product_string) {}

HIDDeviceInfo::~HIDDeviceInfo() {
  if (device_info_) {
    // We remove the next element to prevent from the hid_free_enumeration
    // deleting them.
    // ideally we should modify the hidapi librarby and add an single delete
    // function to the upper hidapi library.
    device_info_->next = nullptr;
    hid_free_enumeration(device_info_);
  }
}

// move constructor
HIDDeviceInfo::HIDDeviceInfo(HIDDeviceInfo &&other) noexcept
    : device_info_(other.device_info_), hid_path_(other.hid_path_),
      device_id_(std::move(other.device_id_)),
      serial_number_(std::move(other.serial_number_)),
      manufacturer_string_(std::move(other.manufacturer_string_)),
      product_string_(std::move(other.product_string_)) {

  other.device_info_ = nullptr;

  other.device_id_ = DeviceID{0, 0};
  other.hid_path_ = HIDPath{};
  other.serial_number_ = HIDAPIString{};
  other.manufacturer_string_ = HIDAPIString{};
  other.product_string_ = HIDAPIString{};
}

// move operator
HIDDeviceInfo &HIDDeviceInfo::operator=(HIDDeviceInfo &&rhs) noexcept {
  if (this != &rhs) {

    // handle case, where the object was moved
    // In rust the moved from object cannot be used, it's a compiler error
    // In C++ we add this check
    // Why we don't implement similar behavior in C++?
    if (device_info_ != nullptr) {
      device_info_->next = nullptr;
      hid_free_enumeration(device_info_);
    }

    this->device_info_ = rhs.device_info_;
    rhs.device_info_ = nullptr;

    this->hid_path_ = rhs.hid_path_;
    this->device_id_ = std::move(rhs.device_id_);
    this->serial_number_ = std::move(rhs.serial_number_);
    this->manufacturer_string_ = std::move(rhs.manufacturer_string_);
    this->product_string_ = std::move(rhs.product_string_);

    // should i also nullify the string views? it will add more overhead
    // but we will prevent leaky abstraction? rhs.hid_path_ = std::string_view{};
    // Better to start with the conceptually correct thing, and then make it
    // fast and correct. The overheard is neglegable

    rhs.device_id_ = DeviceID{0, 0};
    rhs.hid_path_ = HIDPath{};
    rhs.serial_number_ = HIDAPIString{};
    rhs.manufacturer_string_ = HIDAPIString{};
    rhs.product_string_ = HIDAPIString{};
  }

  return *this;
}

HIDPath HIDDeviceInfo::path() const { return hid_path_; }

DeviceID HIDDeviceInfo::device_id() const { return device_id_; }

HIDAPIString HIDDeviceInfo::serial_number() const { return serial_number_; }

ReleaseNumber HIDDeviceInfo::release_number() const {
  return static_cast<const ReleaseNumber &>(device_info_->release_number);
}

HIDAPIString HIDDeviceInfo::manufacturer_string() const {
  return manufacturer_string_;
}

HIDAPIString HIDDeviceInfo::product_string() const { return product_string_; }

UsagePage HIDDeviceInfo::usage_page() const {
  return static_cast<const UsagePage &>(device_info_->usage_page);
}

Usage HIDDeviceInfo::usage() const {
  return static_cast<const Usage &>(device_info_->usage);
}

InterfaceNumber HIDDeviceInfo::interface_number() const {
  return static_cast<const InterfaceNumber &>(device_info_->interface_number);
}

hid_bus_type HIDDeviceInfo::bus_type() const { return device_info_->bus_type; }

std::ostream &operator<<(std::ostream &os, const HIDDeviceInfo &info) {
  os << std::format("{}", info);
  return os;
}

} // namespace hidapi
} // namespace lib
} // namespace ratbag
