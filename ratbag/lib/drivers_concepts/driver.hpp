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

template <class T>
concept DriverLike = requires(T t, ProfileList& load_profiles, const ProfileList& commit_profiles) {
  // TODO: how to throw compile error when same deviceId is already assigned to
  // another one How to make this variable unique accross all of the devices?
  // i would like it to fail on compile time when there is an overlap in the
  // device IDs between drivers... Maybe i can have an container of unique
  // things shared between all Drivers, and uppon header parsing, it will append
  // it's own DeviceID list in constexpr manner.
  // TODO: convert to unique key hash map
  // static const std::array devices_ = {
  //   Device(ratbag::lib::hidapi::DeviceID{0x046d, 0xc08b},
  //   ratbag::lib::drivers::HIDPP20);
  // };

  { T::supported_device_ids() } -> std::same_as<hidapi::DeviceIDList>;

  { T::load(load_profiles) } -> std::same_as<bool>;
  t.commit(commit_profiles);
};

template <DriverLike... Ts>  
using IDriver = std::variant<Ts...>;

using DriverVariants = IDriver<HIDPP20, SteelSeries>;

static std::optional<DriverVariants> open(hidapi::HIDDeviceInfo &hid_device_info) {

  // TODO: i don't want to write these ifs by hand, how can i iterate over types and call their probe() funciton?
  //       what i want to have is an directory of drivers, and i want the "developer" to be able to simply add new "driver" type and have my "core" of the library to iterate over all the "driver type"s
  //       it is possible to iterate over types by using template expansion
  //       yes, it's possible by using tempalte argument to cut off the first value and let the compiler create nested recursive callbacks and then it will collabpse it into a simple if statments during the optimization phase 
  //       look at the example by anthony typelists(1).cpp in this dir
  //       It's also possile to generate with a build system, but try to stay whithin the confines of the language, and avoid resorting to doing things outside of the language with the build system, like generation of some C++ files that hold array of each type

  // TODO: i use ranges contains, is this what recomneded in C++23? there is also alternatives like find
  //            yes
  if (std::ranges::contains(HIDPP20::supported_device_ids(),
                            hid_device_info.device_id())) {

    // TODO: what sort of sorcerry is this? how does C++ knows to convert HIDPP20 into std::optional<DriverVariants> ?
    //            1. convert to return type if it can, eg float to int, int to float
    //            2. srd::optional has constructor of argumnets that can construct the DriverVariants type
    //             std::optional constructor -> DriverVariants constructor -> call the hidpp20 move constructor
    //           first it will construct temporary and then call the move oncstructor of hidpp20, through the constructor of DriverVariants and then through std::optional
    
    return HIDPP20();
  };

  if (std::ranges::contains(SteelSeries::supported_device_ids(),
                            hid_device_info.device_id())) {
    return SteelSeries();
  };

  return {};
}

} // namespace drivers
} // namespace lib
} // namespace ratbag
