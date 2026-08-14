#pragma once

#include <cstddef>
#include <cstdint>
#include <sys/types.h>
#include <vector>
#include <span>
#include <format>
#include <ranges>

namespace hidapi {
namespace detail {

using HIDBuffer = std::vector<std::byte>;
using ReportID = std::byte;
using ReportData = std::span<std::byte>;


class HIDReportInternal {
// friend class std::formatter<HIDReportInternal>;

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

  // TODO: this should be a dedicated type instead of std::span
  ReportData report_data() {
    return ReportData{data_}.subspan(1);  
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

  // TODO(ask): why do i need to tell c++ to use default comparator, why can't it just be the deafult behavior?
  //            I assume the default would just compare all members?
  bool operator==(const HIDReportInternal& rhs) const = default;
  std::strong_ordering operator<=>(const HIDReportInternal& rhs) const = default;
};

} // namespace detail
} // namespace hidapi

namespace std {

using namespace hidapi::detail;

template <> struct formatter<ReportID> : formatter<string_view> {
  template <class FormatContext>
  typename FormatContext::iterator format(ReportID &report_id,
                                          FormatContext &ctx) const {
    return format_to(ctx.out(), "{:#04x}", static_cast<unsigned char>(report_id));
  }
};

template <> struct formatter<ReportData> : formatter<string_view> {
  template <class FormatContext>
  typename FormatContext::iterator format(ReportData &report_data,
                                          FormatContext &ctx) const {
    return format_to(ctx.out(), "{::#04x}",
                     report_data | std::views::transform([](std::byte b) {
                       return std::to_integer<unsigned char>(b);
                     }));
  }
};

template <> struct formatter<HIDReportInternal> : formatter<string_view> {

  template <class FormatContext>
  typename FormatContext::iterator format(HIDReportInternal &report,
                                          FormatContext &ctx) const {
    return format_to(ctx.out(),
                     "HIDReportInternal(report_id: {}, report_data: {})",
                     report.report(), report.report_data());
  }
};

} // namespace std

