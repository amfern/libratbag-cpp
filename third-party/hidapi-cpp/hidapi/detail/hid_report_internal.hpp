#pragma once

#include <cstddef>
#include <cstdint>
#include <sys/types.h>
#include <vector>
#include <span>
// #include <format>

namespace hidapi {
namespace detail {

using HIDBuffer = std::vector<std::byte>; // TODO: this should be in some common type, because it's also used by device.cpp
using ReportID = std::byte;


// if based class was empty type, inhereting it would have saved data, but here it's not the case
class HIDReportInternal : private HIDBuffer {
// friend class std::formatter<HIDReportInternal>;
private:
  using HIDBuffer::begin;
  using HIDBuffer::end;

public:
  // TODO: maybe use std::array instead of std::vector, and make all HIDBuffer static_size
  // HIDBuffer data_;

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
    // TODO: it feels strange to use *this, because i am refering to pointer, why can't i use "this" as a reference...
    // return std::span<std::byte>{this->begin() + 1, this->end()}.subspan(1);
    return std::span<std::byte>(this->begin() + 1, this->end());
  };
  
  template <std::same_as<std::byte>... Ts>
  HIDReportInternal(ReportID report, Ts ...report_data) : HIDBuffer{report, report_data...} {}

  // TODO: do i even need this function
  //       it can be provided, instead of exposing the internal data_ memeber
  // TODO: this will copy the data... i wonder if there is a btter way of not
  // copying the data Is there a way to use some kind in-place optimization? How
  // HIDReport(ReportID report, std::vector<std::byte> data) : data_{std::move(data)} {
  //   data_.insert(data_.begin(), report);
  // }

  // This will resize the vector and preallocte empty values
  // We assume the buffers are small and OS+compiler can handle zeroing out in the cache line even before reaching the memory
  // TODO: shouljd i use std::ssize instead of std::size_t
  HIDReportInternal(ReportID report, std::size_t report_data_size) : HIDBuffer(report_data_size + 1) {
    setReport(report);
  }

  // forward comperators
  bool operator==(const HIDReportInternal& rhs) const = default; 
  std::strong_ordering operator<=>(const HIDReportInternal& rhs) const = default;
  // // 1. Custom Equality (only check serial numbers)
  // bool operator==(const HIDReportInternal& rhs) const {
  //   return data_ == rhs.data_;
  // }

  // std::strong_ordering operator<=>(const HIDReportInternal& rhs) const {
  //   return data_ <=> rhs.data_;
  // };
  //

  // expose vector operations
  using HIDBuffer::size;
  using HIDBuffer::data;
};

} // namespace detail
} // namespace hidapi
