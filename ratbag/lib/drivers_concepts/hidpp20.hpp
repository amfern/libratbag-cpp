#pragma once

#include "ratbag/lib/drivers_concepts/driver_like.hpp"
#include "ratbag/lib/common/profile.hpp"
#include "ratbag/lib/hidapi/device_info.hpp"

namespace ratbag {
namespace lib {
namespace drivers_concepts {

// This class implements the DriverLike concept
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

  // TODO: i can pass by const &, but the nested memebers of ProfileList can still be accessed and modified?
  // const propogates to the memebers, and even for vector memebres because of the way it's implement.
  // unless there is a pointer inside that can be derefenced
  void commit(const ProfileList &profiles) const {
    // TODO: write profiles
  }
};

static_assert(DriverLike<HIDPP20>);

} // namespace drivers
} // namespace lib
} // namespace ratbag
