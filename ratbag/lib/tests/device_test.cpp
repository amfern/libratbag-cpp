#include "ratbag/lib/hidapi/device_info.hpp"
#include "ratbag/lib/device.hpp"

#include <codecvt>
#include <variant>

#include "gtest/gtest.h"

using ratbag::lib::Device;
using ratbag::lib::hidapi::HIDDeviceInfo;

TEST(DriverTestSuit, OpenDummyDevice) {
  // TODO: maybe create a dummy test driver and test device here, maybe use gMock to create a mock driver and mock device
  auto deviceInfos = HIDDeviceInfo::enumerate_hid_devices();

  auto dev_result = Device::open(deviceInfos.front());
  EXPECT_TRUE(dev_result.has_value());

  auto dev = dev_result.value();
  auto& profiles = dev.profiles();

  EXPECT_EQ(profiles.size(), 1u);
  // auto& prfl = profiles.front();

  // prfl.led();
}
