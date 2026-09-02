#include "hidapi/detail/hid_bus_type.hpp"

#include "gtest/gtest.h"

// TODO: should include whole namespace or only specific stuff i need?
// TODO: prefer global qualifiers hidapi::detail::
// ans: using is okay, but "using" in headers files has to be considerate because it's an intentionl choice that will make namespace available udner a new name to all users of the header file
using hidapi::detail::bus_type_to_string;
using hidapi::detail::HidBusType;

using hidapi::detail::to_string;

// using namespace hidapi::detail;

TEST(BusTypeToStringTest, CanReturnCorrectStringToEnum) {
  to_string();


  ASSERT_EQ(hidapi::detail::bus_type_to_string(hidapi::detail::HidBusType::USB), "USB");
  ASSERT_EQ(hidapi::detail::bus_type_to_string(hidapi::detail::HidBusType::Bluetooth), "Bluetooth");
  ASSERT_EQ(hidapi::detail::bus_type_to_string(hidapi::detail::HidBusType::I2C), "I2C");
  ASSERT_EQ(hidapi::detail::bus_type_to_string(hidapi::detail::HidBusType::SPI), "SPI");
  ASSERT_EQ(hidapi::detail::bus_type_to_string(hidapi::detail::HidBusType::Unknown), "Unknown");
}

TEST(BusTypeToStringTest, CanFormatString) {
  ASSERT_EQ(std::format("This is {}", hidapi::detail::HidBusType::USB), "This is USB");
}
