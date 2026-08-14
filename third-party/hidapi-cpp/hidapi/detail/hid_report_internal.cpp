#include <cstddef>
#include <cstdint>
#include <vector>
#include <span>
#include <format>

#include "hidapi/detail/hid_report_internal.hpp"

namespace hidapi {
namespace detail {

ReportID HIDReportInternal::report() const {
  return data_[0];
};

void HIDReportInternal::setReport(ReportID report_id) {
  data_[0] = report_id;
};

} // namespace detail
} // namespace hidapi

// namespace std {

// using namespace hidapi::detail;

// template <> struct formatter<HIDReportInternal> : formatter<string_view> {

//   template <class FormatContext>
//   typename FormatContext::iterator format(HIDReportInternal &report,
//                                           FormatContext &ctx) const {
//     // return format_to(ctx.out(), "report_id: {:02X}", static_cast<int>(report.report()));
//     return format_to(ctx.out(), "report_id: {}", 254);
//     // return format_to(ctx.out(), "report_id: {}, report_data: {}", report.report(), report.report_data());
//   }
// };

// } // namespace std


