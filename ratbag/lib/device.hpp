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

#include <memory>
#include <optional>

#include "ratbag/lib/driver/driver.hpp"
#include "ratbag/lib/hidapi/device.hpp"
#include "ratbag/lib/hidapi/device_info.hpp"
#include "ratbag/lib/profile/profile.hpp"

namespace ratbag {
namespace lib {

// # concepts 

class Device {

public:
  // TODO: maybe use exception instead of optional?
  //            use std::expected to return error what went wrong
  //            philosophical debate, either use exception of std::expected, but stay consistent.
  //            constructor can't return std::expected, so we can have a factory functions like here.
  //            exceptions can propogate, and handled at later stage up the stack. If i anticipate if the error will handle in the imidiate caller, then std::expected is more convenient.
  //            Maybe rename this function to "find", and then let the user call device.open()?
  //            if it's always find + open then maybe have single function.
  static std::optional<Device> open(hidapi::HIDDeviceInfo &hid_device_info) {
    auto drv_result = driver::open(hid_device_info);

    // prefer early exists
    if (!drv_result) {
      // prefer {}
      return {};
    }

    auto drv = drv_result.value();
    // TODO: auto doesn't forward reference(&), so it may endup calling the copy constructor.
    // auto& profiles2 = std::visit( []() -> ProfileList& { return Profiles; } );
    // I can delete the copy constructor to make sure things are not copied by mistake, but i don't have to worry, because C++ will do the sensible thing
    // TODO: on the return it will move? how can i validate that it's actually what happens?
    // conseptually it's a 3-4 stage move(constructor), but in practice because it's return by value, it will probabbly construct in the destination
    auto profiles = std::visit([&](auto& d) { return d.load(); }, drv);

    return Device{drv, profiles};
  };

  ProfileList& profiles() {
    return profiles_;
  }

private:
  explicit Device(driver::DriverVariants driver, ProfileList profiles) : driver_(std::move(driver)), profiles_(std::move(profiles)) {} 

  driver::DriverVariants driver_;

  ProfileList profiles_;

  // std::vector<Profile>::iter active_profile; // erase or insert invalidates the iterator, so it's better to use some external index. we don't tend to hold iterator for long time
  // So how do i create an relationship between the profiles and that unsigned int called active_profile
  // Well the Device class create the relationship
  // TODO: create a strong type def https://www.justsoftwaresolutions.co.uk/cplusplus/strong_typedef.html
  //       https://github.com/PeterSommerlad/PSsst
  // unsigned int active_profile;
};


} // namespace lib

} // namespace ratbag
