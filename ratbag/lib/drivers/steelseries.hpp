#pragma once

#include "ratbag/lib/drivers/driver.hpp"

namespace ratbag {
namespace lib {
namespace drivers {

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


















// class HIDPP20 : public ratbag::lib::Driver<HIDPP20> {
// public:
//   DriverName driver_name() const;
//   DriverID driver_id() const;
//   void commit() const;
//   void set_active_profile() const;

//   bool probe();

//   HIDPP20(std::string name) : name_(name) {}

// private:

// };

} // namespace drivers
} // namespace lib
} // namespace ratbag
