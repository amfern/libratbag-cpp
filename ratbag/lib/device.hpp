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
#include "ratbag/lib/drivers/hidpp20.hpp"
#include "ratbag/lib/drivers/steelseries.hpp"
#include "ratbag/lib/hidapi/device.hpp"
#include "ratbag/lib/hidapi/device_info.hpp"

namespace ratbag {
namespace lib {

// template<typename T>
// concept DeviceInterface = requires(T t) {
//     { t.open() } -> std::same_as<bool>;
//     // { t.name() } -> std::same_as<DriverName>;
//     // { t.id() } -> std::same_as<DriverID>;
// };

// using LogitechG502Hero = Device<ratbag::lib::hidapi::DeviceID{0x046d,
// 0xc08b}, ratbag::lib::drivers::HIDPP20 _device_id>;

class Device {

public:
  // TODO(ask): i want to resolve it in compile time instead of returning
  // optional Can't because it's i don't know what the hid_device_info during
  // runtime
  static std::optional<Device> open(hidapi::HIDDeviceInfo &hid_device_info) {
    std::unique_ptr<ratbag::lib::drivers::Driver> drv_ptr;

    // ratbag::lib::drivers::Driver driver;

    // TODO: i really don't want to write if else for every driver, i wonder if
    // there is a compile time way to iterate over the drivers
    if (hid_device_info.device_id().vid() == 0x046d &&
        hid_device_info.device_id().pid() == 0xc08b) {
      auto hid_device = ratbag::lib::hidapi::HIDDevice::open(hid_device_info);
      // auto driver = ratbag::lib::drivers::HIDPP20(hid_device);
      auto driver = ratbag::lib::drivers::HIDPP20();
      return Device(hid_device_info, driver);
      if hid_device_info
        .device_id().pid() in[] std::ranges::any_of(
            {0x1384, 0x1392},
            [&](uint_t &id) { return id == hid_device_info.device_id().pid() })
            std::ranges::contains({0x1384, 0x1392},
                                  hid_device_info.device_id().pid());
    } else if (hid_device_info.device_id().vid() == 0x1038 &&
               (hid_device_info.device_id().pid() == 0x1384 ||
                hid_device_info.device_id().pid() == 0x1392)) {
      // TODO(ask): any pretty way to do any_of to check if the pid() is any of
      // the supported integers
      // DeviceMatch=usb:1038:1384;usb:1038:1392;usb:1038:1710;usb:1038:1712;usb:1038:171c;usb:1038:1394;usb:1038:171a;usb:1038:1716;usb:1038:1714;usb:1038:1718
      auto hid_device = ratbag::lib::hidapi::HIDDevice::open(hid_device_info);
      // auto driver = ratbag::lib::drivers::SteelSeries(hid_device);
      driver = ratbag::lib::drivers::SteelSeries();
      return Device(hid_device_info, driver);
    } else {
      return std::nullopt;
    }

    return Device(hid_device_info, driver);

    // auto devices = std::vector({});
    // // TODO: iterate over all available DeviceId and create the appropriate
    // one for (auto &device : devices_) {
    //   if (device.device_id == hid_device_info.device_id()) {
    //     return device.;
    //   }
    // }
  };

private:
  // TODO: how to throw compile error when same deviceId is already assigned to
  // another one How to make this variable unique acrsso all of the devices?
  // TODO: convert to unique key hash map
  // static const std::array devices_ = {
  //   Device(ratbag::lib::hidapi::DeviceID{0x046d, 0xc08b},
  //   ratbag::lib::drivers::HIDPP20);
  // };

  explicit Device(ratbag::lib::hidapi::HIDDevice &hid_device);

  ratbag::lib::hidapi::DeviceID &device_id_;
  // With dynamic polymorphsim i have to create a pointer and allocate on the
  // heap, because i don't know the actual size of the object with variant the
  // biggest size among all of the objects will be allocated, which would allow
  // me to allocate on the stack, with a bit of wasted memory. what is better
  // runtime overheard of pointers, or std::variant. If i try to convert
  // everythiing to a tempalte, i am just pushing the problem to the upper
  // layer, which would have to be aware of the template.
  std::Variant<ratbag::lib::drivers::HIDPP20, ratbag::lib::drivers::SteelSeries>
      driver_;
  ratbag::lib::drivers::Driver driver_;
};

} // namespace lib

} // namespace ratbag
