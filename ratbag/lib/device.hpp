#pragma once

#include "ratbag/lib/common/profile.hpp"

#include <vector>

namespace ratbag {
namespace lib {

class Device {
 public:
  static Device Open();

  void Read();

  void Write();

  std::vector<Profile> GetProfiles();
};
}  // namespace lib

}  // namespace ratbag
