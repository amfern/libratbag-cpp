### siphon ideas and best c++23 practices
https://github.com/fmtlib/fmt - modern C++
things should be done. BOOST - abit mixed bag, with legacy and modern
https://github.com/ericniebler/range-v3 - abit complicated, things i really
don't need to know about this point, full of modern yet complicated bits.

### reference vs pointer 
No difference between pointer and reference in terms of performance...
refernce is an c++ syntatic sugar, can't be null.
Compiler cannot optimize reference more than an pointer.
Underlying architecture still passes raw pointer even if it's reference.
Reference is just to limit the programmer and communicate that it can't be
null Using reference as class member tights instance of the class to the
object unrevocably

I want it to be reference because it gives me the assurance it's
not going to be nullptr DeviceInfo class cannot exist without
hid_device_info. However, the other side is that i can't move HIDDeviceInfo
class. Why can't i have both, being able to tell c++ that this object has
to be always valid. And also being able to move the objects... Is it up to
me to enforce??? is my class internal implementation details. so it's okay
to not have device_info_ reference?
It feels like c++ gives me the means to implement memory safety, but
doesn't implement it by it self out of the box. non_null.hpp
unlike other language which try to hide the memory management from developer behind abstractions, c++ give the developers the means to defined memmory management abstractions.

regarding that "reference is just an pointer with limitation",
does it mean that if i do "const &" it will return an pointer.
And the caller will receive and pointer that it will have to
dereference? Usage is just an uint16_t, should i just return by
value and let it be copied?

yes! the reason for using reference
1. avoid copying the thing if the thing is expansive to copy
2. To make sure the returned values stays synced if something changes whithin the class
string_view is an alredy and reference, so we cna return it by value. It's
cheap to copy. cheaper than dereferencing an address
