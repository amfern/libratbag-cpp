#pragma once

#include "ratbag/lib/drivers_dynamic/driver.hpp"

namespace ratbag {
namespace lib {
namespace drivers_dynamic {

class SteelSeries final : public IDriver {
public:
  // SteelSeries(const ratbag::lib::Device &device) : Driver(device) {}

  void probe() const override {
    // TODO: open hid devices and read from it some name
  }

  void commit() const override {
    // TODO: open hid devices and read from it some name
  }
};

} // namespace drivers
} // namespace lib
} // namespace ratbag
