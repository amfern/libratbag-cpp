# pragma once


#include <stdint.h>  // uint32_t

#include <chrono>
#include <variant>
#include <vector>

#include "ratbag/lib/utils/bitmask_operators.hpp"

namespace ratbag {
namespace lib {
namespace led {

// using Index = unsigned int;
using ColorChannel = uint8_t;

// TODO: what is the difference between the two approaches
// typedef char myname[5]; // the typedef name is in the middle, so for more complex examples it's hard to understand what is being defined
// typedef std::array<std::uint8_t, 3> uint24_tttt; // C feature and has a C syntax, which is differ. First introduced in C
// using uint24_ttttt = std::array<std::uint8_t, 3>; // Using is easier to read, because it the name is i the middle. I should use using because it's new

// TODO: consider using a color library https://github.com/dmilos/color
union Color {
  using uint24_t = unsigned _BitInt(24);

  uint24_t hex;
  struct alignas(uint24_t) {
    ColorChannel red;
    ColorChannel green;
    ColorChannel blue;
  };

  // Default constructor
  Color(const uint24_t new_hex = 0XFFFFFF) : hex(new_hex) {}
  Color(ColorChannel new_red, ColorChannel new_green, ColorChannel new_blue)
      : red(new_red), green(new_green), blue(new_blue) {}

  Color &operator=(const uint24_t new_hex) {
    hex = new_hex;
    return *this;
  }

  // Compiler automatically generates equality logic for all members
  bool operator==(const uint24_t &other) const {
    return hex == other; 
  };
};

using ActionDuration = std::chrono::milliseconds;

struct ModeOff {

};

struct ModeFixedColor {
  Color color;
};

struct ModeCycleColor {
  Color color;
  ActionDuration action_duration;
  ColorChannel brightness;
};

struct ModeBreathingColor {
  Color color;
  ActionDuration action_duration;
  ColorChannel brightness;
};

using Mode = std::variant<ModeOff, ModeFixedColor, ModeCycleColor, ModeBreathingColor>;

// helper type for the visitor
template<class... Ts>
struct mode_visitor : Ts... { using Ts::operator()...; };

// TODO: is there a way to automatically assign value like 1 << 2
  // in C++ 26 with refelection i can, in compile time that the values are all powers of two
// TODO: LedModeFlag is connected to the Mode variant above, how do i make sure adding Mode Flag also forces to add Variant type? static_assert?
//            Yes, with reflection. and even check that the names match. And in C++ 29 reflection can even generate code
enum class ModeFlag {
  // led is now off
  Off = 0,

  // led is on with fixed color
  Fixed = 1 << 0,

  // led is cycling between all colors
  Cycle = 1 << 1,

  // led is pulsating with static color
  Breathing = 1 << 2,
};

constexpr bool enable_bitmask_operators(ModeFlag);

enum class ColorDepth {
  /**
   * The device only supports a single color.
   * All color components should be set to 255.
   */
  Monochrome,
  // The device supports RBG color with 8 bits per color.
  RGB_888,
  // The device supports RBG colors with 1 bit per color.
  RGB_111,
};

} // namespace led

class Led {

public:
  // TODO: maybe don't let define the number of lights, just let it be set to fixed number, same as the mouse and in the off state... or some other default state.

  // TODO: should i use use && in the arguments, or just normal value, and let the compiler do copy ellision?
  // this tell that i want to take only temporary objects, but that's not what i want.
  // avoiding uncessary copies is a good thing, but don't try to force it, because it makes the code harder to use.
  // From usiblity perspective it's better to allow people to copy thing, don't worry about the excesive copies, because most of them will go away.
  // people will discover that copying is slow, and will be able to fix it.
  // most of the time the performance is good enough whithout us doing anything special, and it's better to optimize for readiblity, usibility and correctness of the code. Optimize for the programmer.
  // and if it's easy to read, and then if there is a perf problem, only then optimize for performance.copying few 10 bytes object doesn't matter, but 10k objects can matter.
  // const & if i am going to processs it but not store it.
  Led(led::ModeFlag supported_modes = led::ModeFlag::Off, led::ColorDepth supported_color_depth = led::ColorDepth::Monochrome, led::Mode current_mode = led::ModeOff()) :
    supported_modes_(supported_modes), supported_color_depth_(supported_color_depth), mode_(std::move(current_mode)) {

    // TODO: i should rename it to light... instead of LED, led is light emitting diode which is too specific.
    const auto visitor = led::mode_visitor{
        [&](const led::ModeOff &m) {

        },
        [&](const led::ModeFixedColor &m) {
          if ((supported_modes_ & led::ModeFlag::Fixed) != led::ModeFlag::Fixed) {
            throw std::runtime_error("Fixed mode is not supported by this LED");
          }
        },
        [&](const led::ModeCycleColor &m) {
          if ((supported_modes_ & led::ModeFlag::Cycle) != led::ModeFlag::Cycle) {
            throw std::runtime_error("Cycle mode is not supported by this LED");
          }
        },
        [&](const led::ModeBreathingColor &m) {
          if ((supported_modes_ & led::ModeFlag::Breathing) != led::ModeFlag::Breathing) {
            throw std::runtime_error("Breathing mode is not supported by this LED");
          }
        },
    };

    std::visit(visitor, current_mode);
    
    // TODO: assert check if the mode is whithin the supported modes 
  };

  // // TODO: what do you think about this style of setter and getter, i wonder if i can just  do led.index = 11; 
  // led::Index index() const {
  //   return index_;
  // };

  // void set_index(led::Index index) {
  //   index_ = index;
  // }

  // TODO: Perhaps use std::variant for the supported mode, and save values for all variants
  //       Or store an mirror representation in the device, and not try to be smart about things.
  //       should i design it, like an second twin, so the data in this library will match exactly the data storedin the HID(mouse) device?
  //       eg. when i create an empty new profile i imidiatly store it into the device. And then allow allow the user to change the colors, and modes..
  //           Or i create the profile, allow user to change the values, and then write to the device(the appraoch taken by current libratbag implementation)
  //       so instead of storing the LedMode we will store the variant with the correct, instead of enum and the variables outside... which means switching moes will lose their data. 
  //       Otherwise we store values for all modes, and just chose the correct one. 
  const led::Mode& mode() const {
    return mode_;
  };

  void set_mode(led::Mode&& mode) {
    mode_ = std::move(mode);
  }

  led::ModeFlag supported_modes() const {
    return supported_modes_;
  };

  led::ColorDepth supported_color_depth() const {
    return supported_color_depth_;
  };

private:
  // led::Index index_;

  // TODO: maybe create something like LEDInfo, to store such meta-data about the LED
  led::ModeFlag supported_modes_;

  // TODO: maybe create variation for colors struct, 
  led::ColorDepth supported_color_depth_;

  led::Mode mode_;

};

using LedList = std::vector<Led>;

} // namespace lib

} // namespace ratbag
