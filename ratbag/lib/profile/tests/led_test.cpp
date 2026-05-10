#include "ratbag/lib/profile/led.hpp"

#include "gtest/gtest.h"

using ratbag::lib::Led;
using ratbag::lib::LedModeFlag;

TEST(LedTestSuit, CreateNewLed) {
  Led someLed;
  // TODO: i really would like to be able to just do: if (someLed.supported_modes() & LedMode::Off) {...}
  // yes, but it's maybe not a good idea.
  ASSERT_TRUE((someLed.supported_modes() & LedModeFlag::Off) == LedModeFlag::Off);
  ASSERT_FALSE((someLed.supported_modes() & LedModeFlag::Fixed) == LedModeFlag::Fixed);
  ASSERT_FALSE((someLed.supported_modes() & LedModeFlag::Cycle) == LedModeFlag::Cycle);
  ASSERT_FALSE((someLed.supported_modes() & LedModeFlag::Breathing) == LedModeFlag::Breathing);
  ASSERT_FALSE((someLed.supported_modes() & (LedModeFlag::Fixed | LedModeFlag::Cycle | LedModeFlag::Breathing)) == LedModeFlag::Breathing);
}
