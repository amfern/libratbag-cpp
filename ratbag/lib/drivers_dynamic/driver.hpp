#pragma once

#include "ratbag/lib/hidapi/device_info.hpp"
#include <string>
#include <string_view>
#include <memory>

namespace ratbag {
namespace lib {
namespace drivers_dynamic {

class IDriver;

using Driver = std::unique_ptr<IDriver>;

class IDriver {
public:
  static Driver open(hidapi::HIDDeviceInfo &hid_device_info);

  // Callback called while trying to open a device by libratbag.
  // This function should decide whether or not this driver will
  // handle the given device.
  // TODO: why would i probe device, if i know ahead of time what driver to use
  // based on vid, pid If a matching driver is found it should initialize itself
  // and synchronize all profiles with the current state on the device
  // in this callback.
  virtual void probe() const = 0;

  // Callback called while trying to open a device by libratbag.
  // This function should decide whether or not this driver will
  // handle the given device.
  // TODO: throw exception if commit failed?
  virtual void commit() const = 0;

  // Callback called while trying to open a device by libratbag.
  // This function should decide whether or not this driver will
  // handle the given device.
  // TODO: the setting of active profile should be called as part of the ?
  // set_active_profile()

  // private:
  //   const ratbag::lib::Device &device_;

  // TODO(ask): For pure interface i also have to define the virtual destructore, so the derived class destructor can be called.
  //            and if i add virtual destructor here, should i also follow the rule of 3?
  virtual ~IDriver() = default;
};

} // namespace drivers
} // namespace lib
} // namespace ratbag
