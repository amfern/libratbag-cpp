#include "ratbag/lib/hidapi/hid_report.hpp"

#include <algorithm>
#include <cstddef>
#include <gtest/gtest.h>
#include <iterator>
#include <ranges>

using ratbag::lib::hidapi::HIDReport;
using ratbag::lib::hidapi::ReportID;

// TODO: how do i know this will not be optimization out
// ANS:  the compiler will do the full validation and generate the code and then optimized that code out. and will probabbly write the code if it had side effects.
//       So the compiler will validate the code, even if it's going to be optimized later. And if it has side effects like assert, it will aslo run the code.
TEST(HidReportTest, CanInitialize) {
  HIDReport report(ReportID{0x77}, std::size_t{16});
  auto data = report.data();
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

TEST(HidReportTest, CanInitializeFromBuffer) {
  unsigned char data_raw[] = {0x77, 1,2,3,4,5,6};
  HIDReport report;
  report.data_.reserve(std::ssize(data_raw));

  std::ranges::copy(data_raw | std::views::transform([](unsigned char val) {
                      return std::byte{val};
                    }),
                    std::back_inserter(report.data_));

  auto data = report.data();

  ASSERT_EQ(report.report(), std::byte{0x77});
  ASSERT_EQ(data[0], std::byte{1});
  ASSERT_EQ(data[1], std::byte{2});
  ASSERT_EQ(data[2], std::byte{3});
  ASSERT_EQ(data[3], std::byte{4});
  ASSERT_EQ(data[4], std::byte{5});
  ASSERT_EQ(data[5], std::byte{6});
}

