#pragma once

namespace ratbag {
namespace lib {
namespace drivers_concepts {

// This class implements the DriverLike concept
// TODO(ask): Is there a n C++ language way to say: "This class must satisfy the DriverLike concept concept"
class HIDPP20 final {
public:

  static constexpr hidapi::DeviceIDList supported_device_ids() {
    return {
      {0x046d, 0xc332},
      {0x046d, 0xc08b},
    }; 
  }

  // TODO: instead of callling probe function, have a simple VID and PID support
  // static bool probe(hidapi::DeviceID id) {
  //   if (id.vid() != 0x046d) {
  //     return false;
  //   }

  //   switch (id.pid()) {
  //   case 0xc08b: 
  //   case 0xc332: return true;
  //   default: return false;
  //   }
  // }

  void commit() const {
    // TODO: open hid devices and read from it some name
  }
};

} // namespace drivers
} // namespace lib
} // namespace ratbag
