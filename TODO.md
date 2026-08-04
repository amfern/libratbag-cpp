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

3. if the underlying type is an premetive type, like int, byte..etc. It's trivial to copy and doesnt't worth the overheard of pointer of the same size...
4. code writing flow
  1. Write the code how i want to do
  2. The see if it's possible to optimize
  3. Profile, if the speed is actually important for that function, if it's called many times a second or impact the latency of the program
5. things that are refernce types, even just pointers, even that doesn't own the object, It's up to the user to treat the reference they received with approproiate care for life of the source object. The user needs to be aware that this is a reference and it points to something in this HIDReport, so if it this hid report goes out of scope, this referfence becomes invalid 
  1. Have reference only as a parameter type, so you know it will outlive the entierly of the function call
  2. As a local variable where you can see the scope it reffering to, And that's the limit, unless i write my own view.
