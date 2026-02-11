#include "ratbag/lib/hidapi/detail/bus_type_to_string.hpp"

#include <gtest/gtest.h>

using ratbag::lib::hidapi::detail::bus_type_to_string;

TEST(BusTypeToStringTest, CanReturnCorrectStringToEnum) {
  ASSERT_EQ(bus_type_to_string(HID_API_BUS_USB), L"USB");
  ASSERT_EQ(bus_type_to_string(HID_API_BUS_BLUETOOTH), L"Bluetooth");
  ASSERT_EQ(bus_type_to_string(HID_API_BUS_I2C), L"I2C");
  ASSERT_EQ(bus_type_to_string(HID_API_BUS_SPI), L"SPI");
  ASSERT_EQ(bus_type_to_string(HID_API_BUS_UNKNOWN), L"unknown");
}

TEST(BusTypeToStringTest, CanFormatString) {
  ASSERT_EQ(std::format(L"This is {}", HID_API_BUS_USB), L"This is USB");
}
