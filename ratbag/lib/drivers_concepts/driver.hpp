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


// TODO(ask): Essentially concepts allow me to create a function that a accepts object with specific signature, and then iterate over the signature
//            But the concrete object has to be stored anyway, so i can't create inheritance as in a traditional term of base class and derived classes.             
// template<typename T>
// concept DriverLike = requires(hidapi::DeviceID &id) {
//     { T::probe(id) } -> std::same_as<bool>>;
//     t.commit();
// };

template<typename T>
concept DriverLike = requires(T t, hidapi::DeviceID id) {
    { T::probe(id) } -> std::same_as<bool>;
    t.commit();
};

using DriverVariants = std::variant<HIDPP20, SteelSeries>;

// TODO: is this how to use concepts? with static assert?
static_assert(
    DriverLike<HIDPP20> and
    DriverLike<SteelSeries>
); // OK


// template <DriverLike TDriverImpl> 
// struct Driver : TDriverImpl {};

// class Driver {
// public:
//   static std::optional<DriverVariants> open(hidapi::HIDDeviceInfo &hid_device_info);

// };



static std::optional<DriverVariants> open(hidapi::HIDDeviceInfo &hid_device_info) {

  // TODO(ask): i don't want to write these ifs by hand, how can i iterate over types and call their probe() funciton?
  //            what i want to have is an directory of drivers, and i want the "developer" to be able to simply add new "driver" type and have my "core" of the library to iterate over all the "driver type"s
  if (HIDPP20::probe(hid_device_info.device_id())) {
    // TODO(ask): what sort of sorcerry is this? how does C++ knows to convert HIDPP20 into std::optional<DriverVariants> ?
    return HIDPP20();    
  }

  if (SteelSeries::probe(hid_device_info.device_id())) {
    return SteelSeries();    
  }

  return {};
}

} // namespace drivers
} // namespace lib
} // namespace ratbag
