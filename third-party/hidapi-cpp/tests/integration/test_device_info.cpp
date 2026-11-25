#include "hidapi/device_info.hpp"

#include "gtest/gtest.h"

#include <algorithm>
#include <codecvt>

using hidapi::HidBusType;
using hidapi::HIDDeviceInfo;

// 1. Define the test fixture class
class DeviceInfoTestSuit : public ::testing::Test {
 protected:
  // Member variables available to all tests in this fixture
  struct hid_device_info *devs, *cur_dev;

  // 2. SetUp() runs before EVERY TEST_F in this suite
  void SetUp() override {
    devs = hid_enumerate(0, 0);  // 0,0 = find all devices
    cur_dev = devs;
  }

  // 3. TearDown() runs after EVERY TEST_F in this suite
  void TearDown() override { hid_free_enumeration(devs); }
};

// call the underlying API and compare the two...will work on all PC's
TEST_F(DeviceInfoTestSuit, EnumarateDevicesCompareToHidAPI) {
  if (cur_dev == nullptr) {
    hid_free_enumeration(devs);
    GTEST_SKIP() << "Skipping this test due no HID devices found in the system, requires atleast one HID device";
  }

  auto deviceInfos = HIDDeviceInfo::enumerate_hid_devices();

  for (auto& info : deviceInfos) {
    if (cur_dev == nullptr) {
      FAIL() << "HIDDeviceInfo::enumerate_hid_devices returned more elements "
                "than returned by hid_enumurate";
    }

    HidBusType bus_type = static_cast<HidBusType>(cur_dev->bus_type);
    ASSERT_EQ(info.path(), cur_dev->path);
    ASSERT_EQ(info.device_id().vid(), cur_dev->vendor_id);
    ASSERT_EQ(info.device_id().pid(), cur_dev->product_id);
    ASSERT_EQ(info.serial_number(), cur_dev->serial_number);
    ASSERT_EQ(info.release_number(), cur_dev->release_number);
    ASSERT_EQ(info.manufacturer_string(), cur_dev->manufacturer_string);
    ASSERT_EQ(info.product_string(), cur_dev->product_string);
    ASSERT_EQ(info.usage_page(), cur_dev->usage_page);
    ASSERT_EQ(info.usage(), cur_dev->usage);
    ASSERT_EQ(info.interface_number(), cur_dev->interface_number);
    ASSERT_EQ(info.bus_type(), bus_type);

    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string serial_number_utf8 = converter.to_bytes(cur_dev->serial_number);
    std::string manufacturer_string_utf8 = converter.to_bytes(cur_dev->manufacturer_string);
    std::string product_string_utf8 = converter.to_bytes(cur_dev->product_string);

    std::string hid_device_info_string = std::format(
        "HIDDeviceInfo(path: {}, deviceId: DeviceID(vid: {:#06x}, pid: "
        "{:#06x}), serial_number: {}, release_number: {:#05x}, manufacturer_string: "
        "{}, "
        "product_string = {}, usage_page = {:#04x}, usage = {:#04x}, "
        "interface_number = {}, bus_type = {})",
        cur_dev->path,
        cur_dev->vendor_id,
        cur_dev->product_id,
        serial_number_utf8,
        cur_dev->release_number,
        manufacturer_string_utf8,
        product_string_utf8,
        cur_dev->usage_page,
        cur_dev->usage,
        cur_dev->interface_number,
        bus_type
    );

    ASSERT_EQ(std::format("{}", info), hid_device_info_string);

    cur_dev = cur_dev->next;
  }

  if (cur_dev != nullptr) {
    FAIL() << "HIDDeviceInfo::enumerate_hid_devices returned less elements "
              "than returned by hid_enumurate";
  }
}
