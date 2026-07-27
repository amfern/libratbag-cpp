#pragma once

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

private:
  HIDBuffer data_;

public:
  // TODO: use std::array instead of std::vector
  // TODO: how to hide this data_ only to be accesiable to the friend class Device

  ReportID &report = data_[0];
  std::span<std::byte> data = std::span<std::byte>{data_}.subspan(1);

  // HIDReport(const HIDBuffer& data) : data_(data.begin() +1, data.end()) {
  //   this->report = report;
  // }

  // TODO: this will copy the data... i wonder if there is a btter way of not
  // copying the data Is there a way to use some kind in-place optimization? How
  // about using templated vargs template <typename ...Ts> requires
  // std::same_as<std::common_type_t<Ts...>, std::byte> HIDReport(ReportID
  // report, Ts&&... data) : data_{data...} {  }

  // TODO(ask): what do you think about this approach, i want a function that
  // allows to pass reportID, and all of the elements of the data. And i don't
  // want to create intermidate vector or array contains to avoid the copy paste
  // I used DerivedFrom "concept" there
  template <DerivedFrom<std::byte>... Ts>
  HIDReport(ReportID report, Ts &&...data) : data_{report, data...} {}
  HIDReport(ReportID report, std::size_t count) : data_(count + 1) {
    this->report = report;
  }
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
