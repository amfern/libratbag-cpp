#include "ratbag/lib/common/led.hpp"

#include "gtest/gtest.h"

using ratbag::lib::Led;
using ratbag::lib::LedMode;

TEST(LedTestSuit, CreateNewLed) {
  Led someLed;
  // TODO(ask): i really would like to be able to just do: if (someLed.supported_modes() & LedMode::Off) {...}
  ASSERT_TRUE((someLed.supported_modes() & LedMode::Off) == LedMode::Off);
  ASSERT_FALSE((someLed.supported_modes() & LedMode::Fixed) == LedMode::Fixed);
  ASSERT_FALSE((someLed.supported_modes() & LedMode::Cycle) == LedMode::Cycle);
  ASSERT_FALSE((someLed.supported_modes() & LedMode::Breathing) == LedMode::Breathing);
  ASSERT_FALSE((someLed.supported_modes() & (LedMode::Fixed | LedMode::Cycle | LedMode::Breathing)) == LedMode::Breathing);
}
