#include <iostream>

struct Base {
  virtual void foo() const {
    std::cout << 1;
  }
};
struct Derived : Base {
  virtual void foo() {
    std::cout << 2;
  }
};
int main() {
  Derived d;
  d.foo();
  Base& b = d;
  b.foo();
}

/*#include <iostream>  4.1
class A {
 public:
  A() {
    std::cout << "create A" << std::endl;
  }
  ~A() {
    std::cout << "destroy A" << std::endl;
  }
};
class C {
 public:
  C() {
    std::cout << "create C" << std::endl;
  }
  ~C() {
    std::cout << "destroy C" << std::endl;
  }
};
class B : public A {
 public:
  B() {
    std::cout << "create B" << std::endl;
  }
  ~B() {
    std::cout << "destroy B" << std::endl;
  }

 private:
  C c;
};
int main() {
  B b;
  A a = b;
}*/

/*#include <iostream>       3.9
class Int {
 public:
  Int() = default;
  Int(int x) : x_(x) {
  }
  friend bool operator<(const Int& other, const Int& other1);

 private:
  int x_ = 0;
};
bool operator<(const Int& other, const Int& other1) {
    return other.x_ < other1.x_;
  }
int main() {
  Int a;
  if (3 < a) {
    std::cout << "less\n";
  }
}*/

/*#include <iostream>   3.8
class C {
 public:
  C() = default;
  C(const C& other) { std::cout << 1 << std::endl; }
  //C& operator=(const C& other) {
  //  std::cout << 2 << std::endl;
  //  return *this;
  //}
  C& operator+=(const C& other) { return *this; }
};
C operator+(const C& first, const C& second) { return C(first) += second; }
int main() {
  C a, b;
  C c = a + b;
}*/








/*
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

std::vector<double> l;
std::pair<int, int> digit;
int counter = 0;
std::vector < std::pair < int, int>> dots;
int amount;
int rule;

int main() {
  std::cin >> amount >> rule;
  for (int i = 0; i < amount; ++i) {
    if (rule == 2) {
      std::cout << 0;
      return 0;
    }
    std::cin >> digit.first;
    std::cin >> digit.second;
    dots.push_back(digit);
  }
  for (long unsigned int i = 0; i < dots.size(); ++i) {
    for (long unsigned int j = 0; j < dots.size(); ++j) {
      if (j == i) {
        continue;
      }
      l.push_back(pow(pow(dots[i].second - dots[j].second, 2) +
                          pow(dots[i].first - dots[j].first, 2),
                      0.5));
    }
    sort(l.begin(), l.end());
    int ind_beg = 0;
    for (long unsigned int i = 0; i < l.size(); ++i) {
      if (l[i] != l[ind_beg]) {
        ind_beg = i;
      }
      counter += (i - ind_beg);
    }
    l.clear();
  }
  std::cout << counter;
}
*/

/*
#include <bits/stdc++.h>

using namespace std;

int main() {
  int n;
  cin >> n;
  vector<int> a(n + 1);
  vector<int> dp1(n + 1);
  vector<int> dp2(n + 1);
  vector<int> pred1(n + 1);
  vector<int> pred2(n + 1);
  for (int i = 1; i <= n; ++i) {
    cin >> a[i];
  }
  int ind = 0;
  int mx = 0;
  for (int i = 1; i <= n; ++i) {
    dp1[i] = dp2[i] = 1;
    for (int j = 1; j < i; ++j) {
      if (a[i] > a[j]) {
        dp1[i] = max(dp1[j], dp2[j] + 1);
        if (dp1[i] == dp2[j] + 1) {
          pred1[i] = j;
        }
      } else if (a[i] < a[j]) {
        dp2[i] = max(dp2[j], dp1[j] + 1);
        if (dp2[i] == dp1[j] + 1) {
          pred2[i] = j;
        }
      }
    }
    mx = max(mx, dp1[i]);
    mx = max(mx, dp2[i]);
    if (mx == dp1[i] || mx == dp2[i]) {
      ind = i;
    }
  }
  cout << mx << "\n";
  bool f = false;
  if (mx == dp2[ind]) {
    f = true;
  }
  vector<int> v;
  while (ind != 0) {
    v.push_back(ind);
    if (f) {
      ind = pred2[ind];
    } else {
      ind = pred1[ind];
    }
    f ^= 1;
  }
  reverse(v.begin(), v.end());
  for (auto i : v) {
    cout << a[i] << " ";
  }
}
*/