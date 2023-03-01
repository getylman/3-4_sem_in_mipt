#include <iostream>

template <size_t N>
struct A {
  void f() { std::cout << N; }
};

template <size_t K>
using B = A<K>;

using C = B<1>;
using D = B<2>;

int main() {
  C a;
  a.f();
}