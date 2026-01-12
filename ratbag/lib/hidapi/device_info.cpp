#include <tuple>

#include "ratbag/lib/hidapi/device_info.hpp"

namespace ratbag {
namespace lib {
namespace hidapi {

  // TODO: the idea is to wrap around the memory and not copy it...
  //       should i use std::span with custom itereator that uses the cur_dev->next? 
  const std::vector<HIDDeviceInfo> HIDDeviceInfo::enumerate_hid_devices() {
    std::vector<HIDDeviceInfo> deviceInfos;
    struct hid_device_info *devs, *cur_dev;
    devs = hid_enumerate(0, 0); // 0,0 = find all devices

    cur_dev = devs;
    while (cur_dev) {
      deviceInfos.emplace_back(*cur_dev);
      cur_dev = cur_dev->next;
    }

    return deviceInfos; 
  }

  HIDDeviceInfo::HIDDeviceInfo(hid_device_info &device_info)
      : device_info_(device_info),
        HIDPath_(device_info_.path),
        DeviceID_{ device_info_.vendor_id, device_info_.product_id }, // TODO: What is this syntax {...  } , were the {} overriden?
        SerialNumber_(device_info_.serial_number),
        ManufacturerString_(device_info_.manufacturer_string),
        ProductString_(device_info_.product_string) {
  }

  HIDDeviceInfo::~HIDDeviceInfo() {
    delete device_info_.path;
    delete device_info_.serial_number;
    delete device_info_.manufacturer_string;
    delete device_info_.product_string;
    delete &device_info_;
  }

  // TODO: implement those, they are required because we override the destructor
  // HIDDeviceInfo::HIDDeviceInfo(const HIDDeviceInfo &other) {
  //   // TODO: should i copy the strings?? or just create and std::shared_ptr for device_info_
  //   // or disable the copy constructor and disallow copying?
  // } // copy constructor

  // HIDDeviceInfo::HIDDeviceInfo(
  //     HIDDeviceInfo &&other) noexcept; // copy assignment
  // HIDDeviceInfo::HIDDeviceInfo &

  // HIDDeviceInfo::HIDDeviceInfo(HIDDeviceInfo&& other) noexcept :
  //     : device_info_(other.device_info),
  //       HIDPath_(std::move(other.HIDPath_)),
  //       DeviceID_(std::move(other.DeviceID)),
  //       SerialNumber_(std::move(other.SerialNumber_)),
  //       ManufacturerString_(std::move(other.ManufacturerString_)),
  //       ProductString_(std::move(other.ProductString_)) {

  //   // TODO: how to prevent from the "other" object destructor deleteing the c_string??
  // }

  const HIDPath &HIDDeviceInfo::path() {
    return HIDPath_;
  }

  const DeviceID &HIDDeviceInfo::device_id() {
    return DeviceID_; 
  }

  const SerialNumber &HIDDeviceInfo::serial_number() {
    return SerialNumber_;
    
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
