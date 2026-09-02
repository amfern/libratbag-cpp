#include "hidapi/device_info.hpp"
#include <codecvt>

#include "gtest/gtest.h"
#include <concepts>
#include <type_traits>

using namespace hidapi;

TEST(DeviceInfoTest, CanEnumarate) {
  auto deviceInfos = HIDDeviceInfo::enumerate_hid_devices();

  auto& deviceInfo = deviceInfos.front();
  EXPECT_EQ(deviceInfo.path(), "/mock/path");
  EXPECT_EQ(deviceInfo.device_id(), DeviceID(0x1234, 0x4321));
  EXPECT_EQ(deviceInfo.serial_number().toString(), "serial number");
  EXPECT_EQ(deviceInfo.release_number(), 1);
  EXPECT_EQ(deviceInfo.manufacturer_string().toString(), "mock manufacturer");
  EXPECT_EQ(deviceInfo.product_string().toString(), "mock product");
  EXPECT_EQ(deviceInfo.usage_page(), 2);
  EXPECT_EQ(deviceInfo.usage(), 3);
  EXPECT_EQ(deviceInfo.interface_number(), 4);
  EXPECT_EQ(deviceInfo.bus_type(), HidBusType::SPI);
}

TEST(DeviceInfoTest, CanMove) {
  auto deviceInfos = HIDDeviceInfo::enumerate_hid_devices();

  auto newDeviceInfo(std::move(deviceInfos.front()));
  // check that newDeviceInfo has the correct mocked values
  
  EXPECT_EQ(newDeviceInfo.path(), "mock/path"); // example
  // TODO: .... do same checks for the rest of the attributes
  // auto newNewDeviceInfo = std::move(newDeviceInfo);

  
  // check that the moved-from has the
  auto& movedFrom = deviceInfos.front();
  EXPECT_EQ(movedFrom.path(), ""); // example
  // TODO: .... do same checks for the rest of the attributes


  // move back to a deviceInfo that was moved from, to see that all attributes are correcly assign and the moved-from can be used again
  deviceInfos.front() = std::move(newDeviceInfo);
  EXPECT_EQ(deviceInfos.front(), "mock/path"); // example
  // TODO: .... do same checks for the rest of the attributes  
}


// TODO: implement build fail test with Yuya's bazel rule
TEST(DeviceInfoTest, CanNotCopy) {
  static_assert(!std::copy_constructible<HIDDeviceInfo>);
  static_assert(!std::is_copy_assignable_v<HIDDeviceInfo>); // TODO: why no copy assignable concept?
}

TEST(DeviceInfoTest, CanPrintFormat) {
  auto deviceInfos = HIDDeviceInfo::enumerate_hid_devices();

  // hid_enumerate() is mocked in hid_mocked.c to always return the same value
  EXPECT_EQ(
      std::format("{}", deviceInfos.front()),
      "HIDDeviceInfo(path: /mock/path, deviceId: DeviceID(vid: 0x1234, pid: "
      "0x4321), serial_number: serial number, release_number: 1, "
      "manufacturer_string: mock manufacturer, product_string = mock product, "
      "usage_page = 2, usage = 3, interface_number = 4, bus_type = SPI)");
}
