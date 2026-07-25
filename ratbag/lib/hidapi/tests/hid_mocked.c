
/* C */
#include <stdlib.h>

#include "hidapi.h"

// TODO(ask): why am i able to overwrite the symbol? shouldn't it complain?
// there was no redifinition error.
//            I wasn't able to overwrite with linker --wrap, it seems like all
//            "hid_enumerate" symbol was resolved and stripped in
//            bazel-bin/ratbag/lib/hidapi/libhidapi.so
struct hid_device_info HID_API_EXPORT *
hid_enumerate(unsigned short vendor_id, unsigned short product_id) {
  struct hid_device_info *root =
      (struct hid_device_info *)calloc(1, sizeof(struct hid_device_info));
  root->path = "/mock/path";
  root->vendor_id = 0x1234;
  root->product_id = 0x4321;
  root->serial_number = L"serial number";
  root->release_number = 1;
  root->manufacturer_string = L"mock manufacturer";
  root->product_string = L"mock product";
  root->usage_page = 2;
  root->usage = 3;
  root->interface_number = 4;
  root->next = NULL;
  root->bus_type = HID_API_BUS_SPI;

  return root;
}

void HID_API_EXPORT hid_free_enumeration(struct hid_device_info *devs) {
  free(devs);
}

// TODO: this test will work on linux only, maybe i should create a special variant for the mock in BUILD.bazel.hidapi
struct hid_device_ {
	int device_handle;
	int blocking;
	wchar_t *last_error_str;
	wchar_t *last_read_error_str;
	struct hid_device_info* device_info;
};

HID_API_EXPORT hid_device *HID_API_CALL hid_open_path(const char *path) {
  hid_device *dev = (hid_device *)calloc(1, sizeof(hid_device));

  if (dev == NULL) {
    return NULL;
  }
  dev->device_handle = -1;

  return dev;
}

int HID_API_EXPORT HID_API_CALL hid_write(hid_device *dev, const unsigned char *data, size_t length) {
  return length;
}

int HID_API_EXPORT HID_API_CALL hid_read_timeout(hid_device *dev, unsigned char *data, size_t length, int milliseconds) {
  for (int i = 0; i < length; i++) {
    data[i] = i;
  }

  return length;
}

int HID_API_EXPORT HID_API_CALL hid_send_feature_report(hid_device *dev, const unsigned char *data, size_t length) {
  return 16;
}

int HID_API_EXPORT HID_API_CALL hid_get_feature_report(hid_device *dev, unsigned char *data, size_t length) {
  for (int i = 1; i < length; i++) {
    data[i] = i - 1; 
  }
  
  return length;
}

HID_API_EXPORT const wchar_t * HID_API_CALL hid_error(hid_device *dev) {
  return  L"Success";
}
