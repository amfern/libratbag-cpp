#include <cstddef>
#include <cstdint>
#include <vector>
#include <span>
#include <format>

#include "hidapi/hid_report.hpp"
#include "hid_report.hpp"

namespace hidapi {

ReportID HIDReportInternal::report() const {
  return data_[0];
};

void HIDReportInternal::setReport(ReportID report_id) {
  data_[0] = report_id;
};


} // namespace hidapi

namespace std {
using namespace hidapi;

template <> struct formatter<HIDReportInternal> : formatter<string_view> {

  template <class FormatContext>
  typename FormatContext::iterator format(const HIDReportInternal &report,
                                          FormatContext &ctx) const {
    return format_to(ctx.out(), "{}", report.data_);
  }
};
} // namespace std


