#pragma once
#include <iostream>
#include <vector>

class BigInt {
 public:
  // constructors
  BigInt();
  BigInt(const std::string&);
  BigInt(const int64_t&);
  BigInt(const BigInt&);
  ~BigInt() = default;
  BigInt& operator=(const BigInt& num);
  // modificators
  BigInt& operator+=(const BigInt& num);
  BigInt operator+(const BigInt& num) const;
  BigInt& operator-=(const BigInt& num);
  BigInt operator-(const BigInt& num) const;
  BigInt& operator*=(const BigInt& num);
  BigInt operator*(const BigInt& num) const;
  BigInt& operator/=(const BigInt& num);
  BigInt operator/(const BigInt& num) const;
  BigInt& operator%=(const BigInt& num);
  BigInt operator%(const BigInt& num) const;
  // comp operators
  bool operator==(const BigInt&) const;  // done
  bool operator!=(const BigInt&) const;  // done
  bool operator<(const BigInt&) const;   // done
  bool operator<=(const BigInt&) const;  // done
  bool operator>(const BigInt&) const;   // done
  bool operator>=(const BigInt&) const;  // done
  // other modificators
  BigInt& operator-();
  BigInt& operator++();
  BigInt& operator--();
  BigInt operator++(int);
  BigInt operator--(int);
  BigInt Abs() const;  // функция модуля
  // iostream
  friend std::istream& operator>>(std::istream&, BigInt&);
  friend std::ostream& operator<<(std::ostream&, const BigInt&);

 private:
  const uint64_t kBigNumForMod = 1000000000;
  //мне лень конвертировать в вдоичную 9 nulls
  bool sign_;
  std::vector<uint64_t> big_int_;
  void ErasePrNulls();
  // для удаления лидирующих нулей
  void ToSumWithSameSign(const BigInt&);
  // для оператора +=
  void ToSumWithNotSameSignes(const BigInt&);
  // для оператора +=
  BigInt ToMultByDigit(const uint64_t&, const size_t&) const;
  // для оператора *=
  void ToDivideByTwo(BigInt&) const;
  // для оператора /=
  BigInt ExcInDiv(const BigInt&);
  // для оператора /=
};

std::istream& operator>>(std::istream& is, BigInt& num);
std::ostream& operator<<(std::ostream& os, const BigInt& num);
