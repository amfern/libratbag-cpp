#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <gtest/gtest.h>
#include <iterator>
#include <ranges>

#include "hidapi/hid_report.hpp"

// using hidapi::HIDReport;
// using hidapi::ReportID;

// TODO: now that i have HidReport an HidReportInternal, i endup doing duplicate tests, should i be doing duplicated tests?
// ans : yes, but have more nuanced tests in the internal implementation
TEST(HidReportTest, CanInitialize) {
  hidapi::HIDReport report(hidapi::ReportID{0x77}, std::size_t{16});
  auto data = report.report_data();
  data[0] = {0};
  data[1] = {1};
  data[2] = {2};
  data[3] = {3};
  data[4] = {4};
  data[5] = {5};
  data[6] = {6};

  ASSERT_EQ(report.report(), std::byte{0x77});
  assert(data[6] == std::byte{6});
}

TEST(HidReportTest, CanInitiazlieAnyNumberOfbytes) {
  hidapi::HIDReport report(hidapi::ReportID{0x77}, std::byte{0}, std::byte{1}, std::byte{2},
                   std::byte{3}, std::byte{4}, std::byte{5}, std::byte{6},
                   std::byte{7}, std::byte{8}, std::byte{9}, std::byte{10},
                   std::byte{11}, std::byte{12}, std::byte{13}, std::byte{14},
                   std::byte{15});
}

TEST(HidReportTest, CanFormatPrint) {
  hidapi::HIDReport report(hidapi::ReportID{0x77}, std::byte{0}, std::byte{1},
                           std::byte{2}, std::byte{3}, std::byte{4},
                           std::byte{5}, std::byte{6});

  EXPECT_EQ(std::format("{}", report),
            "HIDReport(report_id: 0x77, report_data: [0x00, 0x01, "
            "0x02, 0x03, 0x04, 0x05, 0x06])");
}
