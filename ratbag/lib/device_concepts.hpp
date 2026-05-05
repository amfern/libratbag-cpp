#pragma once

// #include <vector>

// TODO:
// Each driver can support multiple devices
// Devices -> Device[Profile, Driver] -> Profile[led, button, macro, resoltuion]
//                                    -> Driver[read(), commit()]
// Create mapping between DeviceID and Device class, each device has an driver
// to be used to operate it. The same Driver could be used by many other devices
// There is no need to probe the driver for supported devices, because that
// would be known ahead of time.

// #include "ratbag/lib/common/profile.hpp"

#include <memory>
#include <optional>

#include "ratbag/lib/drivers_concepts/driver.hpp"
#include "ratbag/lib/hidapi/device.hpp"
#include "ratbag/lib/hidapi/device_info.hpp"
#include "ratbag/lib/common/profile.hpp"

namespace ratbag {
namespace lib {

// # concepts 

class Device {

public:
  // TODO(ask): maybe use exception instead of optional?
  //            use std::expected to return error what went wrong
  //            philosophical debate, either use exception of std::expected, but stay consistent.
  //            constructor can't return std::expected, so we can have a factory functions like here.
  //            exceptions can propogate, and handled at later stage up the stack. If i anticipate if the error will handle in the imidiate caller, then std::expected is more convenient.
  //            Maybe rename this function to "find", and then let the user call device.open()?
  //            if it's always find + open then maybe have single function.
  static std::optional<Device> open(hidapi::HIDDeviceInfo &hid_device_info) {
    auto drv_result = drivers_concepts::open(hid_device_info);

    // TODO(ask): i like to do these early exists to avoid long nested code in the if clasue, what do you thin about?
    // yes
    if (!drv_result) {
      // TODO(ask): prefer {}
      return {};
    }

    auto drv = drv_result.value();

    
    Device device{drv};
    device.load_profiles();

    return device;
  };

  void load_profiles() {
    std::visit([&](auto& d) { d.load(profiles); }, driver_);
  };

private:
  explicit Device(drivers_concepts::DriverVariants driver) : driver_(std::move(driver)) {} 

  drivers_concepts::DriverVariants driver_;

  ProfileList profiles;
};


} // namespace lib

} // namespace ratbag
