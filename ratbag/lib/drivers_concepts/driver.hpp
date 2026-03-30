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

// TODO: i created the concept by it's not used anywhere...???
template<typename T>
concept DriverConcept = requires(T t) {
    t.probe();
    t.commit();
};

class Driver {
public:
  static std::optional<DriverVariants> open(hidapi::HIDDeviceInfo &hid_device_info);
};


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
