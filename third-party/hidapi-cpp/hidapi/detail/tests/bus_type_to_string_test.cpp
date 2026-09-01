#include "hidapi/detail/hid_bus_type.hpp"

#include "gtest/gtest.h"

// TODO(ask): should include whole namespace or only specific stuff i need?
// using hidapi::detail::bus_type_to_string;
// using hidapi::detail::HidBusType;
using namespace hidapi::detail;

TEST(BusTypeToStringTest, CanReturnCorrectStringToEnum) {
  ASSERT_EQ(bus_type_to_string(HidBusType::USB), "USB");
  ASSERT_EQ(bus_type_to_string(HidBusType::Bluetooth), "Bluetooth");
  ASSERT_EQ(bus_type_to_string(HidBusType::I2C), "I2C");
  ASSERT_EQ(bus_type_to_string(HidBusType::SPI), "SPI");
  ASSERT_EQ(bus_type_to_string(HidBusType::Unknown), "Unknown");
}

TEST(BusTypeToStringTest, CanFormatString) {
  ASSERT_EQ(std::format("This is {}", HidBusType::USB), "This is USB");
}
