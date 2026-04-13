#include <chrono>

namespace ratbag {
namespace lib {

using LedIndex = unsigned int;

enum class LedMode {
  // led is now off
  Off = 0,

  // led is on with fixed color
  Fixed,

  // led is cycling between all colors
  Cycle,

  // led is pulsating with static color
  Breathing,

  Count,
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

// TODO(ask): what do you think about this class to hold flags of supported modes?
struct SupportedModes : std::bitset<static_cast<std::size_t>(LedMode::Count)> {
  const bool is_off_supported() const {
    return test(static_cast<std::size_t>(LedMode::Off));
  }

  const bool is_fixed_supported() const {
    return test(static_cast<std::size_t>(LedMode::Fixed));
  }

  const bool is_cycle_supported() const {
    return test(static_cast<std::size_t>(LedMode::Cycle));
  }

  const bool is_breathing_supported() const {
    return test(static_cast<std::size_t>(LedMode::Breathing));
  }
};

using ColorChannel = unsigned int;

struct Color {
  ColorChannel red;
  ColorChannel green;
  ColorChannel blue;
};

using Brightness = ColorChannel;

// TODO(ask): should i sue std::chrono::duration<int29, std::milli>;   ?
//            i don't expect to put large numbers (maybe it's also a good ooportunity to use contrats)
using ActionDuration = std::chrono::milliseconds;

class Led {

public:
  LedIndex index() const {
    return index_;
  };

  const Color& color() const {
    return color_;
  };

  // TODO(ask): Perhaps use std::variant for the supported mode, and save values for all variants
  //       Or store an mirror representation in the device, and not try to be smart about things.
  //       should i design it, like an second twin, so the data in this library will match exactly the data storedin the HID(mouse) device?
  //       eg. when i create an empty new profile i imidiatly store it into the device. And then allow allow the user to change the colors, and modes..
  //           Or i create the profile, allow user to change the values, and then write to the device(the appraoch taken by current libratbag implementation)
  LedMode current_mode() const {
    return current_mode_;
  };

  SupportedModes supported_modes() const {
    return supported_modes_;
  };

  LedColorDepth supported_color_depth() const {
    return supported_color_depth_;
  };

  ActionDuration action_duration() const {
    return action_duration_;
  };

  Brightness brightness() const {
    return brightness_;
  };

private:
  LedIndex index_;
  Color color_;
  LedMode current_mode_;
  
  SupportedModes supported_modes_;

  LedColorDepth supported_color_depth_;

  ActionDuration action_duration_;

  Brightness brightness_;
};

} // namespace lib

} // namespace ratbag
