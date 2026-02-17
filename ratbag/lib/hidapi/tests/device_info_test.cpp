#include "ratbag/lib/hidapi/device_info.hpp"
#include <codecvt>

#include "gtest/gtest.h"

using ratbag::lib::hidapi::HidBusType;
using ratbag::lib::hidapi::HIDDeviceInfo;

// TODO(ask): Now that i create an wrapper for C class, what sort of tests
// should i write.

// TODO: https://github.com/libusb/hidapi/blob/master/hidtest/test.c
TEST(DeviceInfoTestSuit, EnumarateDevices) {
  auto deviceInfos = HIDDeviceInfo::enumerate_hid_devices();

  // TODO: i am testing an call to enumurate devices, but this would be
  // different on each PC. I can't find an good way to mock the devices, what
  // should i do in such situation that mocking is immpossible. Should i use

  // 1. linker --wrap function call to mock the C api return to test my C++
  // code.

  for (auto &info : deviceInfos) {
    std::cout << std::format("{}\n", info);
  }

  SUCCEED();
}

// call the underlying API and compare the two...will work on all PC's
TEST(DeviceInfoTestSuit, EnumarateDevicesCompareToHidAPI) {
  auto deviceInfos = HIDDeviceInfo::enumerate_hid_devices();
  struct hid_device_info *devs, *cur_dev;
  devs = hid_enumerate(0, 0); // 0,0 = find all devices
  cur_dev = devs;

  for (auto &info : deviceInfos) {
    if (cur_dev == nullptr) {
      FAIL() << "HIDDeviceInfo::enumerate_hid_devices returned more elements than returned by hid_enumurate";
    }

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
    ASSERT_EQ(info.bus_type(), cur_dev->bus_type);

    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string manufacturer_string_utf8 = converter.to_bytes(cur_dev->manufacturer_string);
    std::string product_string_utf8 = converter.to_bytes(cur_dev->product_string);
    std::string serial_number_utf8 = converter.to_bytes(cur_dev->serial_number);

    std::string hid_device_info_string = std::format(
        "HIDDeviceInfo(path: {}, deviceId: DeviceID(vid: {:#06x}, pid: "
        "{:#06x}), serial_number: {}, release_number: {}, manufacturer_string: "
        "{}, "
        "product_string = {}, usage_page = {}, usage = {}, "
        "interface_number = {}, bus_type = {})",
        cur_dev->path, cur_dev->vendor_id, cur_dev->product_id,
        serial_number_utf8, cur_dev->release_number, manufacturer_string_utf8,
        product_string_utf8, cur_dev->usage_page, cur_dev->usage,
        cur_dev->interface_number, cur_dev->bus_type);

    ASSERT_EQ(std::format("{}", info), hid_device_info_string);

    cur_dev = cur_dev->next;
  }

  if (cur_dev != nullptr) {
    FAIL() << "HIDDeviceInfo::enumerate_hid_devices returned less elements than returned by hid_enumurate";
  }

  hid_free_enumeration(devs);
}



// // TODO: fill this test by creating device.cpp
// TEST(DeviceInfoTestSuit, OpenDeviceAndPrintDescriptors) {
//   // auto deviceInfos = HIDDeviceInfo::enumerate_hid_devices();

//   // for (auto &info : deviceInfos) {
//   //   auto device = info.open();
//   //   std::wcout << std::format(L"{}\n", device);
//   // }

//   // auto device = deviceInfos[0].open();

//   // handle = hid_open(0x4d8, 0x3f, NULL);

//   // if (!handle) {
//   //   printf("unable to open device\n");
//   //   // hid_exit();
//   //   return 1;
//   // }

//   // HID_API_EXPORT hid_device * HID_API_CALL hid_open(unsigned short vendor_id,
//   // unsigned short product_id, const wchar_t *serial_number);

//   EXPECT_TRUE(true);
// }
