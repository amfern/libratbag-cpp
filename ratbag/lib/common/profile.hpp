#pragma once

#include "ratbag/lib/common/led.hpp"

namespace ratbag {
namespace lib {

class Profile {

public:
  const Led &led() const { return led_; };

private:
  // explicit Profile() {}

  Led led_;
};

// TODO(ask): how to represent multiple profiles, but one of them active, should
// i use
class ProfileList : std::vector<Profile> {

private:
  // explicit ProfileList() {}

  // Profile &active_profile;
  // int active_profile;
};


} // namespace lib
} // namespace ratbag
