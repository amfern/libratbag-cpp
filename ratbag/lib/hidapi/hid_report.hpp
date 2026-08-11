#pragma once

#include <cstddef>
#include <cstdint>
#include <sys/types.h>
#include <vector>
#include <span>
#include <format>

#include "ratbag/lib/hidapi/common.hpp"

namespace ratbag {
namespace lib {
namespace hidapi {

template <typename T, typename B>
concept DerivedFrom = std::is_base_of_v<B, T> or std::same_as<B, T>;

using HIDBuffer = std::vector<std::byte>;
using ReportID = std::byte;

class HIDReport {
friend class HIDDevice;
friend class std::formatter<HIDReport>;
friend class HidReportTest;

private:

public:
  // TODO(ask): I want my device.hpp class to assign directly to this data_ raw data() but i don't want to expose to outside..., how can i test it?
  //            or maybe it's okay to allow to expose it to outside
  HIDBuffer data_;
  // TODO: use std::array instead of std::vector
  // TODO: how to hide this data_ only to be accesiable to the friend class Device

  // TODO: use a member function to return the refefnce to report
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
  
  std::span<std::byte> data() {
    return std::span<std::byte>{data_}.subspan(1);  
  };
  
  // HIDReport(const HIDBuffer& data) : data_(data.begin() +1, data.end()) {
  //   this->report = report;
  // }

  // TODO: this will copy the data... i wonder if there is a btter way of not
  // copying the data Is there a way to use some kind in-place optimization? How
  // about using templated vargs template <typename ...Ts> requires
  // std::same_as<std::common_type_t<Ts...>, std::byte> HIDReport(ReportID
  // report, Ts&&... data) : data_{data...} {  }

  template <std::same_as<std::byte>... Ts>
  HIDReport(ReportID report, Ts ...data) : data_{report, data...} {}

  // TODO: do i even need this function
  //       it can be provided, instead of exposing the internal data_ memeber
  // HIDReport(ReportID report, std::vector<std::byte> data) : data_{std::move(data)} {
  //   data_.insert(data_.begin(), report);
  // }

  // This will resize the vector and preallocte empty values
  HIDReport(ReportID report, std::size_t count) : data_(count + 1) {
    setReport(report);
  }

  HIDReport() { }

};

} // namespace hidapi
} // namespace lib
} // namespace ratbag
