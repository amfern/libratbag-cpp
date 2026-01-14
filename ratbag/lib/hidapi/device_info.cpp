#include "ratbag/lib/hidapi/device_info.hpp"

namespace ratbag {
namespace lib {
namespace hidapi {

// TODO: should i return here a vector or be more vague/abstract. and return an
// iterator....?
const std::vector<HIDDeviceInfo> HIDDeviceInfo::enumerate_hid_devices() {
  struct hid_device_info *devs, *cur_dev;
  devs = hid_enumerate(0, 0); // 0,0 = find all devices

  std::vector<HIDDeviceInfo> deviceInfos;

  cur_dev = devs;
  while (cur_dev) {
    deviceInfos.emplace_back(HIDDeviceInfo(cur_dev));
    cur_dev = cur_dev->next;
  }

  return deviceInfos;
}

// TODO: What is this syntax {...  }, is the {} overriden? NO,
// 1. if no constructor in the class it's an aggregate initialization, the class
// memeber initilzation
// 2. If constrctuion has std::initializer_list<T>, the the list in the curly
// braces will be elements.
// 3. MyClass{} will call constructor as if ()
// 4. {} It's a C++ things, uniform initialization feature.
HIDDeviceInfo::HIDDeviceInfo(hid_device_info *device_info)
    : device_info_(device_info), HIDPath_(device_info_->path),
      DeviceID_(DeviceID{device_info_->vendor_id, device_info_->product_id}),
      SerialNumber_(device_info_->serial_number),
      ManufacturerString_(device_info_->manufacturer_string),
      ProductString_(device_info_->product_string) {}

HIDDeviceInfo::~HIDDeviceInfo() {
  if (device_info_) {
    // TODO: we remove the next element to prevent from the hid_free_enumeration
    // deleting them.
    //       ideally we should add an single delete function to the upper hidapi
    //       library.
    device_info_->next = nullptr;
    hid_free_enumeration(device_info_);
  }
}

// move constructor
HIDDeviceInfo::HIDDeviceInfo(HIDDeviceInfo &&other) noexcept
    : device_info_(other.device_info_), HIDPath_(std::move(other.HIDPath_)),
      DeviceID_(std::move(other.DeviceID_)),
      SerialNumber_(std::move(other.SerialNumber_)),
      ManufacturerString_(std::move(other.ManufacturerString_)),
      ProductString_(std::move(other.ProductString_)) {

  other.device_info_ = nullptr;
  // TODO: we can set the other.HIDPath_ = string_view{} to reset the other
  // device, however it will add overhead. But we can leave the other object in
  // and valid but undefined state
}

HIDDeviceInfo &HIDDeviceInfo::operator=(HIDDeviceInfo &&rhs) noexcept {
  if (this != &rhs) {
    device_info_->next = nullptr;
    hid_free_enumeration(device_info_);

    device_info_ = rhs.device_info_;
    rhs.device_info_ = nullptr;
    HIDPath_ = std::move(rhs.HIDPath_);
    DeviceID_ = std::move(rhs.DeviceID_);
    SerialNumber_ = std::move(rhs.SerialNumber_);
    ManufacturerString_ = std::move(rhs.ManufacturerString_);
    ProductString_ = std::move(rhs.ProductString_);

    // TODO: should i also nullify the string views? it will add more overhead
    // but we will prevent leaky abstraction? rhs.HIDPath_ = std::string_view{};
    // rhs.DeviceID_ = std::string_view{};
    // rhs.SerialNumber_ = std::string_view{};
    // rhs.ManufacturerString_ = std::string_view{};
    // rhs.ProductString_ = std::string_view{};
  }

  return *this;
}

const HIDPath &HIDDeviceInfo::path() const { return HIDPath_; }

const DeviceID &HIDDeviceInfo::device_id() const { return DeviceID_; }

const SerialNumber &HIDDeviceInfo::serial_number() const {
  return SerialNumber_;
}

const ReleaseNumber &HIDDeviceInfo::release_number() const {
  // return device_info_.release_number;

  // TODO:  device_info_.release_number is unsigned short and const
  // ReleaseNumber is uint16_t, should i do an static_cast here?
  //        ideally i want to return the number by reference and not copy it.
  // is this correct?
  return static_cast<const ReleaseNumber &>(device_info_->release_number);
}

const std::wstring_view &HIDDeviceInfo::manufacturer_string() const {
  return ManufacturerString_;
}

const std::wstring_view &HIDDeviceInfo::product_string() const {
  return ProductString_;
}

// TODO: this are really simple function.... should i force them inline? or
const UsagePage &HIDDeviceInfo::usage_page() const {
  return static_cast<const UsagePage &>(device_info_->usage_page);
}

const Usage &HIDDeviceInfo::usage() const {
  return static_cast<const Usage &>(device_info_->usage);
}

const InterfaceNumber &HIDDeviceInfo::interface_number() const {
  return static_cast<const InterfaceNumber &>(device_info_->interface_number);
}

const HidBusType &HIDDeviceInfo::bus_type() const {
  return device_info_->bus_type;
}

} // namespace hidapi
} // namespace lib
} // namespace ratbag
