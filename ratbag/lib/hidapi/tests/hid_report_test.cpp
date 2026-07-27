#include "ratbag/lib/hidapi/hid_report.hpp"

#include <gtest/gtest.h>

using ratbag::lib::hidapi::HIDReport;
using ratbag::lib::hidapi::ReportID;

// TODO(ask): how do i know this will not be optimization out
TEST(HidReportTest, CanInitialize) {
  HIDReport report(ReportID{0x77}, std::size_t{16});
  report.data[0] = {0};
  report.data[1] = {1};
  report.data[2] = {2};
  report.data[3] = {3};
  report.data[4] = {4};
  report.data[5] = {5};
  report.data[6] = {6};
}

TEST(HidReportTest, CanInitiazlieAnyNumberOfbytes) {
  HIDReport report(ReportID{0x77}, std::byte{0}, std::byte{1}, std::byte{2},
                   std::byte{3}, std::byte{4}, std::byte{5}, std::byte{6},
                   std::byte{7}, std::byte{8}, std::byte{9}, std::byte{10},
                   std::byte{11}, std::byte{12}, std::byte{13}, std::byte{14},
                   std::byte{15});
}
