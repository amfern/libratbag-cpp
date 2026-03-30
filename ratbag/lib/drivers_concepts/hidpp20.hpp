#pragma once

namespace ratbag {
namespace lib {
namespace drivers_concepts {

// TODO(ask): with concepts it's not clear what interface this class implements by simplify looking at it.
//            There is also no enforcment of interface here 
class HIDPP20 final {
public:

  void probe() const {
    // TODO: open hid devices and read from it some name
  }

  void commit() const {
    // TODO: open hid devices and read from it some name
  }
};

} // namespace drivers
} // namespace lib
} // namespace ratbag
