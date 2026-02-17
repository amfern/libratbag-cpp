#pragma once

#include <format>
#include <string_view>

#include "hidapi.h"

namespace ratbag {
namespace lib {
namespace hidapi {
namespace detail {

using HidBusType = hid_bus_type;

// it's not possible to test all outcomes of a big function,
// but we can intentionally test a small thing and relay on small thing
// working correcntly first It's harder to check all possible inputs on an
// higher level. integration - two different parts from different places are
// working togather, even if it's in the same process and same project. Use
// detail like in arene-base for all the private functions that need testing,
// extract helper class.
static constexpr std::string_view bus_type_to_string(HidBusType bus_type) {
  switch (bus_type) {
  case HID_API_BUS_USB:
    return std::string_view("USB");
    break;
  case HID_API_BUS_BLUETOOTH:
    return std::string_view("Bluetooth");
    break;
  case HID_API_BUS_I2C:
    return std::string_view("I2C");
    break;
  case HID_API_BUS_SPI:
    return std::string_view("SPI");
    break;
  case HID_API_BUS_UNKNOWN:
    return std::string_view("unknown");
  }

  return std::string_view("unknown");
}

// TOOD: testing why some symbols are not included
void miau();

} // namespace detail
} // namespace hidapi
} // namespace lib
} // namespace ratbag

namespace std {
using namespace ratbag::lib::hidapi::detail;

template <> struct formatter<HidBusType> : formatter<string_view> {

  template <class FormatContext>
  typename FormatContext::iterator format(const HidBusType &bus_type,
                                          FormatContext &ctx) const {
    auto name = bus_type_to_string(bus_type);
    return format_to(ctx.out(), "{}", name);
  }

};
} // namespace std
