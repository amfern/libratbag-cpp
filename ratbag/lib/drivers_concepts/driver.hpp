#pragma once

#include "ratbag/lib/hidapi/device_info.hpp"

#include "ratbag/lib/drivers_concepts/steelseries.hpp"
#include "ratbag/lib/drivers_concepts/hidpp20.hpp"


#include <optional>
#include <variant>

namespace ratbag {
namespace lib {
namespace drivers_concepts {

// TODO(ask): How would i convert this driver to be
// concepts looks promising, but it's not exactly what i need
// CRTP (Curiously Recurring Template Pattern) looks closer to what i would like
// to have which zero cost polymorphisim, which is resolved in compile time.
// Altough i am not sure how to implement it

using DriverVariants = std::variant<HIDPP20, SteelSeries>;

// TODO(ask): i created the concept by it's not used anywhere...???
//       and deleting it works just as fine.
template<typename T>
concept DriverLike = requires(T t) {
    t.probe();
    t.commit();
};

static_assert(
    DriverLike<HIDPP20> and
    DriverLike<SteelSeries>
); // OK


template <DriverLike TDriverImpl> 
struct Driver : TDriverImpl {};

// class Driver {
// public:
//   static std::optional<DriverVariants> open(hidapi::HIDDeviceInfo &hid_device_info);

// };

static std::optional<DriverVariants> open(hidapi::HIDDeviceInfo &hid_device_info) {
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
    // TODO(ask): what happens here, how is it converted to  optional<DriverVariants>
    return HIDPP20();
    // TODO: any pretty way to do any_of to check if the pid() is any of
    //
    // the supported integers
    // DeviceMatch=usb:1038:1384;usb:1038:1392;usb:1038:1710;usb:1038:1712;usb:1038:171c;usb:1038:1394;usb:1038:171a;usb:1038:1716;usb:1038:1714;usb:1038:1718
  } else if (hid_device_info.device_id().vid() == 0x1038 &&
             (hid_device_info.device_id().pid() == 0x1384 ||
              hid_device_info.device_id().pid() == 0x1392)) {
    return SteelSeries();
  }

  return {};
}

template <DriverLike TDriver>
static void probe(TDriver& driver) {
  driver.probe();
}


// class Driver {

// public:
//   static const Driver open(hidapi::HIDDevice hid_device) {
//     using DriversTypes = std::variant<HIDPP20>;
//     std::vector<DriversTypes> drivers;

//     drivers.push_back(HIDPP20());
//     // shapes.push_back(Square(10));

//     for(const auto& driver : drivers) {
//         std::visit([&](auto& d) {
//           std::cout << d.name() << std::endl;
//           if (d.probe(hid_device)) {
//             return driver;
//           }
//         }, driver);
//     }

//     // TODO: maybe use optional? or not return const
//     return nullptr;
//   };
// };

// TODO: consider using concepts for polymorphisem
// https://ppatoria.hashnode.dev/demystifying-polymorphism-in-c
// https://medium.com/@sagar.necindia/cpp-dynamic-polymorphism-performance-tradeoffs-959d2e8564a0
// https://en.cppreference.com/w/cpp/language/constraints.html
// template <typename Derived>
// class Driver {
// public:
//   bool probe() const { return static_cast<const Derived *>(this)->probe(); }
//   DriverName driver_name() const { return static_cast<const Derived
//   *>(this)->driver_name(); } DriverID driver_id() const { return
//   static_cast<const Derived *>(this)->driver_id(); };

//   void commit() const { return static_cast<const Derived *>(this)->commit();
//   }; void set_active_profile()  const { return static_cast<const Derived
//   *>(this)->set_active_profile(); };

// private:
//   std::string name_;
//   std::string driver_id_;
// };

} // namespace drivers
} // namespace lib
} // namespace ratbag
