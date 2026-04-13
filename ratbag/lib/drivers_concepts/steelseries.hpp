#pragma once

namespace ratbag {
namespace lib {
namespace drivers_concepts {

class SteelSeries final {
public:

  static constexpr hidapi::DeviceIDList supported_device_ids() {
    return {
      {0x1038, 0x1384},
      {0x1038, 0x1392},
    }; 
  }

  // static bool probe(hidapi::DeviceID id) {
  //   if (id.vid() != 0x1038) {
  //     return false;
  //   }

  //   switch (id.pid()) {
  //   case 0x1384: 
  //   case 0x1392: return true;
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
