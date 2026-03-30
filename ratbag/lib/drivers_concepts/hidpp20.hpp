#pragma once

namespace ratbag {
namespace lib {
namespace drivers_concepts {

// TODO(ask): with concepts it's not clear what interface this class implements by simplify looking at it.
//            There is also no enforcment of interface here 
class HIDPP20 final {
public:

  // TODO: instead of callling probe function, have a simple VID and PID support
  static bool probe(hidapi::DeviceID id) {
    if (id.vid() != 0x046d) {
      return false;
    }

    switch (id.pid()) {
    case 0xc08b: 
    case 0xc332: return true;
    default: return false;
    }
  }


  void commit() const {
    // TODO: open hid devices and read from it some name
  }
};

} // namespace drivers
} // namespace lib
} // namespace ratbag
