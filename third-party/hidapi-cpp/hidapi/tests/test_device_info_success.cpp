#include <codecvt>
#include <concepts>
#include <type_traits>

#include "gtest/gtest.h"
#include "hidapi/device_info.hpp"

TEST(DeviceInfoTest, CanEnumarate) {
  auto deviceInfos = hidapi::HIDDeviceInfo::enumerate_hid_devices();

  auto &deviceInfo = deviceInfos.front();
  EXPECT_EQ(deviceInfo.path(), "/mock/path");
  EXPECT_EQ(deviceInfo.device_id(), hidapi::DeviceID(0x1234, 0x4321));
  EXPECT_EQ(deviceInfo.serial_number().toString(), "serial number");
  EXPECT_EQ(deviceInfo.release_number(), 1);
  EXPECT_EQ(deviceInfo.manufacturer_string().toString(), "mock manufacturer");
  EXPECT_EQ(deviceInfo.product_string().toString(), "mock product");
  EXPECT_EQ(deviceInfo.usage_page(), 2);
  EXPECT_EQ(deviceInfo.usage(), 3);
  EXPECT_EQ(deviceInfo.interface_number(), 4);
  EXPECT_EQ(deviceInfo.bus_type(), hidapi::HidBusType::SPI);
}

TEST(DeviceInfoTest, CanMove) {
  auto deviceInfos = hidapi::HIDDeviceInfo::enumerate_hid_devices();

  // move via move constructor
  auto newDeviceInfo(std::move(deviceInfos.front()));

  // check that moved-to object has the correct values
  EXPECT_EQ(newDeviceInfo.path(), "/mock/path");  // example
  EXPECT_EQ(newDeviceInfo.device_id(), hidapi::DeviceID(0x1234, 0x4321));
  EXPECT_EQ(newDeviceInfo.serial_number(), L"serial number");
  EXPECT_EQ(newDeviceInfo.release_number(), 1);
  EXPECT_EQ(newDeviceInfo.manufacturer_string(), L"mock manufacturer");
  EXPECT_EQ(newDeviceInfo.product_string(), L"mock product");
  EXPECT_EQ(newDeviceInfo.usage_page(), 2);
  EXPECT_EQ(newDeviceInfo.usage(), 3);
  EXPECT_EQ(newDeviceInfo.interface_number(), 4);
  EXPECT_EQ(newDeviceInfo.bus_type(), hidapi::HidBusType::SPI);

  // check that the moved-from is valid bu unspecified
  // std:: library expects that object after move:
  //  1. Can be destroyed
  //  2. Can be moved into
  //  3. Strive towards allowing people to use member function on movedFrom, but
  //  not mandatory
  //  4. Last resort implement .isValid()
  //     a. We don't want move operation to throw, and sometimes it move out
  //     operation requries to reallocation an empty object, and in that case
  //     it's better to say object is in invalid state. b. i can instruct users
  //     to not use memebers function of movedFrom, but it adds more cognitive
  //     load and gotaches for end useres to worry about.
  //  The Rule of thum:
  //  - Function that do queries(getters function) return some default values
  //  - Modifies(setters function) throw
  auto &movedFrom = deviceInfos.front();
  ASSERT_DEATH(movedFrom.path(), ".*called on moved-from object");
  ASSERT_DEATH(movedFrom.device_id(), ".*called on moved-from object");
  ASSERT_DEATH(movedFrom.serial_number(), ".*called on moved-from object");
  ASSERT_DEATH(movedFrom.release_number(), ".*called on moved-from object");
  ASSERT_DEATH(movedFrom.manufacturer_string(),
               ".*called on moved-from object");
  ASSERT_DEATH(movedFrom.product_string(), ".*called on moved-from object");
  ASSERT_DEATH(movedFrom.usage_page(), ".*called on moved-from object");
  ASSERT_DEATH(movedFrom.usage(), ".*called on moved-from object");
  ASSERT_DEATH(movedFrom.interface_number(), ".*called on moved-from object");
  ASSERT_DEATH(movedFrom.bus_type(), ".*called on moved-from object");

  // move back via move assign operator
  movedFrom = std::move(newDeviceInfo);
  EXPECT_EQ(movedFrom.path(), "/mock/path");  // example
  EXPECT_EQ(movedFrom.device_id(), hidapi::DeviceID(0x1234, 0x4321));
  EXPECT_EQ(movedFrom.serial_number().toString(), "serial number");
  EXPECT_EQ(movedFrom.release_number(), 1);
  EXPECT_EQ(movedFrom.manufacturer_string().toString(), "mock manufacturer");
  EXPECT_EQ(movedFrom.product_string().toString(), "mock product");
  EXPECT_EQ(movedFrom.usage_page(), 2);
  EXPECT_EQ(movedFrom.usage(), 3);
  EXPECT_EQ(movedFrom.interface_number(), 4);
  EXPECT_EQ(movedFrom.bus_type(), hidapi::HidBusType::SPI);
}

TEST(DeviceInfoTest, CanMoveAndGoOutOfScope) {
  auto deviceInfos = hidapi::HIDDeviceInfo::enumerate_hid_devices();

  {
    auto newDeviceInfo(std::move(deviceInfos.front()));
  }
}

TEST(DeviceInfoTest, CanNotCopy) {
  static_assert(!std::copy_constructible<hidapi::HIDDeviceInfo>);
  static_assert(!std::is_copy_assignable_v<hidapi::HIDDeviceInfo>);
}

TEST(DeviceInfoTest, CanPrintFormat) {
  auto deviceInfos = hidapi::HIDDeviceInfo::enumerate_hid_devices();

  // hid_enumerate() is mocked in hid_mocked.c to always return the same value
  EXPECT_EQ(
      std::format("{}", deviceInfos.front()),
      "HIDDeviceInfo(path: /mock/path, deviceId: DeviceID(vid: 0x1234, pid: "
      "0x4321), serial_number: serial number, release_number: 1, "
      "manufacturer_string: mock manufacturer, product_string = mock product, "
      "usage_page = 2, usage = 3, interface_number = 4, bus_type = SPI)");
}
