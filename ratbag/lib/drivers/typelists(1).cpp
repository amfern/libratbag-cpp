#include <utility>

template <typename DriverList>
DriverList find_driver_impl(std::index_sequence<>) {
  throw std::runtime_error("No matching driver");  
}   

// the  trick to iterate over elements in compile time is to pick off the first memebr of the variadic parameter template, on each iteration through that function
// once the itreation is complete and compiled into recursive funcitons, and during optiomization phase, compiler will collapse all nested recursive calls into simple for loop... 
template <typename DriverList, std::size_t FirstIndex,std::size_t... RemainingIndices>
DriverList
find_driver_impl(std::index_sequence<FirstIndex,RemainingIndices...>) {


// DriverList === std::variant<HIDCPP20,SteelSeries,Driver23,MyOtherDriver>
// FirstIndex === 0
// at_t<DriverList,FirstIndex>=== HIDCPP20
// FirstIndex === 1
// at_t<DriverList,FirstIndex>=== SteelSeries

  using FirstDriverType=arene::base::type_lists::at_t<DriverList, FirstIndex>;

  if (FirstDriverType::probe()) {
    return FirstDriverType{};    
  }

  return find_driver_impl<DriverList>(
                          std::index_sequence<RemainingIndices...>{});
  
  
}



template <typename DriverList> DriverList find_driver() {
  return find_driver_impl<DriverList>(
                          
      std::make_index_sequence<arene::base::type_list::size_v<DriverList>>());
  
}



int main() {
  using DriverVariant = std::variant<X1, X2, X3>;

  DriverVariant drivers;

  drivers = find_driver<DriverVariant>();
  
}
