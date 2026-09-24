#include "hidapi/detail/hid_report_internal.hpp"

#include <cassert>
#include <cstddef>

namespace hidapi::detail {

HIDReportInternal::HIDReportInternal(ReportID report,
                                     std::size_t report_data_size)
    : HIDBuffer(report_data_size + 1) {
  setReport(report);
}

bool HIDReportInternal::isValid() const { return !empty(); }

ReportID HIDReportInternal::report() const {
  assert("called on moved-from object" && isValid());
  return front();
};

void HIDReportInternal::setReport(ReportID report_id) {
  assert("called on moved-from object" && isValid());
  front() = report_id;
};

ReportData HIDReportInternal::reportData() {
  assert("called on moved-from object" && isValid());
  // return everything except the first byte which is the report
  return {this->begin() + 1, this->end()};
};

HIDBuffer &HIDReportInternal::buffer() {
  assert("called on moved-from object" && isValid());
  return *this;
};

}  // namespace hidapi::detail
