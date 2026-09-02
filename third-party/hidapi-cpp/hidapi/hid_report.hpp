#pragma once

#include <cstddef>
#include <cstdint>
#include <sys/types.h>
#include <vector>
#include <span>
#include <format>

#include "hidapi/detail/hid_report_internal.hpp"

namespace hidapi {

// Reexport the following types from detail?
using HIDBuffer = hidapi::detail::HIDBuffer;
using ReportID = hidapi::detail::ReportID;
using ReportData = hidapi::detail::ReportData;

class HIDReport : private hidapi::detail::HIDReportInternal {

// TODO: using friend means by classes are badley design, does it apply for this case aswell?
//       Where i want to limit the a access to raw data buffer only to my internal API?
// ans:  sometimes it is ncessary, and can make the API looks simple to the external users, and as long as it's hidden from the user
friend class HIDDevice;

public:
  using hidapi::detail::HIDReportInternal::report_data;
  using hidapi::detail::HIDReportInternal::report;
  using hidapi::detail::HIDReportInternal::setReport;

  // forward the constructors from base class
  using hidapi::detail::HIDReportInternal::HIDReportInternal;

  bool operator==(const HIDReport& rhs) const = default; 
  std::strong_ordering operator<=>(const HIDReport& rhs) const = default;
};


} // namespace hidapi

namespace std {

template <> struct formatter<hidapi::ReportID> : formatter<string_view> {
  template <class FormatContext>
  typename FormatContext::iterator format(hidapi::ReportID &report_id,
                                          FormatContext &ctx) const {
    return format_to(ctx.out(), "{:#04x}", static_cast<unsigned char>(report_id));
  }
};

template <> struct formatter<hidapi::ReportData> : formatter<string_view> {
  template <class FormatContext>
  typename FormatContext::iterator format(hidapi::ReportData &report_data,
                                          FormatContext &ctx) const {
    return format_to(ctx.out(), "{::#04x}",
                     report_data | std::views::transform([](std::byte b) {
                       return std::to_integer<unsigned char>(b);
                     }));
  }
};

template <> struct formatter<hidapi::HIDReport> : formatter<string_view> {

  template <class FormatContext>
  typename FormatContext::iterator format(hidapi::HIDReport &report,
                                          FormatContext &ctx) const {
    return format_to(ctx.out(),
                     "HIDReport(report_id: {}, report_data: {})",
                     report.report(), report.report_data());
  }
};

} // namespace std
