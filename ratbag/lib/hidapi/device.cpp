#include "ratbag/lib/hidapi/device.hpp"

#include <codecvt>
#include <format>
#include <optional>
#include <ostream>
#include <string_view>

namespace ratbag {
namespace lib {
namespace hidapi {

void HIDDevice::write(HIDBuffer buf) {
  auto buf_ptr = reinterpret_cast<unsigned char*>(buf.data());

  auto res_length = hid_write(handle_, buf_ptr, buf.size());

  if (res_length != buf.size()) {
    throw std::runtime_error(std::format(
        "Actual number of writen bytes({}) doesn't match the expected({})",
        res_length, buf.size()));
  }
}

std::optional<HIDBuffer> HIDDevice::read(size_t length) {
  HIDBuffer buf(length);

  auto buf_ptr = reinterpret_cast<unsigned char*>(buf.data());
  // Read requested state
  auto res_length = hid_read(handle_, buf_ptr, buf.size());

  // the read is non-blocking and there was nothing to read from the hid device
  if (res_length == 0) {
    return {};
  }

  if (res_length == -1) {
    HIDAPIString err(hid_error(handle_));

    throw std::runtime_error(std::format("HID read error: {}", err));
  }

  if (res_length != length) {
    throw std::runtime_error(std::format(
        "Received response length({}) doesn't match the expected({})",
        res_length, length));
  }

  return buf;
}

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
  }

  return *this;
}


} // namespace hidapi
} // namespace lib
} // namespace ratbag
