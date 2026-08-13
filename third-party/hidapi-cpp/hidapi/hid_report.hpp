#pragma once

#include <cstddef>
#include <cstdint>
#include <sys/types.h>
#include <vector>
#include <span>
#include <format>

#include "detail/hid_report_internal.hpp"

namespace hidapi {

using HIDBuffer = hidapi::detail::HIDBuffer;
using ReportID = hidapi::detail::ReportID;
using HIDReportInternal = hidapi::detail::HIDReportInternal;

class HIDReport : private HIDReportInternal {

// TODO(ask): using friend means by classes are badley design, does it apply for this case aswell?
// Where i want to limit the a access to raw data_ buffer only to my internal API?
friend class HIDDevice;

public:
  using HIDReportInternal::report_data;
  using HIDReportInternal::report;
  using HIDReportInternal::setReport;

  // forward the constructors from base class
  using HIDReportInternal::HIDReportInternal;

  bool operator==(const HIDReport& rhs) const = default; 
  std::strong_ordering operator<=>(const HIDReport& rhs) const = default;
};


} // namespace hidapi
