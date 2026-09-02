#include "hidapi/detail/hid_bus_type_internal.hpp"

#include "gtest/gtest.h"

TEST(BusTypeToStringTest, CanReturnCorrectStringToEnum) {
  ASSERT_EQ(hidapi::detail::bus_type_to_string(hidapi::detail::HidBusTypeInternal::USB), "USB");
  ASSERT_EQ(hidapi::detail::bus_type_to_string(hidapi::detail::HidBusTypeInternal::Bluetooth), "Bluetooth");
  ASSERT_EQ(hidapi::detail::bus_type_to_string(hidapi::detail::HidBusTypeInternal::I2C), "I2C");
  ASSERT_EQ(hidapi::detail::bus_type_to_string(hidapi::detail::HidBusTypeInternal::SPI), "SPI");
  ASSERT_EQ(hidapi::detail::bus_type_to_string(hidapi::detail::HidBusTypeInternal::Unknown), "Unknown");
}

TEST(BusTypeToStringTest, CanFormatString) {
  ASSERT_EQ(std::format("This is {}", hidapi::detail::HidBusTypeInternal::USB), "This is USB");
}
