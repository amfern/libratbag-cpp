#pragma once

namespace ratbag {
namespace lib {
namespace drivers {


// TODO: should i use inheritance, what is the proper way way to implement interface in modern C++ 23
class HIDPP20 : ratbag::lib::Driver {

public:
  DriverName DriverName();
  DriverID driver_id();
  void commit();
  void set_active_profile();

  static void probe();
private:

  explicit HIDPP20();
};

}
}
}
