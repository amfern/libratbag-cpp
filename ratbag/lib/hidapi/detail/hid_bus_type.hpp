#pragma once

#include <string_view>
#include <format>

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
  static constexpr std::wstring_view bus_type_to_string(HidBusType bus_type) {
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


// TODO: i wonder if i can move these templates into cpp file
template <typename CharT> struct std::formatter<ratbag::lib::hidapi::detail::HidBusType, CharT> {

  constexpr auto parse(auto &ctx) {
    // TODO: what is this function needed for? what does it do?
    return ctx.begin();
  }

  auto format(const ratbag::lib::hidapi::detail::HidBusType &bus_type, auto &ctx) const {
    if constexpr (std::is_same_v<CharT, char>) {
      // TODO: i don't want to be implementing the switch case bellow for utf8
      // and wchar_t types...
      // TODO: convert all wchar_t to utf8, pay the encoding fee to the god of performance. To make it more ergonomically to use my api
    } else if constexpr (std::is_same_v<CharT, wchar_t>) {
      auto name = ratbag::lib::hidapi::detail::bus_type_to_string(bus_type);
      return std::format_to(ctx.out(), L"{}", name);
    }
  }
};
