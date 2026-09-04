#include "hidapi/device.hpp"

#include "gtest/gtest.h"

#include <memory>
#include <ranges>
#include <algorithm>

#include <codecvt>
#include <cstddef>

class DeviceTestSuit : public ::testing::Test {
protected:
  std::shared_ptr<hidapi::HIDDevice> device_ptr;

  DeviceTestSuit() {
    // TODO: just do a get mock device, and work on that.
    auto deviceInfos = hidapi::HIDDeviceInfo::enumerate_hid_devices();
    device_ptr = std::make_shared<hidapi::HIDDevice>(hidapi::HIDDevice::open(std::move(deviceInfos[0])));
  }
};

TEST_F(DeviceTestSuit, DeviceFailedWrite) {
  hidapi::HIDBuffer buf(64);

  try {
    device_ptr->write(buf);
    FAIL() << "Expected std::runtime_error was not thrown";
  } catch (const std::runtime_error &err) {
    EXPECT_STREQ("Actual number of writen bytes(0) doesn't match the expected(64): Zero buffer/length", err.what());
  } catch (...) {
    FAIL()
        << "Expected std::runtime_error, but a different exception was thrown";
  }
}

TEST_F(DeviceTestSuit, DeviceFailedRead) {
  try {
    auto buf = device_ptr->read(64);
    FAIL() << "Expected std::runtime_error was not thrown";
  } catch (const std::runtime_error &err) {
    EXPECT_STREQ("HID read error: Zero buffer/length", err.what());
  } catch (...) {
    FAIL()
        << "Expected std::runtime_error, but a different exception was thrown";
  }
}

TEST_F(DeviceTestSuit, DeviceReportFailedSend) {
  try {
    hidapi::HIDReport report(hidapi::ReportID{0x77}, std::size_t{0});
    device_ptr->send_feature_report(report);
    FAIL() << "Expected std::runtime_error was not thrown";
  } catch (const std::runtime_error &err) {
    EXPECT_STREQ("Actual number of writen bytes(-1) doesn't match the expected(1): Zero buffer/length", err.what());
  } catch (...) {
    FAIL()
        << "Expected std::runtime_error, but a different exception was thrown";
  }
}

TEST_F(DeviceTestSuit, DeviceReportRead) {

  try {
  auto report_received =
      device_ptr->receive_feature_report(hidapi::ReportID{0x77}, std::size_t{16});

    FAIL() << "Expected std::runtime_error was not thrown";
  } catch (const std::runtime_error &err) {
    EXPECT_STREQ("HIDReport receive error: Zero buffer/length", err.what());
  } catch (...) {
    FAIL()
        << "Expected std::runtime_error, but a different exception was thrown";
  }
}
