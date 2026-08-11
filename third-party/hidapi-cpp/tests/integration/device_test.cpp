#include "hidapi/device.hpp"

#include <codecvt>
#include <algorithm>

#include "gtest/gtest.h"

using hidapi::HidBusType;
using hidapi::HIDDeviceInfo;
using hidapi::HIDDevice;
using hidapi::DeviceID;

// TODO: Run the test when the device is avaiable

// call the underlying API and compare the two...will work on all PC's
TEST(DeviceTestSuit, OpenDevice) {
  auto deviceInfos = HIDDeviceInfo::enumerate_hid_devices();

  auto it = std::ranges::find(deviceInfos, DeviceID{0x046d, 0xc332}, &HIDDeviceInfo::device_id);

  if (it == deviceInfos.end()) {
    GTEST_SKIP() << "Skipping this test because the device Logitech Gaming Mouse G502 not found.";
  }

  // TODO: what i really want is to let the HIDDevice open to pick into device info, not nessary to move it
  auto device = HIDDevice::open(std::move(*it));
}
