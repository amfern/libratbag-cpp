#pragma once

#include <vector>
#include <span>

namespace hidapi {
namespace detail {

using HIDBuffer = std::vector<std::byte>;
using ReportID = std::byte;
using ReportData = std::span<std::byte>;

// if based class was empty type, inhereting it would have saved data, but here it's not the case
class HIDReportInternal : private HIDBuffer {

public:
  template <std::same_as<std::byte>... Ts>
  HIDReportInternal(ReportID report, Ts ...report_data) : HIDBuffer{report, report_data...} {}

  // This will resize the vector and preallocte empty values
  // We assume the buffers are small and OS+compiler can handle zeroing out in the cache line even before reaching the memory
  HIDReportInternal(ReportID report, std::size_t count);

  bool operator==(const HIDReportInternal& rhs) const = default;
  std::strong_ordering operator<=>(const HIDReportInternal& rhs) const = default;

  // lifetime management
  bool isValid() const;

  // members
  ReportID report() const;
  void setReport(ReportID report_id);

  ReportData reportData();

  // get underlying HIDBuffer
  HIDBuffer& buffer();

  // expose vector operations
  using HIDBuffer::size;
  using HIDBuffer::data;
};

} // namespace detail
} // namespace hidapi
