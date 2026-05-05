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

  // TODO: how to prevent double assigning of hid api, so another driver by mistake uses the same DeviceID
  static constexpr hidapi::DeviceIDList supported_device_ids() {
    return {
      {0x046d, 0xc332},
      {0x046d, 0xc08b},
    }; 
  }

  ProfileList load() {
    // TODO: open hid devices and read from it some name
    // return true;
    return {};
  }

  // TODO(ask): i can pass by const &, but the nested memebers of ProfileList can still be accessed and modified?
  void commit(const ProfileList &profiles) const {
    // TODO: write profiles
  }
};

} // namespace drivers
} // namespace lib
} // namespace ratbag
