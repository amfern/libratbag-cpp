
/* C */
#include <stdlib.h>

#include "hidapi.h"


// TODO(ask): why am i able to overwrite the symbol? shouldn't it complain? there was no redifinition error.
//            I wasn't able to overwrite with linker --wrap, it seems like all "hid_enumerate" symbol was resolved and stripped in bazel-bin/ratbag/lib/hidapi/libhidapi.so
struct hid_device_info HID_API_EXPORT *hid_enumerate(unsigned short vendor_id, unsigned short product_id)
{
  	struct hid_device_info *root = (struct hid_device_info*) calloc(1, sizeof(struct hid_device_info));
    root->path = "/mock/path";
	root->vendor_id = 0x1234;
    root->product_id = 0x4321;
    root->serial_number = L"serial number";
    root->release_number = 1;
    root->manufacturer_string = L"mock manufacturer";
    root->product_string = L"mock product";
    root->usage_page = 2;
    root->interface_number = 3;
    root->next = NULL;
	root->bus_type = HID_API_BUS_SPI;

	return root;
}

void HID_API_EXPORT hid_free_enumeration(struct hid_device_info *devs)
{
	struct hid_device_info *d = devs;
    free(d);
}
