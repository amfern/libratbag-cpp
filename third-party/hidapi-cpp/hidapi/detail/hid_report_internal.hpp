#pragma once

#include <cstddef>
#include <cstdint>
#include <sys/types.h>
#include <vector>
#include <span>
#include <format>
#include <ranges>

namespace hidapi {
namespace detail {

using HIDBuffer = std::vector<std::byte>;
using ReportID = std::byte;
using ReportData = std::span<std::byte>;

class HIDReportInternal {

public:
  // TODO: maybe use std::array instead of std::vector, and make all HIDBuffer static_size
  HIDBuffer data_;

  ReportID report() const;
  void setReport(ReportID report_id);

  ReportData report_data();

  template <std::same_as<std::byte>... Ts>
  HIDReportInternal(ReportID report, Ts ...data) : data_{report, data...} {}

  // This will resize the vector and preallocte empty values
  // We assume the buffers are small and OS+compiler can handle zeroing out in the cache line even before reaching the memory
  HIDReportInternal(ReportID report, std::size_t count);

  // TODO(ask): why do i need to tell c++ to use default comparator, why can't it just be the deafult behavior?
  //            I assume the default would just compare all members?
  bool operator==(const HIDReportInternal& rhs) const = default;
  std::strong_ordering operator<=>(const HIDReportInternal& rhs) const = default;
};

} // namespace detail
} // namespace hidapi
