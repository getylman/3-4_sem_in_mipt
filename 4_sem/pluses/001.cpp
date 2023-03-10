#include <iostream>
#include <deque>
// #include "deque/deque.hpp"
struct C {
  const size_t kSize = 10;
};

struct A {
  const size_t kSize = 10;
  static constexpr size_t foo() {
    const size_t val = 15;
    return val;
  }
  template <size_t Size = foo()>
  struct B {
    B() { std::cout << Size; }
  };
  // A() : kSize(10) {}


}; //чекни пометки в самом файле и не забудь надо работать рангом а не kChunkSize


int main() {
  std::deque<int> d(10);
  std::deque<int>::const_iterator cit = d.begin();
  
}

// надо все0таки сделать аналог итератора для чанка
/// _M_impl in 613
/// _Deque_impl in 542
/// _Deque_impl_data in 509