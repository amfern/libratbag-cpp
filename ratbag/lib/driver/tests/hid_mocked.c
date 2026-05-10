
/* C */
#include <stdlib.h>

#include "hidapi.h"

struct hid_device_info HID_API_EXPORT *
hid_enumerate(unsigned short vendor_id, unsigned short product_id) {

  // init hidpp20
  struct hid_device_info *root =
      (struct hid_device_info *)calloc(1, sizeof(struct hid_device_info));
  root->path = "/mock/path";
  root->vendor_id = 0x046d;
  root->product_id = 0xc332;
  root->serial_number = L"serial number";
  root->release_number = 1;
  root->manufacturer_string = L"mock manufacturer";
  root->product_string = L"mock product";
  root->usage_page = 2;
  root->usage = 3;
  root->interface_number = 4;
  /* root->next = NULL; */
  root->bus_type = HID_API_BUS_SPI;

  // init steelseries
  root->next =
      (struct hid_device_info *)calloc(1, sizeof(struct hid_device_info));


  root->next->path = "/mock/path";
  root->next->vendor_id = 0x1038;
  root->next->product_id = 0x1384;
  root->next->serial_number = L"serial number";
  root->next->release_number = 1;
  root->next->manufacturer_string = L"mock manufacturer";
  root->next->product_string = L"mock product";
  root->next->usage_page = 2;
  root->next->usage = 3;
  root->next->interface_number = 4;
  root->next->next = NULL;
  root->next->bus_type = HID_API_BUS_SPI;

  // TODO: maybe instead of mocking hid, we can create an helper util to allocate any type of deviceinfo, so we can test the specific area of the code and not from the bottom everytime everything.
  //       Or maybe test it's better to test from interface-to-interface, so the lowest interace of librtabg is hidapi...
  //       I bet Anthony would say to do both.

  return root;
}

void HID_API_EXPORT hid_free_enumeration(struct hid_device_info *devs) {
  free(devs);
}
