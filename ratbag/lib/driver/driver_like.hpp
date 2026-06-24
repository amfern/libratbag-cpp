#pragma once

#include "ratbag/lib/hidapi/device_info.hpp"
#include "ratbag/lib/profile/profile.hpp"

#include <optional>
#include <variant>
#include <ranges>
#include <algorithm>

namespace ratbag {
namespace lib {
namespace driver {

template <typename T>
concept DriverLike = requires(T t, const ProfileList& commit_profiles) {
  // TODO: how to throw compile error when same deviceId is already assigned to
  // another one How to make this variable unique accross all of the devices?
  // i would like it to fail on compile time when there is an overlap in the
  // device IDs between drivers... Maybe i can have an container of unique
  // things shared between all Drivers, and uppon header parsing, it will append
  // it's own DeviceID list in constexpr manner.
  // TODO: convert to unique key hash map
  // static const std::array devices_ = {
  //   Device(ratbag::lib::hidapi::DeviceID{0x046d, 0xc08b},
  //   ratbag::lib::drivers::HIDPP20);
  // };

  { T::supported_device_ids() } -> std::same_as<hidapi::DeviceIDList>;

  { t.load() } -> std::same_as<ProfileList>;
  t.commit(commit_profiles);
};  

} // namespace drivers
} // namespace lib
} // namespace ratbag
