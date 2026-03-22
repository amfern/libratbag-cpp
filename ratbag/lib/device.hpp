#pragma once

#include <vector>

#include "ratbag/lib/common/profile.hpp"

namespace ratbag {
namespace lib {

class Device;

using DeviceName = std::string_view;
using DeviceList = std::vector<Device>;

class Device {
public:
  
  DeviceName device_name();

  static DeviceList list_devices() {
    // TODO: arene_base type list to iterate over types,
    //       type list is not part of C++ standart, maybe use tuple 
    for ()
    // TODO: for each driver, call the static probe function to check for availale devices
    //       each driver can support multiple device models
    
  }

private:
  Driver _driver;

  // void Read();

  // void Write();

  // TOOD: device has profiles -> profile has buttons, led, sensor
  //       we will start with LEDs
  std::vector<Profile> GetProfiles();
};
} // namespace lib

} // namespace ratbag
