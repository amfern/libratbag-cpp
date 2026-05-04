#pragma once

#include "ratbag/lib/common/led.hpp"
#include <optional>

namespace ratbag {
namespace lib {

class Profile {

public:
  const Led &led() const { return led_; };

private:
  // explicit Profile() {}

  Led led_;
};

// using ProfileList = std::vector<Profile>;
// int active_profile;

// TODO(ask): how to represent multiple profiles, but one of them active, should
class ProfileList {

private:
  // explicit ProfileList() {}

  // Profile profile1;
  // Profile profile2;
  // Use stack
  
  // std::stack<Profile> profiles;
  std::vector<Profile> profiles;
  // std::array<std::optional<Profile>, 5> profiles;
  // Profile &active_profile;
  // std::stack<Profile>::iter;
  // Profile* active_profile;

  // std::vector<Profile>::iter active_profile; // erase or insert invalidates the iterator, so it's better to use some external index. we don't tend to hold iterator for long time
  // using IteratorToProfile = unsigned int;
  // TODO: create a strong type def https://www.justsoftwaresolutions.co.uk/cplusplus/strong_typedef.html
  //       https://github.com/PeterSommerlad/PSsst
  // This active_profile is manged by ProfileList, so there is no need to do more checks.  
  unsigned int active_profile;
};


} // namespace lib
} // namespace ratbag
