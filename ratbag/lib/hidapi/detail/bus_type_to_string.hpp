#pragma once

#include <string_view>

#include "hidapi.h"

namespace ratbag {
namespace lib {
namespace hidapi {
namespace detail {

  // TODO: unit test this function
  // TODO: move this function to detail
  // it's not possible to test all outcomes of a big function,
  // but we can intentionally test a small thing and relay on small thing
  // working correcntly first It's harder to check all possible inputs on an
  // higher level. integration - two different parts from different places are
  // working togather, even if it's in the same process and same project. Use
  // detail like in arene-base for all the private functions that need testing,
  // extract helper class.
  static constexpr std::wstring_view bus_type_to_string(hid_bus_type bus_type) {
    switch (bus_type) {
    case HID_API_BUS_USB:
      return std::wstring_view(L"USB");
      break;
    case HID_API_BUS_BLUETOOTH:
      return std::wstring_view(L"Bluetooth");
      break;
    case HID_API_BUS_I2C:
      return std::wstring_view(L"I2C");
      break;
    case HID_API_BUS_SPI:
      return std::wstring_view(L"SPI");
      break;
    case HID_API_BUS_UNKNOWN:
      return std::wstring_view(L"unknown");
    }

    return std::wstring_view(L"unknown");
  }

}
}
}
}  // namespace
