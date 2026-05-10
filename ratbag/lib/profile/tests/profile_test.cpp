#include "ratbag/lib/profile/profile.hpp"

#include "gtest/gtest.h"

using ratbag::lib::Profile;

TEST(LedTestSuit, CreateNewProfile) {
  Profile prof;
  ASSERT_EQ(prof.leds().size(), 0);
}
