#include "hidapi/device_info.hpp"
#include <codecvt>

#include "gtest/gtest.h"
#include <concepts>
#include <type_traits>

TEST(DeviceInfoTest, CanEnumarate) {
  auto deviceInfos = hidapi::HIDDeviceInfo::enumerate_hid_devices();

  auto& deviceInfo = deviceInfos.front();
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
  EXPECT_EQ(newDeviceInfo.path(), "/mock/path"); // example
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
  // TODO(ask): last time we spoke, i asked how to test move constructors, and should i even be testing the state of moved-from object?
  //            according to C++ i should avoid calling or using the moved from object, and indid
  //            after move, the internal pointer of device_info_ is nullptr, so it triggers ASAN error
  auto& movedFrom = deviceInfos.front();
  // std::ignore = movedFrom;
  // EXPECT_EQ(movedFrom.path(), ""); // example
  // // TODO: actually update those checks
  // EXPECT_EQ(movedFrom.device_id(), hidapi::DeviceID(0x0, 0x0));
  // EXPECT_EQ(movedFrom.serial_number(), L"");
  // EXPECT_EQ(movedFrom.release_number(), 0);
  // EXPECT_EQ(movedFrom.manufacturer_string(), L"");
  // EXPECT_EQ(movedFrom.product_string(), L"");
  // EXPECT_EQ(movedFrom.usage_page(), 2);
  // EXPECT_EQ(movedFrom.usage(), 3);
  // EXPECT_EQ(movedFrom.interface_number(), 4);
  // EXPECT_EQ(movedFrom.bus_type(), hidapi::HidBusType::SPI);

  // move back via move assign operator
  movedFrom = std::move(newDeviceInfo);
  EXPECT_EQ(movedFrom.path(), "/mock/path"); // example
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
