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

  auto bytes_written = hid_write(handle_, buf_ptr, buf.size());

  if (bytes_written != buf.size()) {
    throw std::runtime_error(std::format(
        "Actual number of writen bytes({}) doesn't match the expected({})",
        bytes_written, buf.size()));
  }
}

// TODO(ask): what do you think about magic values that have meaning? or should i use something like std::variant instead. Or just a simple bool
//  timeout can be either
//  ReadTimeoutBlock - Block return
//  ReadTimeoutNone - read and return immididatly
//  ReadTimeoutMilli{<value>} - timeout to wait
std::optional<HIDBuffer> HIDDevice::read(std::size_t length, ReadTimeoutMilli timeout) {
  HIDBuffer buf(length);

  auto buf_ptr = reinterpret_cast<unsigned char*>(buf.data());
  // TODO(ask): what do you thin about t
  auto hid_timeout = timeout == ReadTimeoutBlock ? -1 : static_cast<int>(timeout.count());
 
  // Read requested state
  auto bytes_read = hid_read_timeout(handle_, buf_ptr, buf.size(), hid_timeout);

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
  auto buf_ptr = reinterpret_cast<unsigned char*>(report.data_.data());
  auto bytes_written = hid_send_feature_report(handle_, buf_ptr, report.data.size());
  if (bytes_written != report.data.size()) {
    throw std::runtime_error(std::format(
        "Actual number of writen bytes({}) doesn't match the expected({})",
        bytes_written, report.data.size()));
  }
}

std::optional<HIDReport> HIDDevice::receive_feature_report(ReportID report_id, std::size_t length) {
  HIDReport report(report_id, length);

  auto buf_ptr = reinterpret_cast<unsigned char*>(report.data_.data());
  auto bytes_read = hid_get_feature_report(handle_, buf_ptr, report.data_.size());

  if (bytes_read == -1) {
    HIDAPIString err(hid_error(handle_));
    throw std::runtime_error(std::format("HID read error: {}", err));
  }

  report.data = std::span<std::byte>{report.data_}.subspan(1, bytes_read - 1);

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
} // namespace lib
} // namespace ratbag
