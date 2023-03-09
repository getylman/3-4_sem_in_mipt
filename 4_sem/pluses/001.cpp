#include <iostream>
#include <memory>

struct A {
  const size_t kSize = 10;
  template constexpr <uint64_t Size = kSize>
  struct B {
    
  };



};


int main() {
  
}

// надо все0таки сделать аналог итератора для чанка
/// _M_impl in 613