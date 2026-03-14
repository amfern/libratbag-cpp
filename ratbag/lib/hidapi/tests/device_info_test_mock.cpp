#include "ratbag/lib/hidapi/device_info.hpp"
#include <codecvt>

#include "gtest/gtest.h"

using ratbag::lib::hidapi::HidBusType;
using ratbag::lib::hidapi::HIDDeviceInfo;

TEST(DeviceInfoTestSuit, EnumarateMockedDevices) {
  auto deviceInfos = HIDDeviceInfo::enumerate_hid_devices();

  // hid_enumerate() is mocked in hid_mocked.c to always return the same value
  EXPECT_EQ(
      std::format("{}", deviceInfos.front()),
      "HIDDeviceInfo(path: /mock/path, deviceId: DeviceID(vid: 0x1234, pid: "
      "0x4321), serial_number: serial number, release_number: 1, "
      "manufacturer_string: mock manufacturer, product_string = mock product, "
      "usage_page = 2, usage = 3, interface_number = 4, bus_type = SPI)");
}
