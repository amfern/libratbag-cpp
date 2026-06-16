#include "ratbag/lib/drivers_dynamic/driver.hpp"
#include "ratbag/lib/drivers_dynamic/steelseries.hpp"
#include "ratbag/lib/drivers_dynamic/hidpp20.hpp"
#include "ratbag/lib/hidapi/device_info.hpp"

#include <string>
#include <string_view>
#include <memory>

namespace ratbag {
namespace lib {
namespace drivers_dynamic {


// TODO: in C++ polymorphic inheritance is achieved through pointers, which means we are forced to allocate object via new keyword(or make_shared)
//       Which doesn't work with other aspects of C++ like std::optional, so i haveto use nullptr instead of and rely on the fact that unique can be nullptr
//       it feels to me like inheritance and std::optional don't play well togather.
//       in C++ i have these nice abstraction over memory management, but C++ doesn't let me forget for a second that there is still memory bellow the abstractions
Driver IDriver::open(hidapi::HIDDeviceInfo &hid_device_info) {
  // // TODO: i really don't want to write if else for every driver, i wonder if
  // // there is a compile time way to iterate over the drivers
  //   // if hid_device_info
  //   //   .device_id().pid() in[] std::ranges::any_of(
  //   //       {0x1384, 0x1392},
  //   //       [&](uint_t &id) { return id == hid_device_info.device_id().pid()
  //   })
  //   //       std::ranges::contains({0x1384, 0x1392},
  //   //                             hid_device_info.device_id().pid());

  if (hid_device_info.device_id().vid() == 0x046d &&
      (hid_device_info.device_id().pid() == 0xc08b ||
       hid_device_info.device_id().pid() == 0xc332)) {
    // auto hid_device =
    // ratbag::lib::hidapi::HIDDevice::open(hid_device_info);

    // auto driver = ratbag::lib::drivers::HIDPP20(hid_device);
    return std::make_unique<HIDPP20>();
    // TODO: any pretty way to do any_of to check if the pid() is any of
    //
    // the supported integers
    // DeviceMatch=usb:1038:1384;usb:1038:1392;usb:1038:1710;usb:1038:1712;usb:1038:171c;usb:1038:1394;usb:1038:171a;usb:1038:1716;usb:1038:1714;usb:1038:1718
  } else if (hid_device_info.device_id().vid() == 0x1038 &&
             (hid_device_info.device_id().pid() == 0x1384 ||
              hid_device_info.device_id().pid() == 0x1392)) {
    return std::make_unique<SteelSeries>();
  }

  return Driver{};
}

} // namespace drivers
} // namespace lib
} // namespace ratbag
