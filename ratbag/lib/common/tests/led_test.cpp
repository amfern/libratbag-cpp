#include "ratbag/lib/common/led.hpp"

#include "gtest/gtest.h"

using ratbag::lib::Led;
using ratbag::lib::LedMode;

TEST(LedTestSuit, CreateNewLed) {
  Led someLed;
  ASSERT_TRUE((someLed.supported_modes() & LedMode::Off) == LedMode::Off);
}
