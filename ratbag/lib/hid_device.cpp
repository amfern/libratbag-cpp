#include <tuple>
include "hid_device.hpp"

namespace ratbag {
namespace lib {
namespace hid {

  // TODO: the idea is to wrap around the memory and not copy it...
  std::vector<HIDDeviceInfo> HIDDeviceInfo::enumerate_hid_devices() {
    std::vector<HIDDeviceInfo> deviceInfos;
    struct hid_device_info *devs, *cur_dev;
    devs = hid_enumerate(0, 0); // 0,0 = find all devices

    cur_dev = devs;
    while (cur_dev) {
      deviceInfos.emplace_back(*cur_dev);
      cur_dev = cur_dev->next;
    }
    hid_free_enumeration(devs);
  }

  explicit HIDDeviceInfo::HIDDeviceInfo(hid_device_info &device_info)
      : device_info_(device_info),
        DeviceID_{ device_info_.vendor_id, device_info_.product_id }, // TODO: What is this syntax {...  } , were the {} overriden?
        HIDPath_(device_info_.path),
        SerialNumber_(device_info_.serial_number),
        ManufacturerString_(device_info_.manufacturer_string),
        ProductString_(device_info_.product_string), {
  }

  HIDDeviceInfo::~HIDDeviceInfo() {
    struct hid_device_info *next = d->next;
    free(device_info_.path);
    free(device_info_.serial_number);
    free(device_info_.manufacturer_string);
    free(device_info_.product_string);
    free(&device_info_);

    // TODO: do i need to this? can i just call some default c++ destructor?
    delete HIDPath_;
    delete DeviceID_;
    delete SerialNumber_;
    delete ManufacturerString_;
    delete ProductString_;
  }

  // TODO: implement those, they are required because we override the destructor
  HIDDeviceInfo::HIDDeviceInfo(const HIDDeviceInfo &other); // copy constructor
  HIDDeviceInfo::HIDDeviceInfo(
      HIDDeviceInfo &&other) noexcept; // copy assignment
  HIDDeviceInfo::HIDDeviceInfo &
  operator=(const HIDDeviceInfo &other); // move constructor
  HIDDeviceInfo::HIDDeviceInfo &
  operator=(HIDDeviceInfo &&other) noexcept; // move operator

  const HIDPath &HIDDeviceInfo::path() {
    return HIDPath_;
  }

  const DeviceID &HIDDeviceInfo::device_id() {
    return DeviceID_; 
  }

  const SerialNumber &HIDDeviceInfo::serial_number() {
    return static_cast<const SerialNumber &>(device_info_.serial_number);
  }

  const ReleaseNumber &HIDDeviceInfo::release_number() {
    // return device_info_.release_number;

    // TODO:  device_info_.release_number is unsigned short and const ReleaseNumber is uint16_t, should i do an static_cast here?
    //        ideally i want to return the number by reference and not copy it.
    // is this correct?
    return static_cast<const ReleaseNumber &>(device_info_.release_number);
  }

  const std::wstring_view &HIDDeviceInfo::manufacturer_string() {
    return ManufacturerString_;
  }

  const std::wstring_view &HIDDeviceInfo::product_string() {
    return ProductString_;
  }

  const UsagePage &HIDDeviceInfo::usage_page() {
    return static_cast<const UsagePage &>(device_info_.usage_page);
  }

  const Usage &HIDDeviceInfo::usage() {
    return static_cast<const Usage &>(device_info_.usage);
  }

  const InterfaceNumber &HIDDeviceInfo::interface_number() {
    return static_cast<const InterfaceNumber &>(device_info_.interface_number);
  }

  const HidBusType &HIDDeviceInfo::bus_type() {
    return device_info_.bus_type;
  }

  } // namespace hid
  } // namespace lib
} // namespace ratbag
