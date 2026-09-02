#include "hidapi/detail/hid_bus_type.hpp"

#include "gtest/gtest.h"

TEST(BusTypeToStringTest, CanReturnCorrectStringToEnum) {
  ASSERT_EQ(hidapi::detail::bus_type_to_string(hidapi::detail::HidBusType::USB), "USB");
  ASSERT_EQ(hidapi::detail::bus_type_to_string(hidapi::detail::HidBusType::Bluetooth), "Bluetooth");
  ASSERT_EQ(hidapi::detail::bus_type_to_string(hidapi::detail::HidBusType::I2C), "I2C");
  ASSERT_EQ(hidapi::detail::bus_type_to_string(hidapi::detail::HidBusType::SPI), "SPI");
  ASSERT_EQ(hidapi::detail::bus_type_to_string(hidapi::detail::HidBusType::Unknown), "Unknown");
}

TEST(BusTypeToStringTest, CanFormatString) {
  ASSERT_EQ(std::format("This is {}", hidapi::detail::HidBusType::USB), "This is USB");
}
