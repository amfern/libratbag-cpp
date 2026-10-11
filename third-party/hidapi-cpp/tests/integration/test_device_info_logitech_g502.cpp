#include "hidapi/device.hpp"

#include "gtest/gtest.h"
// #include "gmock/gmock.h"

#include <algorithm>
#include <codecvt>
#include <regex>

using hidapi::DeviceID;
using hidapi::HidBusType;
using hidapi::HIDDevice;
using hidapi::HIDDeviceInfo;

// using ::testing::MatchesRegex;

// TODO: Run the test when the device is avaiable

// call the underlying API and compare the two...will work on all PC's
TEST(DeviceTestSuit, OpenDevice) {
  auto deviceInfos = HIDDeviceInfo::enumerate_hid_devices();

  auto it = std::ranges::find(deviceInfos, DeviceID{0x046d, 0xc332}, &HIDDeviceInfo::device_id);

  if (it == deviceInfos.end()) {
    GTEST_SKIP() << "Skipping this test because the device Logitech Gaming "
                    "Mouse G502 not found.";
  }

  auto info = std::move(*it);
  // TODO: this test is very linux specific
  EXPECT_TRUE(std::regex_match(info.path().begin(), info.path().end(), std::regex("/dev/hidraw\\d+"))); // /dev/hidraw0
  ASSERT_EQ(info.device_id().vid(), 0x046d);
  ASSERT_EQ(info.device_id().pid(), 0xc332);
  EXPECT_TRUE(std::regex_match(info.serial_number().toString(), std::regex("0E\\d{10}"))); // eg: my mouse serial is 0E6935743730
  ASSERT_GT(info.release_number(), 0x100); // eg: my mouse release is 0x301
  ASSERT_EQ(info.manufacturer_string().toString(), "Logitech");
  ASSERT_EQ(info.product_string().toString(), "Gaming Mouse G502");
  // TODO: usage_page and usage are enums https://developer.mozilla.org/ja/docs/Web/API/HIDDevice/collections
  ASSERT_EQ(info.usage_page(), 0x01); // eg. 0x01 means (mouse, keyboard)??
  ASSERT_EQ(info.usage(), 0x02);
  ASSERT_GE(info.interface_number(), 0);
  ASSERT_EQ(info.bus_type(), HidBusType::USB);

  auto g502_regex = std::regex("HIDDeviceInfo\\(path: /dev/hidraw\\d+, deviceId: DeviceID\\(vid: 0x046d, pid: 0xc332\\), serial_number: 0E\\d{10}, release_number: 0x\\d+, manufacturer_string: Logitech, product_string = Gaming Mouse G502, usage_page = 0x01, usage = 0x02, interface_number = \\d, bus_type = USB\\)");
  auto g502_to_string = std::format("{}", info);

  EXPECT_TRUE(std::regex_match(g502_to_string, g502_regex)) << g502_to_string;
}
