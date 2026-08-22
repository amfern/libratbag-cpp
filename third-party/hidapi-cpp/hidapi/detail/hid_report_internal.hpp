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

// if based class was empty type, inhereting it would have saved data, but here it's not the case
class HIDReportInternal : private HIDBuffer {

public:
  ReportID report() const;
  void setReport(ReportID report_id);

  ReportData report_data();

  template <std::same_as<std::byte>... Ts>
  HIDReportInternal(ReportID report, Ts ...report_data) : HIDBuffer{report, report_data...} {}

  // This will resize the vector and preallocte empty values
  // We assume the buffers are small and OS+compiler can handle zeroing out in the cache line even before reaching the memory
  HIDReportInternal(ReportID report, std::size_t count);

  bool operator==(const HIDReportInternal& rhs) const = default;
  std::strong_ordering operator<=>(const HIDReportInternal& rhs) const = default;

  // expose vector operations
  using HIDBuffer::size;
  using HIDBuffer::data;
};

} // namespace detail
} // namespace hidapi
