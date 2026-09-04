
/* C */
#include <stdlib.h>

#include "hidapi.h"


struct hid_device_info HID_API_EXPORT *
hid_enumerate(unsigned short vendor_id, unsigned short product_id) {
  return NULL;
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

HID_API_EXPORT const wchar_t *HID_API_CALL hid_error(hid_device *dev) {
  return L"Couldn't create udev context";
}
