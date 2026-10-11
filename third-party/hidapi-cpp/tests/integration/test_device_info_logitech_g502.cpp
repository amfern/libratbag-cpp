#include "hidapi/device_info.hpp"

#include "gtest/gtest.h"
// #include "gmock/gmock.h"

#include <algorithm>
#include <codecvt>
#include <print>
#include <regex>
#include <stdexcept>
#include <ranges>

using hidapi::DeviceID;
using hidapi::HidBusType;
using hidapi::HIDDeviceInfo;

static constexpr hidapi::VendorID G502_VID = 0x046d; 
static constexpr hidapi::ProductID G502_PID = 0xc332;

// using ::testing::MatchesRegex;

// TODO: Run the test when the device is avaiable

// call the underlying API and compare the two...will work on all PC's
TEST(DeviceTestSuit, OpenDevice) {
  // TODO(ask): this funciton may throw an exception, and i don't handle it. clang-tidy doesn't warn about it.
  //            This fails i the CI, because there is no devices

  // TODO(ask): how do catch error and return the std::vector out of the catch block?
  // hidapi::HIDDeviceInfoList deviceInfos;
  // try {
  //   deviceInfos = HIDDeviceInfo::enumerate_hid_devices();
  // } catch(std::runtime_error e) {
  //   GTEST_SKIP() << "Skipping this test because the device Logitech Gaming "
  //                   "Mouse G502 not found.";    
  // }

  auto deviceInfos = HIDDeviceInfo::enumerate_hid_devices({G502_VID, G502_PID});
  // auto it = std::ranges::find(deviceInfos, DeviceID{0x046d, 0xc332}, &HIDDeviceInfo::device_id);

  if (deviceInfos.empty()) {
    GTEST_SKIP() << "Skipping this test because the device Logitech Gaming "
                    "Mouse G502 not found.";
  }

  // Single USB can identify as multiple hid devices
  ASSERT_EQ(deviceInfos.size(), 6);

  std::array<hidapi::UsagePage, 6> usagePages{0x01, 0x01, 0x0c, 0x01, 0xff00, 0xff00};
  std::array<hidapi::Usage, 6> usages{0x02, 0x06, 0x01, 0x80, 0x01, 0x02};

  // C++23 structured binding with zip view
  for (auto&& [info, usagePage, usage] : std::views::zip(deviceInfos, usagePages, usages)) {
    EXPECT_TRUE(
        std::regex_match(info.path().begin(), info.path().end(), std::regex("/dev/hidraw\\d+"))
    );  // /dev/hidraw0
    ASSERT_EQ(info.device_id().vid(), G502_VID);
    ASSERT_EQ(info.device_id().pid(), G502_PID);
    EXPECT_TRUE(
        std::regex_match(info.serial_number().toString(), std::regex("0E\\d{10}"))
    );                                        // eg: my mouse serial is 0E6935743730
    ASSERT_GT(info.release_number(), 0x100);  // eg: my mouse release is 0x301
    ASSERT_EQ(info.manufacturer_string().toString(), "Logitech");
    ASSERT_EQ(info.product_string().toString(), "Gaming Mouse G502");
    // TODO: usage_page and usage are enums https://developer.mozilla.org/ja/docs/Web/API/HIDDevice/collections
    ASSERT_EQ(info.usage_page(), usagePage);  // eg. 0x01 means (mouse, keyboard)??
    ASSERT_EQ(info.usage(), usage);
    ASSERT_GE(info.interface_number(), 0);
    ASSERT_EQ(info.bus_type(), HidBusType::USB);

    std::string regex_fmt = std::format("HIDDeviceInfo\\(path: /dev/hidraw\\d+, deviceId: DeviceID\\(vid: 0x046d, pid: 0xc332\\), serial_number: "
      "0E\\d{{10}}, release_number: 0x\\d+, manufacturer_string: Logitech, product_string = Gaming Mouse G502, "
        "usage_page = {:#04x}, usage = {:#04x}, interface_number = \\d, bus_type = USB\\)", usagePage, usage);

    auto g502_to_string = std::format("{}", info);

    EXPECT_TRUE(std::regex_match(g502_to_string, std::regex(regex_fmt))) << g502_to_string << std::endl << regex_fmt;
  }
}
