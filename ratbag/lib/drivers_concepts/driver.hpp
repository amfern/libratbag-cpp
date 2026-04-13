#pragma once

#include "ratbag/lib/hidapi/device_info.hpp"

#include "ratbag/lib/drivers_concepts/steelseries.hpp"
#include "ratbag/lib/drivers_concepts/hidpp20.hpp"


#include <optional>
#include <variant>
#include <ranges>
#include <algorithm>

namespace ratbag {
namespace lib {
namespace drivers_concepts {

template<class T>
concept DriverLike = requires(T t, hidapi::DeviceID id) {
    { T::supported_device_ids() } -> std::same_as<hidapi::DeviceIDList>;

    { T::load(id) } -> std::same_as<bool>;
    t.commit();
};

template <DriverLike... Ts>  
using IDriver = std::variant<Ts...>;

using DriverVariants = IDriver<HIDPP20, SteelSeries>;


static std::optional<DriverVariants> open(hidapi::HIDDeviceInfo &hid_device_info) {

  // TODO(ask): i don't want to write these ifs by hand, how can i iterate over types and call their probe() funciton?
  //            what i want to have is an directory of drivers, and i want the "developer" to be able to simply add new "driver" type and have my "core" of the library to iterate over all the "driver type"s
  //            it is possible to iterate over types by using template expansion
  //            try to stay whithin the limits of the language, and avoid resorting to doing things outside with the build system, like generation of some C++ files that hold array of each type
  //            look at the example by anthony typelists(1).cpp in this dir

  if (std::ranges::contains(HIDPP20::supported_device_ids(),
                            hid_device_info.device_id())) {
    // TODO(ask): what sort of sorcerry is this? how does C++ knows to convert HIDPP20 into std::optional<DriverVariants> ?
    return HIDPP20();
  };

  if (std::ranges::contains(SteelSeries::supported_device_ids(),
                            hid_device_info.device_id())) {
    // TODO(ask): what sort of sorcerry is this? how does C++ knows to convert HIDPP20 into std::optional<DriverVariants> ?    
    return SteelSeries();
  };

  return {};
}

} // namespace drivers
} // namespace lib
} // namespace ratbag
