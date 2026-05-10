# pragma once

#include <chrono>
#include <variant>
#include <vector>

#include "ratbag/lib/utils/bitmask_operators.hpp"

namespace ratbag {
namespace lib {

using LedIndex = unsigned int;
using ColorChannel = unsigned char;

struct Color {
  ColorChannel red;
  ColorChannel green;
  ColorChannel blue;
};

using ActionDuration = std::chrono::milliseconds;


struct Off {

};

struct FixedColor {
  Color color;
};

struct CycleColor {
  Color color;
  ActionDuration action_duration;
  ColorChannel brightness;
};

struct BreathingColor {
  Color color;
  ActionDuration action_duration;
  ColorChannel brightness;
};

using Mode = std::variant<Off, FixedColor, CycleColor, BreathingColor>;


// TODO: is there a way to automatically assign value like 1 << 2
  // in C++ 26 with refelection i can, in compile time that the values are all powers of two
// TODO: LedModeFlag is connected to the Mode variant above, how do i make sure adding Mode Flag also forces to add Variant type? static_assert?
//            Yes, with reflection. and even check that the names match. And in C++ 29 reflection can even generate code
enum class LedModeFlag {
  // led is now off
  Off = 0,

  // led is on with fixed color
  Fixed = 1 << 0,

  // led is cycling between all colors
  Cycle = 1 << 1,

  // led is pulsating with static color
  Breathing = 1 << 2,
  
  NewTyp = 1 << 3,
};

constexpr bool enable_bitmask_operators(LedModeFlag);

enum class LedColorDepth {
  /**
   * The device only supports a single color.
   * All color components should be set to 255.
   */
  MONOCHROME,
  // The device supports RBG color with 8 bits per color.
  RGB_888,
  // The device supports RBG colors with 1 bit per color.
  RGB_111,
};


class Led {

public:
  // Led(LedIndex &index, Mode& mode, supported): {

  // };

  LedIndex index() const {
    return index_;
  };

  // TODO: Perhaps use std::variant for the supported mode, and save values for all variants
  //       Or store an mirror representation in the device, and not try to be smart about things.
  //       should i design it, like an second twin, so the data in this library will match exactly the data storedin the HID(mouse) device?
  //       eg. when i create an empty new profile i imidiatly store it into the device. And then allow allow the user to change the colors, and modes..
  //           Or i create the profile, allow user to change the values, and then write to the device(the appraoch taken by current libratbag implementation)
  //       so instead of storing the LedMode we will store the variant with the correct, instead of enum and the variables outside... which means switching moes will lose their data. 
  //       Otherwise we store values for all modes, and just chose the correct one. 
  const Mode& mode() const {
    return mode_;
  };

  LedModeFlag supported_modes() const {
    return supported_modes_;
  };

  LedColorDepth supported_color_depth() const {
    return supported_color_depth_;
  };

private:
  LedIndex index_;

  Mode mode_;

  // TODO: maybe create something like LEDInfo, to store such meta-data about the LED
  LedModeFlag supported_modes_;

  // TODO: maybe create variation for colors struct, 
  LedColorDepth supported_color_depth_;
};

using LedList = std::vector<Led>;

} // namespace lib

} // namespace ratbag
