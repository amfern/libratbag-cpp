#pragma once

#include "ratbag/lib/driver/driver_like.hpp"
#include "ratbag/lib/profile/profile.hpp"
#include "ratbag/lib/hidapi/device_info.hpp"

namespace ratbag {
namespace lib {
namespace driver {
namespace testdummy {

class TestDummy final {
public:

  static constexpr hidapi::DeviceIDList supported_device_ids() {
    return {
      {0x1234, 0x4321},
    }; 
  }

  ProfileList load() {
    return {
      {},
    };
  }

  void commit(const ProfileList &profiles) const {
    // TODO: open hid devices and read from it some name
  }
};

static_assert(DriverLike<TestDummy>);

}
} // namespace drivers
} // namespace lib
} // namespace ratbag
