#include "ratbag/lib/hidapi/detail/hid_bus_type.hpp"

#include <gtest/gtest.h>

using ratbag::lib::hidapi::detail::bus_type_to_string;

TEST(BusTypeToStringTest, CanReturnCorrectStringToEnum) {
  ASSERT_EQ(bus_type_to_string(HID_API_BUS_USB), "USB");
  ASSERT_EQ(bus_type_to_string(HID_API_BUS_BLUETOOTH), "Bluetooth");
  ASSERT_EQ(bus_type_to_string(HID_API_BUS_I2C), "I2C");
  ASSERT_EQ(bus_type_to_string(HID_API_BUS_SPI), "SPI");
  ASSERT_EQ(bus_type_to_string(HID_API_BUS_UNKNOWN), "unknown");
}

TEST(BusTypeToStringTest, CanFormatString) {
  ASSERT_EQ(std::format("This is {}", HID_API_BUS_USB), "This is USB");
}
