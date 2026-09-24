#include "hidapi/device.hpp"

#include <cassert>
#include <codecvt>
#include <cstdint>
#include <exception>
#include <format>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <string_view>
#include <utility>
#include <vector>

namespace hidapi {

HIDDevice::HIDDevice(hid_device *handle, HIDDeviceInfo device_info)
    : handle_(handle), device_info_(std::move(device_info)) {}

HIDDevice::~HIDDevice() {
  if (isValid()) {
    hid_close(handle_);
  }
}

// move constructor
HIDDevice::HIDDevice(HIDDevice &&other) noexcept
    : handle_(std::exchange(other.handle_, nullptr)),
      device_info_(std::move(other.device_info_)) {}

// move operator
HIDDevice &HIDDevice::operator=(HIDDevice &&rhs) noexcept {
  if (this == &rhs) {
    return *this;
  }

  // handle case, where the object was moved
  if (isValid()) {
    hid_close(handle_);
  }

  this->handle_ = std::exchange(rhs.handle_, nullptr);
  this->device_info_ = std::move(rhs.device_info_);

  return *this;
}

bool HIDDevice::isValid() const { return handle_ != nullptr; }

HIDDeviceInfo &HIDDevice::deviceInfo() {
  assert("called on moved-from object" && isValid());
  return device_info_;
}

std::optional<HIDBuffer> HIDDevice::read(std::size_t max_length,
                                         ReadTimeoutMilli timeout) {
  assert("called on moved-from object" && isValid());

  HIDBuffer buf(max_length);

  auto buf_ptr = reinterpret_cast<unsigned char *>(buf.data());

  // Read requested state
  auto bytes_read = hid_read_timeout(handle_, buf_ptr, buf.size(),
                                     static_cast<int>(timeout.count()));

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

void HIDDevice::write(HIDBuffer buf) {
  assert("called on moved-from object" && isValid());

  auto buf_ptr = reinterpret_cast<unsigned char *>(buf.data());
  auto bytes_written = hid_write(handle_, buf_ptr, buf.size());

  if (bytes_written == buf.size()) {
    return;
  }

  HIDAPIString err(hid_error(handle_));
  throw std::runtime_error(std::format("Actual number of writen bytes({}) "
                                       "doesn't match the expected({}): {}",
                                       bytes_written, buf.size(), err));
}

// TODO: should i reduce the error count with -ferror-limit=1
std::optional<HIDReport> HIDDevice::receive_feature_report(ReportID report_id,
                                                           std::size_t length) {
  assert("called on moved-from object" && isValid());

  HIDReport report(report_id, length);

  auto buf_ptr = reinterpret_cast<unsigned char *>(report.data());
  auto bytes_read = hid_get_feature_report(handle_, buf_ptr, report.size());

  if (bytes_read == -1) {
    HIDAPIString err(hid_error(handle_));
    throw std::runtime_error(std::format("HIDReport receive error: {}", err));
  }

  return report;
}

void HIDDevice::send_feature_report(HIDReport report) {
  assert("called on moved-from object" && isValid());

  auto buf_ptr = reinterpret_cast<unsigned char *>(report.data());
  auto bytes_written = hid_send_feature_report(handle_, buf_ptr, report.size());
  if (bytes_written == report.size()) {
    return;
  }

  HIDAPIString err(hid_error(handle_));
  throw std::runtime_error(std::format("Actual number of writen bytes({}) "
                                       "doesn't match the expected({}): {}",
                                       bytes_written, report.size(), err));
}

} // namespace hidapi
