#include "ratbag/lib/common/led.hpp"

#include "gtest/gtest.h"

using ratbag::lib::Led;
using ratbag::lib::LedMode;

TEST(LedTestSuit, CreateNewLed) {
  Led someLed;
  ASSERT_TRUE((someLed.supported_modes() & LedMode::Off) == LedMode::Off);
  ASSERT_FALSE((someLed.supported_modes() & LedMode::Fixed) == LedMode::Fixed);
  ASSERT_FALSE((someLed.supported_modes() & LedMode::Cycle) == LedMode::Cycle);
  ASSERT_FALSE((someLed.supported_modes() & LedMode::Breathing) == LedMode::Breathing);
}
