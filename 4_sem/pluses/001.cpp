#include <iostream>
// #include <deque>
#include <vector>
#include <memory>
#include <type_traits>
#include "deque2.0/deque.hpp"
// struct C {
//   const size_t kSize = 10;
// };
// 
// struct A {
//   const size_t kSize = 10;
//   static constexpr size_t foo() {
//     const size_t val = 15;
//     return val;
//   }
//   template <size_t Size = foo()>
//   struct B {
//     B() { std::cout << Size; }
//   };
//   // A() : kSize(10) {}
// 
// 
// }; //чекни пометки в самом файле и не забудь надо работать рангом а не kChunkSize

template <typename T>
struct S {
  T* ptr = nullptr;
  size_t size = 10;
  size_t size_ = 0;
  std::allocator<T> alloc;
  S() : size_(size) {
    ptr = std::allocator_traits<decltype(alloc)>::allocate(alloc, size);
    for (size_t i = 0; i < size; ++i) {
      std::allocator_traits<decltype(alloc)>::construct(alloc, ptr + i, T());
    }
  }
  S(const S<T>& s) : size_(s.size_) {
    ptr = std::allocator_traits<decltype(alloc)>::allocate(alloc, size_);
    for (size_t i = 0; i < size_; ++i) {
      std::allocator_traits<decltype(alloc)>::construct(alloc, ptr + i, s.ptr + i);
    }
  } 
  void printt() {
    for (size_t i = 0; i < size_; ++i) {
      std::cout << *(ptr + i) << ' ';
    }
  }
  ~S() {
    for (size_t i = 0; i < size_; ++i) {
      std::allocator_traits<decltype(alloc)>::destroy(alloc, ptr + i);
    }
    std::allocator_traits<decltype(alloc)>::deallocate(alloc, ptr, size_);
  }
};

template <typename T>
void PPPPP(const std::vector<T>& v) {
  for (const auto& i : v) {
    std::cout << i << ' ';
  }
}

int main() {
  Deque<int> dk(10);
}

// надо все0таки сделать аналог итератора для чанка
/// _M_impl in 613
/// _Deque_impl in 542
/// _Deque_impl_data in 509