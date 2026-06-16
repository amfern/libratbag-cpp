#pragma once

#include "ratbag/lib/hidapi/device_info.hpp"

#include "ratbag/lib/drivers_variant/steelseries.hpp"
#include "ratbag/lib/drivers_variant/hidpp20.hpp"


#include <optional>
#include <variant>

namespace ratbag {
namespace lib {
namespace drivers_variant {

using DriverVariants = std::variant<HIDPP20, SteelSeries>;

class Driver {
public:
  static std::optional<DriverVariants> open(hidapi::HIDDeviceInfo &hid_device_info);

};

} // namespace drivers
} // namespace lib
} // namespace ratbag
