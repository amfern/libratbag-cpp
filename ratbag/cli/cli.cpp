#include <iostream>

#include "ratbag/lib/libratbag.hpp"

using ratbag::lib::Device;

int main() {
  // 1. List all HID devices

  // 2. Open Device with corresponding driver to the HID based on HID
  // description and Path
  Device<Driver>::Open();

  std::cout << "Hello, Modern C++ World!" << std::endl;

  return 0;
}
