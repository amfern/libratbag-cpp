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
  // TODO(ask): i want my device.hpp class to asign directly to this data_ raw data() but i don't want to expose to outside..., how can i test it?
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

  // TODO: what do you think about this approach, i want a function that
  // allows to pass reportID, and all of the elements of the data. And i don't
  // want to create intermidate vector or array contains to avoid the copy paste
  // I used DerivedFrom "concept" there
  // it's good, it's correct
  template <std::same_as<std::byte>... Ts>
  HIDReport(ReportID report, Ts ...data) : data_{report, data...} {}

  HIDReport(ReportID report, std::vector<std::byte> data) : data_{std::move(data)} {
    data_.insert(data_.begin(), report);

  }

  HIDReport(ReportID report, std::size_t count) : data_(count + 1) {
    setReport(report);
  }

  HIDReport() { }

};

// TODO(ask): i want to be able to create HIDReport out of buffer received from hidapi.c api
// So i want to wrap around pre-allocated memory bufferi receive from the C API.
// But also i want to initialize a container of my own to pass to the C api.
// And i want it to be the same class, not possible because the user needs to be aware that it's a view.
// the user wouldn't be abel to know if they got the data, or the refernece. and they wouldn't be able to make decision about the lifecycle of the object.

// Or maybe i can have the read return this HIDReportView which is readonly.
// Is there a way to make it a single class, that sometimes is const? maybe with std::variant
// Or maybe i can use concept to operate on an interface, without knowin the actual container underneath?? sort of HIDReportLike....
// class HIDReportView {
//   std::span<std::byte> data_view_;

// public:
//   const ReportID &report = data_view_[0];
//   const std::span<std::byte> data = std::span<std::byte>{data_view_}.subspan(1);

//   // TODO(ask): This constuctor is used internally by HIDDevice class, how to make it private and still be able to test it. I can create a factory in the detail folder?
//   HIDReportView(std::span<std::byte> data_view): data_view_(data_view) {
//   }
// };

} // namespace hidapi
} // namespace lib
} // namespace ratbag
