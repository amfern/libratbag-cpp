#include "ratbag/lib/hidapi/device.hpp"

#include "gtest/gtest.h"

#include <ranges>
#include <algorithm>

#include <codecvt>
#include <cstddef>

using ratbag::lib::hidapi::HidBusType;
using ratbag::lib::hidapi::HIDDeviceInfo;
using ratbag::lib::hidapi::HIDDevice;
using ratbag::lib::hidapi::HIDBuffer;
using ratbag::lib::hidapi::HIDReport;
using ratbag::lib::hidapi::ReportID;


TEST(DeviceTestSuit, DeviceWrite) {
  auto deviceInfos = HIDDeviceInfo::enumerate_hid_devices();

  auto device = HIDDevice::open(std::move(deviceInfos[0]));

  HIDBuffer buf(64);

  device.write(buf);
}

TEST(DeviceTestSuit, DeviceRead) {
  auto deviceInfos = HIDDeviceInfo::enumerate_hid_devices();

  auto device = HIDDevice::open(std::move(deviceInfos[0]));

  auto buf = device.read(64);

  // TODO(ask): why i can't do HIDBuffer expected{{0}, {0}, {0}};?
  HIDBuffer expected{
      std::byte{0},  std::byte{1},  std::byte{2},  std::byte{3},  std::byte{4},
      std::byte{5},  std::byte{6},  std::byte{7},  std::byte{8},  std::byte{9},
      std::byte{10}, std::byte{11}, std::byte{12}, std::byte{13}, std::byte{14},
      std::byte{15}, std::byte{16}, std::byte{17}, std::byte{18}, std::byte{19},
      std::byte{20}, std::byte{21}, std::byte{22}, std::byte{23}, std::byte{24},
      std::byte{25}, std::byte{26}, std::byte{27}, std::byte{28}, std::byte{29},
      std::byte{30}, std::byte{31}, std::byte{32}, std::byte{33}, std::byte{34},
      std::byte{35}, std::byte{36}, std::byte{37}, std::byte{38}, std::byte{39},
      std::byte{40}, std::byte{41}, std::byte{42}, std::byte{43}, std::byte{44},
      std::byte{45}, std::byte{46}, std::byte{47}, std::byte{48}, std::byte{49},
      std::byte{50}, std::byte{51}, std::byte{52}, std::byte{53}, std::byte{54},
      std::byte{55}, std::byte{56}, std::byte{57}, std::byte{58}, std::byte{59},
      std::byte{60}, std::byte{61}, std::byte{62}, std::byte{63},
  };

  ASSERT_EQ(buf, expected);
}

TEST(DeviceTestSuit, DeviceReportSend) {
  auto deviceInfos = HIDDeviceInfo::enumerate_hid_devices();

  auto device = HIDDevice::open(std::move(deviceInfos[0]));

  HIDReport report(ReportID{0x77}, std::size_t{16});
  report.data[0] = {0}; 
  report.data[1] = {1};
  report.data[2] = {2};
  report.data[3] = {3};
  report.data[4] = {4};
  report.data[5] = {5};
  report.data[6] = {6};

  device.send_feature_report(report);
}

TEST(DeviceTestSuit, DeviceReportRead) {
  auto deviceInfos = HIDDeviceInfo::enumerate_hid_devices();

  auto device = HIDDevice::open(std::move(deviceInfos[0]));

  auto report_received =
      device.receive_feature_report(ReportID{0x77}, std::size_t{16});

  HIDReport expected_report(
      ReportID{0x77}, std::byte{0}, std::byte{1}, std::byte{2}, std::byte{3},
      std::byte{4}, std::byte{5}, std::byte{6}, std::byte{7}, std::byte{8},
      std::byte{9}, std::byte{10}, std::byte{11}, std::byte{12}, std::byte{13},
      std::byte{14}, std::byte{15});

  // TODO: the comparison should be implemented in the HIDReport
  ASSERT_EQ(report_received.value().report, expected_report.report);
  ASSERT_TRUE(std::ranges::equal(report_received.value().data, expected_report.data));
}
