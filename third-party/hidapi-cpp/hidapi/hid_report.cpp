#include <cstddef>
#include <cstdint>
#include <vector>
#include <span>
#include <format>

#include "hidapi/hid_report.hpp"

namespace std {
using namespace hidapi;

template <> struct formatter<HIDReport> : formatter<string_view> {

  template <class FormatContext>
  typename FormatContext::iterator format(HIDReport &report,
                                          FormatContext &ctx) const {
    return format_to(ctx.out(), "report_id: {}, report_data: {}", report.report(), report.report_data());
  }
};
} // namespace std


