#include "ratbag/lib/hidapi/device.hpp"

#include <codecvt>
#include <format>
#include <ostream>
#include <string_view>

namespace ratbag {
namespace lib {
namespace hidapi {

HIDDevice::HIDDevice(hid_device* handle, HIDDeviceInfo device_info)
    : handle_(handle), device_info_(std::move(device_info)) {}

HIDDevice::~HIDDevice() {
  if (handle_) {
    hid_close(handle_);
  }
}

// move constructor
HIDDevice::HIDDevice(HIDDevice &&other) noexcept
    : handle_(other.handle_),
      device_info_(std::move(other.device_info_)) {

  other.handle_ = nullptr;
}

// move operator
HIDDevice &HIDDevice::operator=(HIDDevice &&rhs) noexcept {
  if (this != &rhs) {

    // handle case, where the object was moved
    // In rust the moved from object cannot be used, it's a compiler error
    // In C++ we add this check
    // Why we don't implement similar behavior in C++?
    if (handle_ != nullptr) {
      hid_close(handle_);
    }

    this->handle_ = rhs.handle_;
    rhs.handle_ = nullptr;

    this->device_info_ = std::move(rhs.device_info_);

    // should i also nullify the string views? it will add more overhead
    // but we will prevent leaky abstraction? rhs.HIDPath_ = std::string_view{};
    // Better to start with the conceptually correct thing, and then make it
    // fast and correct. The overheard is neglegable

    // rhs.DeviceID_ = DeviceID{0, 0};
    // rhs.HIDPath_ = HIDPath{};
    // rhs.SerialNumber_ = HIDAPIString{};
    // rhs.ManufacturerString_ = HIDAPIString{};
    // rhs.ProductString_ = HIDAPIString{};
  }

  return *this;
}


} // namespace hidapi
} // namespace lib
} // namespace ratbag
