#pragma once

namespace ratbag {
namespace lib {
namespace drivers_concepts {

class HIDPP20 final {
public:
  // HIDPP20(const ratbag::lib::Device &device) : Driver(device) {}

  void probe() const {
    // TODO: open hid devices and read from it some name
  }

  void commit() const {
    // TODO: open hid devices and read from it some name
  }
};

} // namespace drivers
} // namespace lib
} // namespace ratbag
