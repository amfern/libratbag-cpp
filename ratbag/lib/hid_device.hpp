#include <vector>

namespace ratbag {
namespace lib {

class HIDDevice {
public:
  // TODO: is it possible to use array here without knowing the size upfront?
  static const std::vector &list_hid_devices();

private:
};
} // namespace lib

} // namespace ratbag
