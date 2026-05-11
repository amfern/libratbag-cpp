#include "ratbag/lib/profile/led.hpp"

#include "gtest/gtest.h"

// TODO(ask): should i match the namespace to directory structure?
// TODO(ask): what do you think about this type of nesting, where all Led related stuff are nested under  ratbag::lib::led namespce, but the class nested udner ratbag::lib::Led, and the whole files under /lib/profile
using ratbag::lib::Led;
using namespace ratbag::lib::led;
using namespace std::chrono_literals; 

TEST(LedTestSuit, CreateNewLed) {
  Led someLed;
  // TODO: i really would like to be able to just do: if (someLed.supported_modes() & LedMode::Off) {...}
  // yes, but it's maybe not a good idea.
  // ASSERT_EQ(someLed.index(), 0);

  ASSERT_TRUE((someLed.supported_modes() & ModeFlag::Off) == ModeFlag::Off);
  ASSERT_FALSE((someLed.supported_modes() & ModeFlag::Fixed) == ModeFlag::Fixed);
  ASSERT_FALSE((someLed.supported_modes() & ModeFlag::Cycle) == ModeFlag::Cycle);
  ASSERT_FALSE((someLed.supported_modes() & ModeFlag::Breathing) == ModeFlag::Breathing);
  ASSERT_FALSE((someLed.supported_modes() & (ModeFlag::Fixed | ModeFlag::Cycle | ModeFlag::Breathing)) == ModeFlag::Breathing);

  ASSERT_TRUE(std::holds_alternative<ModeOff>(someLed.mode()));

  ASSERT_TRUE(someLed.supported_color_depth() == ColorDepth::Monochrome);
}

TEST(LedTestSuit, SetLedValues) {
  Led someLed;

  // someLed.set_index(22);
  // ASSERT_EQ(someLed.index(), 22);

  // TODO: how to indicate that this 
  ModeFixedColor fixedMode{
    .color = {252, 253, 254},
  };
  someLed.set_mode(fixedMode);

  const auto visitor = mode_visitor {
    [](auto& d) { FAIL() << "expected ModeFixedColor";  },
    [](const ModeFixedColor &fc) {
      ASSERT_EQ(fc.color, 0xFEFDFC);
    },
  };

  std::visit(visitor, someLed.mode());
}

TEST(LedTestSuit, LedConstructor) {
  Led someLed{ModeFlag::Fixed | ModeFlag::Cycle, ColorDepth::RGB_888,
              ModeCycleColor{
                  .color = {0xFEFDFC},
                  .action_duration = 10ms,
                  .brightness = 245,
              }};

  ASSERT_TRUE((someLed.supported_modes() & ModeFlag::Cycle) == ModeFlag::Cycle);
  ASSERT_EQ(someLed.supported_modes(), ModeFlag::Fixed | ModeFlag::Cycle);
  ASSERT_TRUE(someLed.supported_color_depth() == ColorDepth::RGB_888);

  const auto visitor = mode_visitor {
    [](auto& d) { FAIL() << "expected ModeFixedColor";  },
    [](const ModeCycleColor &m) {
      ASSERT_EQ(m.color.red, 252);
      ASSERT_EQ(m.color.green, 253);
      ASSERT_EQ(m.color.blue, 254);
      ASSERT_EQ(m.action_duration, 10ms);
      ASSERT_EQ(m.brightness, 245);
    },
  };

  std::visit(visitor, someLed.mode());
}

TEST(LedTestSuit, LedConstructorFailureTest) {
  EXPECT_THROW(Led({ModeFlag::Off, ColorDepth::RGB_888, ModeFixedColor{}}),
               std::runtime_error);

  EXPECT_THROW(Led({ModeFlag::Fixed, ColorDepth::RGB_888, ModeCycleColor{}}),
               std::runtime_error);

  EXPECT_THROW(Led({ModeFlag::Off, ColorDepth::RGB_888, ModeBreathingColor{}}),
               std::runtime_error);
}
