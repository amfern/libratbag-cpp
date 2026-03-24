#pragma once

#include "ratbag/lib/hidapi/device_info.hpp"
#include <string>
#include <string_view>

namespace ratbag {
namespace lib {
namespace drivers {

// TODO(ask): How would i convert this driver to be
// concepts looks promising, but it's not exactly what i need
// CRTP (Curiously Recurring Template Pattern) looks closer to what i would like
// to have which zero cost polymorphisim, which is resolved in compile time.
// Altough i am not sure how to implement it

class Driver {
  // Driver(const ratbag::lib::Device &device) : device_(device) {

  // }

  // Callback called while trying to open a device by libratbag.
  // This function should decide whether or not this driver will
  // handle the given device.
  // TODO: why would i probe device, if i know ahead of time what driver to use
  // based on vid, pid If a matching driver is found it should initialize itself
  // and synchronize all profiles with the current state on the device
  // in this callback.
  virtual void probe() const = 0;

  // Callback called while trying to open a device by libratbag.
  // This function should decide whether or not this driver will
  // handle the given device.
  virtual void commit() const = 0;

  // Callback called while trying to open a device by libratbag.
  // This function should decide whether or not this driver will
  // handle the given device.
  // TODO: the setting of active profile should be called as part of the ?
  // set_active_profile()

  // private:
  //   const ratbag::lib::Device &device_;
};

// using DriverName = std::string_view;
// using DriverID = std::string_view;

// template<typename T>
// concept DriverInterface = requires(T t) {
//     { t.probe() } -> std::same_as<bool>;
//     // { t.name() } -> std::same_as<DriverName>;
//     // { t.id() } -> std::same_as<DriverID>;
// };

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
//   // TODO: throw exception if commit failed?
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
