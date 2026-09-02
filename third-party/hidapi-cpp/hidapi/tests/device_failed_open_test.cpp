#include "hidapi/device.hpp"

#include "gtest/gtest.h"

TEST(DeviceTestSuit, FailedToOpen) {
  auto deviceInfos = hidapi::HIDDeviceInfo::enumerate_hid_devices();
  try {
    auto device = hidapi::HIDDevice::open(std::move(deviceInfos[0]));
    FAIL() << "Expected std::runtime_error was not thrown";
  } catch (const std::runtime_error &err) {
    EXPECT_STREQ("ioctl(GRDESCSIZE) error for", err.what());
  } catch (...) {
    FAIL()
        << "Expected std::runtime_error, but a different exception was thrown";
  }
}
