#include <iostream>
#include <cmath>
#include <exception>
#include <algorithm>

class QNum {
 public:
  QNum() {
    znam = 1;
    chis = 0;
    sign = true; 
  }
  QNum(int64_t ch, int64_t zm) {
    if (zm == 0) {
      throw std::logic_error("u idiot");
    }
    znam = abs(zm);
    chis = abs(ch);
    sign = (zm > 0) == (ch >= 0);
  }
  QNum(int64_t num) {
    znam = 1;
    chis = abs(num);
    sign = (num >= 0);
  }
  bool GetSign() { return sign; }
  size_t GetZnam() { return znam; }
  size_t GetChis() { return chis; }
  friend void operator-(QNum&);
  friend void operator+(QNum&, QNum);
 private:
  bool sign;
  size_t znam = 1;
  size_t chis = 0; 
  size_t lcd_(size_t lt, size_t rt) {
    return lt * rt / std::__gcd(lt, rt);
  }
};

bool operator==(QNum lt, QNum rt) {
  if (lt.GetSign() == rt.GetSign()) {
    return ((lt.GetZnam() / std::__gcd(lt.GetZnam(), rt.GetZnam()) * rt.GetChis()) 
            == ((rt.GetZnam() / std::__gcd(lt.GetZnam(), rt.GetZnam()) * lt.GetChis())));
  }
  return false;
}

bool operator<(QNum lt, QNum rt) {
  if (lt.GetSign() == rt.GetSign()) {
    return ((lt.GetZnam() / std::__gcd(lt.GetZnam(), rt.GetZnam()) * rt.GetChis()) 
            < ((rt.GetZnam() / std::__gcd(lt.GetZnam(), rt.GetZnam()) * lt.GetChis())));
  }
  return (lt.GetSign() == false);
}

void operator-(QNum& num) {
  num.sign = !num.sign;
}

void operator+(QNum& lt, QNum rt) {
  ;
}

int main() {
  return 0;
}