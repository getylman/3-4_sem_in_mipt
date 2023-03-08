#include <iostream>
#include <memory>

int main() {
  std::allocator<int> al;
  int* a = std::allocator_traits<std::allocator<int>>::allocate(al, 10);
  for (size_t i = 0; i < 10; ++i) {
    std::allocator_traits<std::allocator<int>>::construct(al, a + i, i);
  }
  for (size_t i = 0; i < 10; ++i) {
    std::cout << a[i] << ' ';
    std::allocator_traits<std::allocator<int>>::destroy(al, a + i);
  }
  std::allocator_traits<std::allocator<int>>::deallocate(al, a, 10);
}