#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <vector>

class A {
 public:
 struct B {
  size_t a = 0, b = 1;
 };
 //friend class C;
};

class C {
  void foo1() {
    A::B a;
  }
  friend class D;
};

class D : public C {
 public: 
  void foo() {
    A::B a;
  }
};

int main() {
  
}