#pragma once

namespace ratbag {
namespace lib {

using DriverName = std::string_view;
using DriverID = std::string_view;

class Driver {

public:
  // TODO: this is an common intefaces between mouse and keyboards devices as defined by libratbag
  //       check other libraries for similar things


  // TODO: should i use inheritance, what is the proper way way to implement interface in modern C++ 23
  //       I believe virtual is froun upon in modern C++, but what is the alternative?

  // TODO: C++20 concept, something that satisfies this concept, is like an interace that is not an object. requires function template
  //       but inheritnce is good if we want to pass pointers
  //       Another option is std::variant. with std::visit or just refer by auto
  virtual DriverName DriverName();
  virtual DriverID driver_id();
  virtual void commit(); // TODO: throw exception if commit failed?
  virtual void set_active_profile();

  // TODO: what is this .sym file https://github.com/libratbag/libratbag/blob/master/src/liblur.sym
};

}
}
