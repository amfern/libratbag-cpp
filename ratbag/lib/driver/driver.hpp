#pragma once

#include "ratbag/lib/driver/driver_like.hpp"
#include "ratbag/lib/driver/drivers/hidpp20/hidpp20.hpp"
#include "ratbag/lib/driver/drivers/steelseries/steelseries.hpp"
#include "ratbag/lib/driver/drivers/test_dummy/test_dummy.hpp"



#include <optional>
#include <variant>
#include <ranges>
#include <algorithm>

namespace ratbag {
namespace lib {
namespace driver {

using HIDPP20 = hidpp20::HIDPP20;
using SteelSeries = steelseries::SteelSeries;
using TestDummy = testdummy::TestDummy;

// TODO: read good books about tempates https://github.com/xuchen-tech/Books/blob/main/C%2B%2B%20Templates%20The%20Complete%20Guide%2C%202nd%20Edition%20%5BBooxRack%5D.pdf
// https://en.cppreference.com/cpp/language/variable_template
// read cpp templates cpp_templates(2)

// template <auto V> struct Derive { };
// template <auto V> inline constexpr Derive<V> derive;
// derive<Bitmask> = Derive<V>();

// TODO(ask): i really don't like changing the runtime just for the sake of being able to test... maybe there is a way to mock DriverVariants
//            maybe i can put the function into details, or provide the variants as an input, so i can create a separate test for it.
//            Like create a class that accepts DriverVariants as a template argument 
template <DriverLike... Ts>
using IDriver = std::variant<Ts...>;
using DriverVariants = IDriver<HIDPP20, SteelSeries, TestDummy>;

//  TODO(ask): should i put such factory functions under a class as static function or under a namespace?
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

  if (std::ranges::contains(TestDummy::supported_device_ids(),
                            hid_device_info.device_id())) {
    return TestDummy();
  };

  return {};
}

} // namespace drivers
} // namespace lib
} // namespace ratbag
