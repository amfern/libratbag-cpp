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

#include "ratbag/lib/drivers/driver.hpp"
#include "ratbag/lib/drivers_concepts/driver.hpp"
#include "ratbag/lib/hidapi/device.hpp"
#include "ratbag/lib/hidapi/device_info.hpp"

namespace ratbag {
namespace lib {

// class Device {

// public:
//   // TODO(ask): i want to resolve it in compile time instead of returning
//   // optional Can't because it's i don't know what the hid_device_info size during
//   // runtime
//   // TODO: maybe use exception instead of optional?
//   static std::optional<Device> open(hidapi::HIDDeviceInfo &hid_device_info) {
//     // // TODO: this is not really pretty that i have IDriver::open on the interface and not the Device class
//     auto drv = drivers::IDriver::open(hid_device_info);

    
//    if (drv) {
//      // TODO(ask): what happens here
//      // how does the Device is converted to std::optional<Device> ?
//      auto drv->probe();
//      return Device{std::move(drv)};
//    }
    
//     return std::nullopt;
//   };

// private:
//   // TODO: how to throw compile error when same deviceId is already assigned to
//   // another one How to make this variable unique acrsso all of the devices?
//   // TODO: convert to unique key hash map
//   // static const std::array devices_ = {
//   //   Device(ratbag::lib::hidapi::DeviceID{0x046d, 0xc08b},
//   //   ratbag::lib::drivers::HIDPP20);
//   // };

//   explicit Device(drivers::Driver driver) : driver_(std::move(driver)) {} 

//   // ratbag::lib::hidapi::DeviceID &device_id_;
//   // With dynamic polymorphsim i have to create a pointer and allocate on the
//   // heap, because i don't know the actual size of the object with variant the
//   // biggest size among all of the objects will be allocated, which would allow
//   // me to allocate on the stack, with a bit of wasted memory. what is better
//   // runtime overheard of pointers, or std::variant. If i try to convert
//   // everythiing to a tempalte, i am just pushing the problem to the upper
//   // layer, which would have to be aware of the template.
//   // std::Variant<ratbag::lib::drivers::HIDPP20, ratbag::lib::drivers::SteelSeries>
//   //     driver_;
//   drivers::Driver driver_;
// };


class Device {

public:
  // TODO: maybe use exception instead of optional?
  static std::optional<Device> open(hidapi::HIDDeviceInfo &hid_device_info) {

    auto drv_result = drivers_concepts::Driver::open(hid_device_info);

    // TODO(ask): i like to do these early exists to avoid long nested code in the if clasue, what do you thin about?
    if (!drv_result) {
      return {}; // TODO(ask): {} vs std::nullopt what syntax is prefered here?
    }


    auto drv = drv_result.value();
    // TODO(ask): what solution has less pointer manipulation, the concepts or the dynamic inheritance?
    //            I see that with std::variant i have to call std::visit with anonymouse funciton, which is basically a pointer?
    std::visit( [&](auto& d) { d.probe(); }, drv);

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

  explicit Device(drivers_concepts::DriverVariants driver) : driver_(std::move(driver)) {} 

  drivers_concepts::DriverVariants driver_;
};

} // namespace lib

} // namespace ratbag
