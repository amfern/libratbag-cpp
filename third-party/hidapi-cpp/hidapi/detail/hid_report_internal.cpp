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

ReportData HIDReportInternal::report_data() {
  // return everything except the first byte which is the report
  return ReportData{data_}.subspan(1);
};

HIDReportInternal::HIDReportInternal(ReportID report, std::size_t count) : data_(count + 1) {
  setReport(report);
}

} // namespace detail
} // namespace hidapi

