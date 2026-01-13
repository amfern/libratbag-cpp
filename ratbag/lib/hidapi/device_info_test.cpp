#include "ratbag/lib/hidapi/device_info.hpp"

#include "gtest/gtest.h"

TEST(DeviceInfoTestSuit, CreateAndMoveDeviceInfo) {
  auto path = new char[100];
  const char *ptr_path = "myPath/somehewere/there";
  std::strcpy(path, ptr_path);

  auto serial_number = new wchar_t[100];
  const wchar_t *ptr_serial_number = L"Myserial";
  std::wcscpy(serial_number, ptr_serial_number);

  auto manufacturer_string = new wchar_t[100];
  const wchar_t *ptr_manufacturer_string = L"Myserial";
  std::wcscpy(manufacturer_string, ptr_manufacturer_string);

  auto product_string = new wchar_t[100];
  const wchar_t *ptr_product_string = L"Myserial";
  std::wcscpy(product_string, ptr_product_string);

  hid_device_info hid_device_dummy = {
      .path = path,
      .vendor_id = 0x123,
      .product_id = 0x321,
      .serial_number = serial_number,
      .release_number = 0,
      .manufacturer_string = manufacturer_string,
      .product_string = product_string,
      .usage_page = 0,
      .usage = 0,
      .interface_number = 0,
      .next = nullptr,
      .bus_type = HID_API_BUS_UNKNOWN,
  };

  // TODO: validate move works correctly
  {
    auto info = ratbag::lib::hidapi::HIDDeviceInfo(hid_device_dummy);
    auto info2 = std::move(info);
  }

  delete[] path;
  delete[] serial_number;
  delete[] manufacturer_string;
  delete[] product_string;

  EXPECT_TRUE(true);
}
