#include "ratbag/lib/hidapi/device_info.hpp"

#include "gtest/gtest.h"

using ratbag::lib::hidapi::HIDDeviceInfo;
using ratbag::lib::hidapi::HidBusType;


// TODO(ask): Now that i create an wrapper for C class, what sort of tests
// should i write.

// TODO: https://github.com/libusb/hidapi/blob/master/hidtest/test.c
TEST(DeviceInfoTestSuit, EnumarateDevices) {
  auto deviceInfos = HIDDeviceInfo::enumerate_hid_devices();

  // TODO: i am testing an call to enumurate devices, but this would be
  // different on each PC. I can't find an good way to mock the devices, what
  // should i do in such situation that mocking is immpossible. Should i use

  // 1. linker --wrap function call to mock the C api return to test my C++ code.
  // 2. call the underlying API and compare the two...will work on all PC's
  // Best - Do both

  for (auto &info : deviceInfos) {
    std::cout << std::format("{}\n", info);
  }

  EXPECT_TRUE(true);
}

// TODO: fill this test by creating device.cpp
TEST(DeviceInfoTestSuit, OpenDeviceAndPrintDescriptors) {
  // auto deviceInfos = HIDDeviceInfo::enumerate_hid_devices();

  // for (auto &info : deviceInfos) {
  //   auto device = info.open();
  //   std::wcout << std::format(L"{}\n", device);
  // }

  // auto device = deviceInfos[0].open();

  // handle = hid_open(0x4d8, 0x3f, NULL);

  // if (!handle) {
  //   printf("unable to open device\n");
  //   // hid_exit();
  //   return 1;
  // }

  // HID_API_EXPORT hid_device * HID_API_CALL hid_open(unsigned short vendor_id,
  // unsigned short product_id, const wchar_t *serial_number);

  EXPECT_TRUE(true);
}
