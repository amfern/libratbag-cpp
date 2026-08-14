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
// using namespace hidapi;

// template <> struct formatter<HIDReportIn> : formatter<string_view> {

//   template <class FormatContext>
//   typename FormatContext::iterator format(HIDReport &report,
//                                           FormatContext &ctx) const {
//     return format_to(ctx.out(), "report_id: {}, report_data: {}", report.report(), report.report_data());
//   }
// };
// } // namespace std


