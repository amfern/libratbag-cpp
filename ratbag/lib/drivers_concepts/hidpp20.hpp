#pragma once

#include "ratbag/lib/common/profile.hpp"
#include "ratbag/lib/hidapi/device_info.hpp"

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

  static bool load(ProfileList &profiles) {
    // TODO: open hid devices and read from it some name
    return true;
  }

  void commit(const ProfileList &profiles) const {
    // TODO: write profiles
  }
};

} // namespace drivers
} // namespace lib
} // namespace ratbag
