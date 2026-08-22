#include <cstddef>
#include <cstdint>
#include <vector>
#include <span>
#include <format>

#include "hidapi/detail/hid_report_internal.hpp"

namespace hidapi {
namespace detail {

ReportID HIDReportInternal::report() const {
  return front();
};

void HIDReportInternal::setReport(ReportID report_id) {
  front() = report_id;
};

ReportData HIDReportInternal::report_data() {
  // return everything except the first byte which is the report
  return std::span<std::byte>(this->begin() + 1, this->end());
};

HIDReportInternal::HIDReportInternal(ReportID report, std::size_t report_data_size) : HIDBuffer(report_data_size + 1) {
  setReport(report);
}

} // namespace detail
} // namespace hidapi

