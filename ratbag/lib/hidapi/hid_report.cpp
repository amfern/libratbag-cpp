#include <cstddef>
#include <cstdint>
#include <vector>
#include <span>
#include <format>

#include "ratbag/lib/hidapi/hid_report.hpp"

namespace ratbag {
namespace lib {
namespace hidapi {

ReportID HIDReport::report() const {
  return data_[0];
};

void HIDReport::setReport(ReportID report_id) {
  data_[0] = report_id;
};


} // namespace hidapi
} // namespace lib
} // namespace ratbag

namespace std {
using namespace ratbag::lib::hidapi;

template <> struct formatter<HIDReport> : formatter<string_view> {

  template <class FormatContext>
  typename FormatContext::iterator format(const HIDReport &report,
                                          FormatContext &ctx) const {
    return format_to(ctx.out(), "{}", report.data_);
  }
};
} // namespace std


