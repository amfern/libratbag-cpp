#pragma once

#include <cstddef>
#include <cstdint>
#include <sys/types.h>
#include <vector>
#include <span>
#include <format>

#include "hidapi/common.hpp"

namespace hidapi {

using HIDBuffer = std::vector<std::byte>;
using ReportID = std::byte;


class HIDReportInternal {
friend class HIDDevice;
friend class std::formatter<HIDReport>;

public:
  // TODO: maybe use std::array instead of std::vector, and make all HIDBuffer static_size
  HIDBuffer data_;

  ReportID report() const;
  void setReport(ReportID report_id);

  
  // make a member function to return it
  // The user needs to be aware that this is a reference and it points to something in this HIDReport, so if it this hid report goes out of scope, this referfence becomes invalid
  // things that are refernce types, even just pointers, even that doesn't own the object.
  // - Have them only as a parameter type, so you know it will outlive the entierly of the function call
  // - As a local variable where you can see the scope it reffering to
  // And that's the limit, unless i write my own view
  // It's up to the user to treat the reference they received with approproiate care for life of the source object
  // span is also short lived, as it referes to internal memory of vector.
  // std::span<std::byte> data = std::span<std::byte>{data_}.subspan(1);
  
  std::span<std::byte> report_data() {
    return std::span<std::byte>{data_}.subspan(1);  
  };
  
  template <std::same_as<std::byte>... Ts>
  HIDReportInternal(ReportID report, Ts ...data) : data_{report, data...} {}

  // TODO: do i even need this function
  //       it can be provided, instead of exposing the internal data_ memeber
  // TODO: this will copy the data... i wonder if there is a btter way of not
  // copying the data Is there a way to use some kind in-place optimization? How
  // HIDReport(ReportID report, std::vector<std::byte> data) : data_{std::move(data)} {
  //   data_.insert(data_.begin(), report);
  // }

  // This will resize the vector and preallocte empty values
  // We assume the buffers are small and OS+compiler can handle zeroing out in the cache line even before reaching the memory
  HIDReportInternal(ReportID report, std::size_t count) : data_(count + 1) {
    setReport(report);
  }

  // 1. Custom Equality (only check serial numbers)
  bool operator==(const HIDReportInternal& rhs) const {
    return data_ == rhs.data_;
  }

  std::strong_ordering operator<=>(const HIDReportInternal& rhs) const {
    return data_ <=> rhs.data_;
  };
};

class HIDReport : private HIDReportInternal {

// TODO(ask): using friend means by classes are badley design, does it apply for this case aswell?
// Where i want to limit the a access to raw data_ buffer only to my internal API?
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
