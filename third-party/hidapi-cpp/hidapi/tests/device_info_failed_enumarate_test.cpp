#include "hidapi/device_info.hpp"
#include <codecvt>

#include "gtest/gtest.h"
#include <concepts>
#include <type_traits>

TEST(DeviceInfoTest, FailedToEnumarate) {
    try {
        auto deviceInfos = hidapi::HIDDeviceInfo::enumerate_hid_devices();
        FAIL() << "Expected std::runtime_error was not thrown";
    } catch (const std::runtime_error& err) {
        EXPECT_STREQ("Failed to enumarate HID devices: Couldn't create udev context", err.what());
    } catch (...) {
        FAIL() << "Expected std::runtime_error, but a different exception was thrown";
    }
}
