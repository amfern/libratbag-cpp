#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <ranges>

#include "gtest/gtest.h"

#include "hidapi/detail/hid_report_internal.hpp"

using hidapi::detail::HIDReportInternal;
using hidapi::detail::ReportID;

// QA:  how do i know this will not be optimization out
// ANS:  the compiler will do the full validation and generate the code and then optimized that code out. and will probabbly write the code if it had side effects.
//       So the compiler will validate the code, even if it's going to be optimized later. And if it has side effects like assert, it will aslo run the code.
TEST(HidReportInternalTest, CanInitialize) {
  HIDReportInternal report(ReportID{0x77}, std::size_t{16});
  auto data = report.report_data();
  data[0] = {0};
  data[1] = {1};
  data[2] = {2};
  data[3] = {3};
  data[4] = {4};
  data[5] = {5};
  data[6] = {6};

  ASSERT_EQ(report.report(), std::byte{0x77});
  assert(data[0] == std::byte{0});
  assert(data[6] == std::byte{6});
  assert(data[7] == std::byte{7});
}

TEST(HidReportInternalTest, CanInitialize) {
  HIDReportInternal report(ReportID{0x77}, std::size_t{0});
  auto data = report.report_data();

  ASSERT_EQ(report.report(), std::byte{0x77});
  ASSERT_EQ(data.size(), std::size_t{0});
}

TEST(HidReportInternalTest, CanInitiazlieAnyNumberOfbytes) {
  HIDReportInternal report(ReportID{0x77}, std::byte{0}, std::byte{1}, std::byte{2},
                   std::byte{3}, std::byte{4}, std::byte{5}, std::byte{6},
                   std::byte{7}, std::byte{8}, std::byte{9}, std::byte{10},
                   std::byte{11}, std::byte{12}, std::byte{13}, std::byte{14},
                   std::byte{15});
}

TEST(HidReportInternalTest, CanInitializeFromBuffer) {
  unsigned char c_api_fill_raw_data[] = {0x77, 1,2,3,4,5,6};

  // preallocate report object
  HIDReportInternal report(ReportID{}, std::ssize(c_api_fill_raw_data));

  // mimic C-API populating the storage
  std::ranges::copy(c_api_fill_raw_data, reinterpret_cast<unsigned char*>(report.data()));

  auto data = report.report_data();

  ASSERT_EQ(report.report(), ReportID{0x77});
  ASSERT_EQ(data[0], std::byte{1});
  ASSERT_EQ(data[1], std::byte{2});
  ASSERT_EQ(data[2], std::byte{3});
  ASSERT_EQ(data[3], std::byte{4});
  ASSERT_EQ(data[4], std::byte{5});
  ASSERT_EQ(data[5], std::byte{6});
}

// TEST(HidReportInternalTest, CanInitializeFromBuffer) {
//   unsigned char data_raw[] = {0x77, 1,2,3,4,5,6};
//   HIDReport report;
//   // report.data_.reserve(std::ssize(data_raw));

//   std::ranges::copy(data_raw | std::views::transform([](unsigned char val) {
//                       return std::byte{val};
//                     }),
//                     std::back_inserter(report.data_));

//   auto data = report.data();

//   ASSERT_EQ(report.report(), std::byte{0x77});
//   ASSERT_EQ(data[0], std::byte{1});
//   ASSERT_EQ(data[1], std::byte{2});
//   ASSERT_EQ(data[2], std::byte{3});
//   ASSERT_EQ(data[3], std::byte{4});
//   ASSERT_EQ(data[4], std::byte{5});
//   ASSERT_EQ(data[5], std::byte{6});
// }
