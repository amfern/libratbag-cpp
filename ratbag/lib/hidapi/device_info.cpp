#include "ratbag/lib/hidapi/device_info.hpp"

#include <format>
#include <ostream>
#include <string_view>
#include <codecvt>

// TODO: i tried to move the implemnetation into cpp file, but i ran into vague
// compiler errors, and after testing it seems the wide stgring type is what
// causing the errors auto fmt::formatter<color>::format(color c,
// format_context& ctx) const
//     -> format_context::iterator {

// template <typename CharT>
// auto std::formatter<ratbag::lib::hidapi::DeviceID, CharTo>::format(
//     const ratbag::lib::hidapi::DeviceID &id, auto &ctx) const {

//   if constexpr (std::is_same_v<CharT, char>) {
//     return std::format_to(ctx.out(), "DeviceID(vid: {:#06x}, pid: {:#06x})",
//                           id.vid(), id.pid());
//   } else if constexpr (std::is_same_v<CharT, wchar_t>) {
//     return std::format_to(ctx.out(), L"DeviceID(vid: {:#06x}, pid: {:#06x})",
//                           id.vid(), id.pid());
//   }
// }

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
      DeviceID_(DeviceID{device_info_->vendor_id, device_info_->product_id}) {

  // TODO: it is very in-convenient to work with wchar_t...
  //       I have to convert path to wstring, and because of hidapi decision
  //       to use wchar, it forces forces my code to use wide chars every
  //       where. also it make this situation impossible, and i can't print
  //       into std::cout... It's better to fix the issue at the root, and
  //       prevent the hidapi library from convert utf8 to wchar_t with. eg:
  //       https://github.com/libusb/hidapi/blob/a7587175f35ddcbdfb1ce5a1e4dd7a268699ba7f/linux/hid.c#L736
  //       And let the c++ pick the correct char encoding fo the platform.
  //       It will make the library abit more cross platform.
  //
  //       Prior to C++26, the recommended way to do UTF-8 conversions is still codecvt_utf8  https://en.cppreference.com/w/cpp/locale/codecvt_utf8.html even though this is deprecated.
  //       we don't reallt plan to have alot of windows users
  //       the utf converation will not be in an performance intensive process, so it's okay to not have it
  //       for linux hidapi.c converts utf8 to wchar, this is stupid, we shouldn't be converting native OS strings, it should be fixed on the hidapi level.
  //       Use the converter to from_bytes (wchar to UTF-8 string)
  //       This is not perfect, because we essentially allocate memory
  std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
  SerialNumber_ = converter.to_bytes(device_info_->serial_number);
  ManufacturerString_ = converter.to_bytes(device_info_->manufacturer_string);
  ProductString_ = converter.to_bytes(device_info_->product_string);
}

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
      DeviceID_(std::move(other.DeviceID_)), SerialNumber_(std::move(other.SerialNumber_)),
      ManufacturerString_(std::move(other.ManufacturerString_)),
      ProductString_(std::move(other.ProductString_)) {

  other.device_info_ = nullptr;

  other.DeviceID_ = DeviceID{0, 0};
  other.HIDPath_ = HIDPath{};
  other.SerialNumber_ = SerialNumber{};
  other.ManufacturerString_ = std::string_view{};
  other.ProductString_ = std::string_view{};
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
    rhs.SerialNumber_ = SerialNumber{};
    rhs.ManufacturerString_ = std::string_view{};
    rhs.ProductString_ = std::string_view{};
  }

  return *this;
}

HIDPath HIDDeviceInfo::path() const { return HIDPath_; }

DeviceID HIDDeviceInfo::device_id() const { return DeviceID_; }

SerialNumber HIDDeviceInfo::serial_number() const { return SerialNumber_; }

ReleaseNumber HIDDeviceInfo::release_number() const {
  return static_cast<const ReleaseNumber &>(device_info_->release_number);
}

std::string_view HIDDeviceInfo::manufacturer_string() const {
  return ManufacturerString_;
}

std::string_view HIDDeviceInfo::product_string() const {
  // From what i understand from the hidapi library, the wide char is only used
  // for windows variant. Should i create like an template that for linux retuns
  // std::string, and for windows retuns std::Would it be better

  // ANS: it's better to not worry about, becuase UI framewoks will be doing
  // convertions anyway. So we shouldn't converyt to std::string to avoid the
  // double allocations
  return ProductString_;
}

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

// Define the overloaded operator outside the class
