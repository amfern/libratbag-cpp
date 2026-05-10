#pragma once

#include "ratbag/lib/profile/led.hpp"
#include <optional>

namespace ratbag {
namespace lib {


class Profile {

public:
  // Profile(LedList &led) : leds_(led) { }

  const LedList &leds() const { return leds_; };
  

private:
  // explicit Profile() {}

  LedList leds_;
};

using ProfileList = std::vector<Profile>;

} // namespace lib
} // namespace ratbag
