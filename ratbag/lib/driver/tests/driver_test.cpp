#include "ratbag/lib/hidapi/device_info.hpp"
#include "ratbag/lib/driver/driver.hpp"

#include <codecvt>
#include <variant>

#include "gtest/gtest.h"

using ratbag::lib::hidapi::HIDDeviceInfo;
using ratbag::lib::driver::HIDPP20;
using ratbag::lib::driver::SteelSeries;

TEST(DriverTestSuit, OpenDriverHiddpp20) {
  auto deviceInfos = HIDDeviceInfo::enumerate_hid_devices();

  auto drv_result = ratbag::lib::driver::open(deviceInfos[0]);
  EXPECT_TRUE(drv_result.has_value());

  auto drv = drv_result.value();
  ASSERT_TRUE(std::holds_alternative<HIDPP20>(drv));
}

TEST(DriverTestSuit, OpenDriverSteelseries) {
  auto deviceInfos = HIDDeviceInfo::enumerate_hid_devices();

  auto drv_result = ratbag::lib::driver::open(deviceInfos[1]);
  EXPECT_TRUE(drv_result.has_value());

  auto drv = drv_result.value();
  ASSERT_TRUE(std::holds_alternative<SteelSeries>(drv));
}
