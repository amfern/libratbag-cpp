#include "ratbag/lib/hidapi/device_info.hpp"

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
  auto [vid, pid] = di;

  os << std::format("vid {:x}\npid {:x}", vid, pid);

  return os;
}

const HIDDeviceInfoList HIDDeviceInfo::enumerate_hid_devices() {
  struct hid_device_info *devs, *cur_dev;
  devs = hid_enumerate(0, 0); // 0,0 = find all devices

  std::vector<HIDDeviceInfo> deviceInfos;

  cur_dev = devs;
  while (cur_dev) {
    deviceInfos.emplace_back(HIDDeviceInfo(*cur_dev));
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
HIDDeviceInfo::HIDDeviceInfo(hid_device_info &device_info)
    : device_info_(&device_info), HIDPath_(device_info_->path),
      DeviceID_(DeviceID{device_info_->vendor_id, device_info_->product_id}),
      SerialNumber_(device_info_->serial_number),
      ManufacturerString_(device_info_->manufacturer_string),
      ProductString_(device_info_->product_string) {}

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
    : device_info_(other.device_info_), HIDPath_(other.HIDPath_),
      DeviceID_(std::move(other.DeviceID_)),
      SerialNumber_(std::move(other.SerialNumber_)),
      ManufacturerString_(std::move(other.ManufacturerString_)),
      ProductString_(std::move(other.ProductString_)) {

  other.device_info_ = nullptr;

  other.DeviceID_ = DeviceID{0, 0};
  other.HIDPath_ = HIDPath{};
  other.SerialNumber_ = HIDAPIString{};
  other.ManufacturerString_ = HIDAPIString{};
  other.ProductString_ = HIDAPIString{};
}

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

    this->HIDPath_ = rhs.HIDPath_;
    this->DeviceID_ = std::move(rhs.DeviceID_);
    this->SerialNumber_ = std::move(rhs.SerialNumber_);
    this->ManufacturerString_ = std::move(rhs.ManufacturerString_);
    this->ProductString_ = std::move(rhs.ProductString_);

    // should i also nullify the string views? it will add more overhead
    // but we will prevent leaky abstraction? rhs.HIDPath_ = std::string_view{};
    // Better to start with the conceptually correct thing, and then make it
    // fast and correct. The overheard is neglegable

    rhs.DeviceID_ = DeviceID{0, 0};
    rhs.HIDPath_ = HIDPath{};
    rhs.SerialNumber_ = HIDAPIString{};
    rhs.ManufacturerString_ = HIDAPIString{};
    rhs.ProductString_ = HIDAPIString{};
  }

  return *this;
}

HIDPath HIDDeviceInfo::path() const { return HIDPath_; }

DeviceID HIDDeviceInfo::device_id() const { return DeviceID_; }

HIDAPIString HIDDeviceInfo::serial_number() const { return SerialNumber_; }

ReleaseNumber HIDDeviceInfo::release_number() const {
  return static_cast<const ReleaseNumber &>(device_info_->release_number);
}

HIDAPIString HIDDeviceInfo::manufacturer_string() const {
  return ManufacturerString_;
}

HIDAPIString HIDDeviceInfo::product_string() const { return ProductString_; }

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


namespace std {

template <class FormatContext>
typename FormatContext::iterator
formatter<DeviceID>::format(const DeviceID &id, FormatContext &ctx) const {
  return format_to(ctx.out(), "DeviceID(vid: {:#06x}, pid: {:#06x})", id.vid(),
                   id.pid());
}

template <class FormatContext>
typename FormatContext::iterator
formatter<HIDAPIString>::format(const HIDAPIString &hidapi_string,
                                FormatContext &ctx) const {
  wstring_convert<codecvt_utf8<wchar_t>> converter;
  string string_utf8 = converter.to_bytes(hidapi_string.data());

  return format_to(ctx.out(), "{}", string_utf8);
}

template <class FormatContext>
typename FormatContext::iterator
formatter<HIDDeviceInfo>::format(const HIDDeviceInfo &info,
                                 FormatContext &ctx) const {
  return format_to(ctx.out(),
                   "HIDDeviceInfo(path: {}, deviceId: {},serial_number: {}, "
                   "manufacturer_string: {}, product_string = {}, usage_page "
                   "= {}, usage = {}, interface_number = {}, bus_type = {})",
                   info.path(), info.device_id(), info.serial_number(),
                   info.manufacturer_string(), info.product_string(),
                   info.usage_page(), info.usage(), info.interface_number(),
                   info.bus_type());
}
} // namespace std
