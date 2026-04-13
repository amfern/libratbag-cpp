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

#include "ratbag/lib/drivers_variant/driver.hpp"
#include "ratbag/lib/hidapi/device.hpp"
#include "ratbag/lib/hidapi/device_info.hpp"

namespace ratbag {
namespace lib {

// ##### variant
class Device {

public:
  static std::optional<Device> open(hidapi::HIDDeviceInfo &hid_device_info) {
    auto drv_result = drivers_variant::Driver::open(hid_device_info);

    if (!drv_result) {
      return {};
    }


    auto drv = drv_result.value();

    std::visit([&](auto& d) { d.probe(); }, drv);

    return Device{drv}; 
  };

private:

  explicit Device(drivers_variant::DriverVariants driver) : driver_(std::move(driver)) {} 

  drivers_variant::DriverVariants driver_;
};


} // namespace lib

} // namespace ratbag
