#pragma once

// have a single header file to include all header for the external user
// consumption
#include "hidapi-cpp/hidapi/device.hpp"
#include "hidapi-cpp/hidapi/device_info.hpp"
#include "hidapi-cpp/hidapi/hid_report.hpp"

// C++26 can do it automatically with C++ modules
// Module is a separate translation unit
// It doesn't replace bazel cc_rules
// so user could just do 'import hidapi';
