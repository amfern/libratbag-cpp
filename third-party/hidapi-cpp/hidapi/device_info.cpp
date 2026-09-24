#include "hidapi/device_info.hpp"

#include <codecvt>
#include <format>
#include <ostream>
#include <stdexcept>
#include <string_view>

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

  // TODO: it will throw error if failed to open udev, or there is just no hid
  // devices in the system. that is not ideal, as not having any hid devices in
  // the system is totally acceptable and shouldn't be treated as an error.
  if (cur_dev == nullptr) {
    HIDAPIString err(hid_error(nullptr));
    throw std::runtime_error(
        std::format("Failed to enumarate HID devices: {}", err));
  }

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
  if (isValid()) {
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
    : device_info_(std::exchange(other.device_info_, nullptr)),
      hid_path_(std::exchange(other.hid_path_, HIDPath{})),
      device_id_(std::exchange(other.device_id_, DeviceID{0, 0})),
      serial_number_(std::exchange(other.serial_number_, HIDAPIString{})),
      manufacturer_string_(
          std::exchange(other.manufacturer_string_, HIDAPIString{})),
      product_string_(std::exchange(other.product_string_, HIDAPIString{})) {}

// move operator
HIDDeviceInfo &HIDDeviceInfo::operator=(HIDDeviceInfo &&rhs) noexcept {
  if (this == &rhs) {
    return *this;
  }

  if (isValid()) {
    device_info_->next = nullptr;
    hid_free_enumeration(device_info_);
  }

  this->device_info_ = std::exchange(rhs.device_info_, nullptr);
  this->hid_path_ = std::exchange(rhs.hid_path_, HIDPath{});
  this->device_id_ = std::exchange(rhs.device_id_, DeviceID{0, 0});
  this->serial_number_ = std::exchange(rhs.serial_number_, HIDAPIString{});
  this->manufacturer_string_ =
      std::exchange(rhs.manufacturer_string_, HIDAPIString{});
  this->product_string_ = std::exchange(rhs.product_string_, HIDAPIString{});

  return *this;
}

bool HIDDeviceInfo::isValid() const { return device_info_ != nullptr; }

HIDPath HIDDeviceInfo::path() const {
  // clang-tidy can detect use-after-move, but still should check if the object
  // is valid, because some cases clang-tidy won't be able to catch.
  // https://clang.llvm.org/extra/clang-tidy/checks/bugprone/use-after-move.html
  // the additional pointer check is tiny, and dwarfed by other things around it
  // https://docs.google.com/document/d/1c3iuOSepMLLYmcd4oeSsmUanQnmULeBsdeVviiSYvOo/edit?usp=sharing
  assert("called on moved-from object" && isValid());
  return hid_path_;
}

DeviceID HIDDeviceInfo::device_id() const {
  assert("called on moved-from object" && isValid());
  return device_id_;
}

HIDAPIString HIDDeviceInfo::serial_number() const {
  assert("called on moved-from object" && isValid());
  return serial_number_;
}

ReleaseNumber HIDDeviceInfo::release_number() const {
  assert("called on moved-from object" && isValid());
  return device_info_->release_number;
}

HIDAPIString HIDDeviceInfo::manufacturer_string() const {
  assert("called on moved-from object" && isValid());
  return manufacturer_string_;
}

HIDAPIString HIDDeviceInfo::product_string() const {
  assert("called on moved-from object" && isValid());
  return product_string_;
}

UsagePage HIDDeviceInfo::usage_page() const {
  assert("called on moved-from object" && isValid());
  return device_info_->usage_page;
}

Usage HIDDeviceInfo::usage() const {
  assert("called on moved-from object" && isValid());
  return device_info_->usage;
}

InterfaceNumber HIDDeviceInfo::interface_number() const {
  assert("called on moved-from object" && isValid());
  return device_info_->interface_number;
}

HidBusType HIDDeviceInfo::bus_type() const {
  assert("called on moved-from object" && isValid());
  return static_cast<HidBusType>(device_info_->bus_type);
}

std::ostream &operator<<(std::ostream &os, const HIDDeviceInfo &info) {
  os << std::format("{}", info);
  return os;
}

} // namespace hidapi
