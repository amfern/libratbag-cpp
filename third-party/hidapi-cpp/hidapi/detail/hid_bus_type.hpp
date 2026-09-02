#pragma once

#include <format>
#include <string_view>

#include "hidapi.h"

namespace hidapi {
namespace detail {

enum class HidBusType : std::underlying_type_t<hid_bus_type> {
  Unknown = HID_API_BUS_UNKNOWN,
  // USB bus
  //   Specifications:
  //   https://usb.org/hid
  USB = HID_API_BUS_USB,
  // Bluetooth or Bluetooth LE bus
  //    Specifications:
  //    https://www.bluetooth.com/specifications/specs/human-interface-device-profile-1-1-1/
  //    https://www.bluetooth.com/specifications/specs/hid-service-1-0/
  //    https://www.bluetooth.com/specifications/specs/hid-over-gatt-profile-1-0/
  Bluetooth = HID_API_BUS_BLUETOOTH,
  // I2C bus
  //     Specifications:
  //     https://docs.microsoft.com/previous-versions/windows/hardware/design/dn642101(v=vs.85)
  I2C = HID_API_BUS_I2C,
  // SPI bus
  //   Specifications: https://www.microsoft.com/download/details.aspx?id=103325
  SPI = HID_API_BUS_SPI,
  // Virtual device
  //    E.g.: https://elixir.bootlin.com/linux/v4.0/source/include/uapi/linux/input.h#L955 
  // Virtual = HID_API_BUS_VIRTUAL,
};

// using HidBusType = hid_bus_type;


// TODO: with c++26 we can use reflections instead of manual switch case
static constexpr std::string_view bus_type_to_string(HidBusType bus_type) {
  switch (bus_type) {
  case HidBusType::USB:
    return std::string_view("USB");
    break;
  case HidBusType::Bluetooth:
    return std::string_view("Bluetooth");
    break;
  case HidBusType::I2C:
    return std::string_view("I2C");
    break;
  case HidBusType::SPI:
    return std::string_view("SPI");
    break;
  case HidBusType::Unknown:
    return std::string_view("Unknown");
  }

  return std::string_view("Unknown");
}

} // namespace detail
} // namespace hidapi

namespace std {

template <> struct formatter<hidapi::detail::HidBusType> : formatter<string_view> {

  template <class FormatContext>
  typename FormatContext::iterator format(const hidapi::detail::HidBusType &bus_type,
                                          FormatContext &ctx) const {
    auto name = hidapi::detail::bus_type_to_string(bus_type);
    return format_to(ctx.out(), "{}", name);
  }
};
} // namespace std
