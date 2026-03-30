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
  // TODO: maybe use exception instead of optional?
  static std::optional<Device> open(hidapi::HIDDeviceInfo &hid_device_info) {
    auto drv_result = drivers_variant::Driver::open(hid_device_info);

    // TODO(ask): i like to do these early exists to avoid long nested code in the if clasue, what do you thin about?
    if (!drv_result) {
      return {}; // TODO(ask): {} vs std::nullopt what syntax is prefered here?
    }


    auto drv = drv_result.value();
    // TODO(ask): what solution has less pointer manipulation, the concepts or the dynamic inheritance?
    //            what solution would be faster in runtime, std::variant or concepts?
    //            I see that with std::variant i have to call std::visit with anonymouse funciton, which is basically a pointer?
    std::visit([&](auto& d) { d.probe(); }, drv);

    return Device{drv}; 
  };

private:
  // TODO: how to throw compile error when same deviceId is already assigned to
  // another one How to make this variable unique acrsso all of the devices?
  // TODO: convert to unique key hash map
  // static const std::array devices_ = {
  //   Device(ratbag::lib::hidapi::DeviceID{0x046d, 0xc08b},
  //   ratbag::lib::drivers::HIDPP20);
  // };

  explicit Device(drivers_variant::DriverVariants driver) : driver_(std::move(driver)) {} 

  drivers_variant::DriverVariants driver_;
};


} // namespace lib

} // namespace ratbag
