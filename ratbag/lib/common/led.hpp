# pragma once

#include <chrono>
#include <variant>

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


enum class LedMode {
  // led is now off
  Off = 0,

  // led is on with fixed color
  Fixed = 1 << 0,

  // led is cycling between all colors
  Cycle = 1 << 1,

  // led is pulsating with static color
  Breathing = 1 << 2,
};

enum class LedColorDepth {
  /**
   * The device only supports a single color.
   * All color components should be set to 255.
   */
  MONOCHROME = 0,
  // The device supports RBG color with 8 bits per color.
  RGB_888,
  // The device supports RBG colors with 1 bit per color.
  RGB_111,
};


class Led {

public:
  LedIndex index() const {
    return index_;
  };

  // TODO(ask): Perhaps use std::variant for the supported mode, and save values for all variants
  //       Or store an mirror representation in the device, and not try to be smart about things.
  //       should i design it, like an second twin, so the data in this library will match exactly the data storedin the HID(mouse) device?
  //       eg. when i create an empty new profile i imidiatly store it into the device. And then allow allow the user to change the colors, and modes..
  //           Or i create the profile, allow user to change the values, and then write to the device(the appraoch taken by current libratbag implementation)
  //       so instead of storing the LedMode we will store the variant with the correct, instead of enum and the variables outside... which means switching moes will lose their data. 
  //       Otherwise we store values for all modes, and just chose the correct one. 
  const Mode& mode() const {
    return mode_;
  };

  LedMode supported_modes() const {
    return supported_modes_;
  };

  LedColorDepth supported_color_depth() const {
    return supported_color_depth_;
  };

private:
  LedIndex index_;

  Mode mode_;

  // TODO: maybe create something like LEDInfo, to store such meta-data about the LED
  LedMode supported_modes_;

  // TODO: maybe create variation for colors struct, 
  LedColorDepth supported_color_depth_;
};

constexpr bool enable_bitmask_operators(LedMode);
constexpr bool enable_bitmask_operators(LedColorDepth);

} // namespace lib

} // namespace ratbag

// template<>
// struct enable_bitmask_operators<ratbag::lib::LedMode>{
//     static const bool enable=true;
// };


// template<>
// struct enable_bitmask_operators<ratbag::lib::LedColorDepth>{
//     static const bool enable=true;
// };
