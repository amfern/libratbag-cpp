#include "hidapi/device.hpp"

#include <codecvt>
#include <cstdint>
#include <format>
#include <optional>
#include <ostream>
#include <string_view>
#include <vector>

namespace hidapi {

void HIDDevice::write(HIDBuffer buf) {
  auto buf_ptr = reinterpret_cast<unsigned char*>(buf.data());

  auto bytes_written = hid_write(handle_, buf_ptr, buf.size());

  if (bytes_written != buf.size()) {
    throw std::runtime_error(std::format(
        "Actual number of writen bytes({}) doesn't match the expected({})",
        bytes_written, buf.size()));
  }
}

std::optional<HIDBuffer> HIDDevice::read(std::size_t max_length, ReadTimeoutMilli timeout) {
  HIDBuffer buf(max_length);

  auto buf_ptr = reinterpret_cast<unsigned char*>(buf.data());
 
  // Read requested state
  auto bytes_read = hid_read_timeout(handle_, buf_ptr, buf.size(), static_cast<int>(timeout.count()));

  // the read is non-blocking and there was nothing to read from the hid device
  if (bytes_read == 0) {
    return {};
  }

  if (bytes_read == -1) {
    HIDAPIString err(hid_error(handle_));
    throw std::runtime_error(std::format("HID read error: {}", err));
  }

  return buf;
}

void HIDDevice::send_feature_report(HIDReport report) {
  auto buf_ptr = reinterpret_cast<unsigned char*>(report.data());
  auto bytes_written = hid_send_feature_report(handle_, buf_ptr, report.size());
  if (bytes_written != report.size()) {
    throw std::runtime_error(std::format(
        "Actual number of writen bytes({}) doesn't match the expected({})",
                                         bytes_written, report.size()));
  }
}

std::optional<HIDReport> HIDDevice::receive_feature_report(ReportID report_id, std::size_t length) {

  HIDReport report(report_id, length);

  auto buf_ptr = reinterpret_cast<unsigned char*>(report.data());
  auto bytes_read = hid_get_feature_report(handle_, buf_ptr, report.size());

  if (bytes_read == -1) {
    HIDAPIString err(hid_error(handle_));
    throw std::runtime_error(std::format("HID read error: {}", err));
  }

  return report;
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
