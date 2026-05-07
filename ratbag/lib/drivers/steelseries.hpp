#pragma once

#include "ratbag/lib/drivers/driver_like.hpp"
#include "ratbag/lib/common/profile.hpp"
#include "ratbag/lib/hidapi/device_info.hpp"

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

  ProfileList load() {
    return {}; 
  }

  void commit(const ProfileList &profiles) const {
    // TODO: open hid devices and read from it some name
  }
};

static_assert(DriverLike<SteelSeries>);

} // namespace drivers
} // namespace lib
} // namespace ratbag
