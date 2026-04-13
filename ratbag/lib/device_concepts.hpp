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

namespace ratbag {
namespace lib {

// # concepts 

class Device {

public:
  // TODO: maybe use exception instead of optional?
  static std::optional<Device> open(hidapi::HIDDeviceInfo &hid_device_info) {
    auto drv_result = drivers_concepts::open(hid_device_info);

    // TODO(ask): i like to do these early exists to avoid long nested code in the if clasue, what do you thin about?
    if (!drv_result) {
      return {}; // TODO(ask): {} vs std::nullopt what syntax is prefered here?
    }

    auto drv = drv_result.value();

    return Device{drv}; 
  };

private:

  explicit Device(drivers_concepts::DriverVariants driver) : driver_(std::move(driver)) {} 

  
  drivers_concepts::DriverVariants driver_;
};


} // namespace lib

} // namespace ratbag
