#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <gtest/gtest.h>
#include <iterator>
#include <ranges>

#include "hidapi/hid_report.hpp"

using hidapi::HIDReport;
using hidapi::ReportID;

// TODO: move those tests to /detail directory


// TODO: how do i know this will not be optimization out
// ANS:  the compiler will do the full validation and generate the code and then optimized that code out. and will probabbly write the code if it had side effects.
//       So the compiler will validate the code, even if it's going to be optimized later. And if it has side effects like assert, it will aslo run the code.
TEST(HidReportTest, CanInitialize) {
  HIDReport report(ReportID{0x77}, std::size_t{16});
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
  HIDReport report(ReportID{0x77}, std::byte{0}, std::byte{1}, std::byte{2},
                   std::byte{3}, std::byte{4}, std::byte{5}, std::byte{6},
                   std::byte{7}, std::byte{8}, std::byte{9}, std::byte{10},
                   std::byte{11}, std::byte{12}, std::byte{13}, std::byte{14},
                   std::byte{15});
}

TEST(HidReportTest, formatting) {
  // HIDReport report(ReportID{0x77}, std::byte{0}, std::byte{1}, std::byte{2},
  //                  std::byte{3}, std::byte{4}, std::byte{5}, std::byte{6});

  // TODO: write test for formatting
  // EXPECT_EQ(std::format("{}", static_cast<uint8_t>(std::byte{3})), "adasds");
}
