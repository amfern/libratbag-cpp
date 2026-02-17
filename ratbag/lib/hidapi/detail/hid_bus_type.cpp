#include "ratbag/lib/hidapi/detail/hid_bus_type.hpp"

#include <iostream>

// namespace std {
// using namespace ratbag::lib::hidapi::detail;

// template <> struct formatter<HidBusType> : formatter<string_view> {
//   template <class FormatContext>
//   typename FormatContext::iterator format(const HidBusType &bus_type,
//                                           FormatContext &ctx) const {

//     auto name = bus_type_to_string(bus_type);
//     return format_to(ctx.out(), "{}", name);
//   };
// };

// } // namespace std

void ratbag::lib::hidapi::detail::miau() {
  std::cout << "miau";
};


namespace std {

// using namespace ratbag::lib::hidapi::detail;

// TOOD: it compiles, but no symbol is generated
// bazel build //ratbag/lib/hidapi/detail:hid_bus_type
// nm --demangle --dynamic --defined-only --extern-only bazel-bin/ratbag/lib/hidapi/detail/libhid_bus_type.so
// TODO(ask): why this symbol is not declared in the .so elf file? is it a C++ thing? the miau() funciton above is included...
// template <class FormatContext>
// typename FormatContext::iterator
// formatter<HidBusType>::format(const HidBusType &bus_type,
//                               FormatContext &ctx) const {
//   auto name = bus_type_to_string(bus_type);
//   return format_to(ctx.out(), "{}", name);
// }

} // namespace std
