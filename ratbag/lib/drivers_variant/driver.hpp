#pragma once

#include "ratbag/lib/hidapi/device_info.hpp"

#include "ratbag/lib/drivers_variant/steelseries.hpp"
#include "ratbag/lib/drivers_variant/hidpp20.hpp"


#include <optional>
#include <variant>

namespace ratbag {
namespace lib {
namespace drivers_variant {

// TODO(ask): How would i convert this driver to be
// concepts looks promising, but it's not exactly what i need
// CRTP (Curiously Recurring Template Pattern) looks closer to what i would like
// to have which zero cost polymorphisim, which is resolved in compile time.
// Altough i am not sure how to implement it

using DriverVariants = std::variant<HIDPP20, SteelSeries>;

class Driver {
public:
  static std::optional<DriverVariants> open(hidapi::HIDDeviceInfo &hid_device_info);

};

} // namespace drivers
} // namespace lib
} // namespace ratbag
