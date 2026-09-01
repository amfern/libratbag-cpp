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
using ReportData = hidapi::detail::ReportData;
using HIDReportInternal = hidapi::detail::HIDReportInternal;

class HIDReport : private HIDReportInternal {

// TODO: using friend means by classes are badley design, does it apply for this case aswell?
//       Where i want to limit the a access to raw data buffer only to my internal API?
// ans:  sometimes it is ncessary, and can make the API looks simple to the external users, and as long as it's hidden from the user
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

namespace std {

using namespace hidapi;

template <> struct formatter<ReportID> : formatter<string_view> {
  template <class FormatContext>
  typename FormatContext::iterator format(ReportID &report_id,
                                          FormatContext &ctx) const {
    return format_to(ctx.out(), "{:#04x}", static_cast<unsigned char>(report_id));
  }
};

template <> struct formatter<ReportData> : formatter<string_view> {
  template <class FormatContext>
  typename FormatContext::iterator format(ReportData &report_data,
                                          FormatContext &ctx) const {
    return format_to(ctx.out(), "{::#04x}",
                     report_data | std::views::transform([](std::byte b) {
                       return std::to_integer<unsigned char>(b);
                     }));
  }
};

template <> struct formatter<HIDReport> : formatter<string_view> {

  template <class FormatContext>
  typename FormatContext::iterator format(HIDReport &report,
                                          FormatContext &ctx) const {
    return format_to(ctx.out(),
                     "HIDReport(report_id: {}, report_data: {})",
                     report.report(), report.report_data());
  }
};

} // namespace std
