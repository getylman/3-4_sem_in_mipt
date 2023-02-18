#include "big_integer.hpp"

//                    constructors
BigInt::BigInt() {
  big_int_.clear();
  sign_ = true;
  big_int_.push_back(0);
}

BigInt::BigInt(const int64_t& num) {
  big_int_.clear();
  sign_ = (num >= 0);
  uint64_t tmp_num = num;
  if (num < 0) {
    tmp_num = -tmp_num;
  }
  if (tmp_num == 0) {
    big_int_.push_back(0);
  }
  while (tmp_num > 0) {
    big_int_.push_back(tmp_num % kBigNumForMod);
    tmp_num /= kBigNumForMod;
  }
  ErasePrNulls();
}

BigInt::BigInt(const std::string& num) {
  big_int_.clear();
  const size_t kNine = 9;
  sign_ = true;
  if (num.empty()) {
    big_int_.push_back(0);
  } else {
    std::string digit;
    size_t ind_in_s = 0;
    if (num[0] == '-') {
      sign_ = false;
      ind_in_s = 1;
    }
    for (size_t i = num.size() - 1; i >= ind_in_s && i < num.size(); --i) {
      digit = num[i] + digit;
      if (digit.size() == kNine) {
        big_int_.push_back(std::stoi(digit));
        digit = "";
      }
    }
    if (!digit.empty()) {
      big_int_.push_back(std::stoi(digit));
    }
  }
  ErasePrNulls();
}
BigInt::BigInt(const BigInt& num) {
  sign_ = num.sign_;
  big_int_ = num.big_int_;
}
BigInt& BigInt::operator=(const BigInt& num) {
  sign_ = num.sign_;
  big_int_ = num.big_int_;
  return *this;
}
//------------------------------------------------------
//                        modificators
BigInt& BigInt::operator+=(const BigInt& num) {
  if (sign_ == num.sign_) {
    ToSumWithSameSign(num);  //реализация в привате
  } else {
    ToSumWithNotSameSignes(num);  //реализация в привате
  }
  ErasePrNulls();
  return *this;
}
BigInt BigInt::operator+(const BigInt& num) const {
  return BigInt(*this) += num;
}
BigInt& BigInt::operator-=(const BigInt& num) {
  return operator+=(-(BigInt(num)));
}
BigInt BigInt::operator-(const BigInt& num) const {
  return BigInt(*this) -= num;
}
BigInt& BigInt::operator*=(const BigInt& num) {
  bool end_sign = (sign_ == num.sign_);
  sign_ = true;
  const BigInt kMainVal = *this;
  *this = 0;
  uint64_t delta = 0;
  for (size_t i = 0; i < num.big_int_.size(); ++i) {
    *this += kMainVal.ToMultByDigit(num.big_int_[i], i);
  }
  sign_ = end_sign;
  ErasePrNulls();
  return *this;
}
BigInt BigInt::operator*(const BigInt& num) const {
  return BigInt(*this) *= num;
}
BigInt& BigInt::operator/=(const BigInt& num) {
  ExcInDiv(num);
  bool end_sign = (sign_ == num.sign_);
  sign_ = true;
  const BigInt kDividend = *this;
  const BigInt kNumAbs = num.Abs();
  BigInt rt = *this;
  BigInt lt = *this = 0;
  BigInt mid = 0;
  while (rt >= lt) {
    mid = lt + rt;
    ToDivideByTwo(mid);
    if (mid * kNumAbs < kDividend) {
      *this = mid + 1;
      lt = mid + 1;
    } else {
      rt = mid - 1;
    }
  }
  if (kDividend != (*this * kNumAbs)) {
    --*this;
  }
  sign_ = end_sign;
  ErasePrNulls();
  return *this;
}
BigInt BigInt::operator/(const BigInt& num) const {
  return BigInt(*this) /= num;
}
BigInt& BigInt::operator%=(const BigInt& num) {
  return *this -= (*this / num) * num;
}
BigInt BigInt::operator%(const BigInt& num) const {
  return BigInt(*this) %= num;
}
//------------------------------------------------------
//                        comp opers
bool BigInt::operator==(const BigInt& num) const {
  return !(*this < num || *this > num);
}
bool BigInt::operator!=(const BigInt& num) const { return !operator==(num); }
bool BigInt::operator<(const BigInt& num) const {
  if (sign_ == num.sign_) {
    if (sign_) {
      if (big_int_.size() != num.big_int_.size()) {
        return big_int_.size() < num.big_int_.size();
      }
      for (size_t i = big_int_.size() - 1; i < big_int_.size(); --i) {
        if (big_int_[i] != num.big_int_[i]) {
          return big_int_[i] < num.big_int_[i];
        }
      }
    }
    if (big_int_.size() != num.big_int_.size()) {
      return big_int_.size() > num.big_int_.size();
    }
    for (size_t i = big_int_.size() - 1; big_int_.size() > i; --i) {
      if (big_int_[i] != num.big_int_[i]) {
        return big_int_[i] > num.big_int_[i];
      }
    }
  }
  return (int)sign_ < (int)num.sign_;
}
bool BigInt::operator<=(const BigInt& num) const { return !operator>(num); }
bool BigInt::operator>(const BigInt& num) const { return num.operator<(*this); }
bool BigInt::operator>=(const BigInt& num) const { return !operator<(num); }
//------------------------------------------------------
//                        other modificators
BigInt& BigInt::operator-() {
  if (big_int_.size() != 1 || big_int_[0] != 0) {
    sign_ = !sign_;
  }
  return *this;
}
BigInt& BigInt::operator++() { return *this += 1; }
BigInt& BigInt::operator--() { return *this -= 1; }
BigInt BigInt::operator++(int) {
  const BigInt kTmpNum = *this;
  *this += 1;
  return kTmpNum;
}
BigInt BigInt::operator--(int) {
  const BigInt kTmpNum = *this;
  *this -= 1;
  return kTmpNum;
}
BigInt BigInt::Abs() const {
  BigInt tmp_num = *this;
  tmp_num.sign_ = true;
  return tmp_num;
}
//------------------------------------------------------
//                        iostream
std::istream& operator>>(std::istream& is, BigInt& num) {
  std::string tmp_str;
  is >> tmp_str;
  num = BigInt(tmp_str);
  return is;
}
std::ostream& operator<<(std::ostream& os, const BigInt& num) {
  if (!num.sign_) {
    os << "-";
  }
  const size_t kNine = 9;
  std::string tmp_str;
  for (size_t i = num.big_int_.size() - 1; i < num.big_int_.size(); --i) {
    tmp_str = std::to_string(num.big_int_[i]);
    if (i != num.big_int_.size() - 1) {
      for (size_t j = tmp_str.size(); j < kNine; ++j) {
        os << "0";
      }
    }
    os << tmp_str;
  }
  return os;
}
//------------------------------------------------------
//                        privat funcs
void BigInt::ErasePrNulls() {
  while (big_int_.size() > 1 && big_int_.back() == 0) {
    big_int_.pop_back();
  }
  if (big_int_.size() == 1 && big_int_[0] == 0) {
    sign_ = true;
  }
}
void BigInt::ToSumWithSameSign(const BigInt& num) {
  size_t mx = std::max(big_int_.size(), num.big_int_.size());
  uint64_t sum = 0;
  uint64_t delta = 0;
  for (size_t i = 0; i < mx; ++i) {
    sum = delta;
    if (i < big_int_.size()) {
      sum += big_int_[i];
    }
    if (i < num.big_int_.size()) {
      sum += num.big_int_[i];
    }
    delta = sum / kBigNumForMod;
    if (i == big_int_.size()) {
      big_int_.push_back(0);
    }
    big_int_[i] = sum % kBigNumForMod;
  }
  if (delta != 0) {
    big_int_.push_back(delta);
  }
}
void BigInt::ToSumWithNotSameSignes(const BigInt& num) {
  BigInt lt = Abs();
  BigInt rt = num.Abs();
  uint64_t delta = 0;
  uint64_t minus = 0;
  if (lt < rt) {
    for (size_t i = 0; i < rt.big_int_.size(); ++i) {
      minus = delta;
      if (i < lt.big_int_.size()) {
        minus += lt.big_int_[i];
      }
      delta = (uint64_t)(rt.big_int_[i] < minus);
      rt.big_int_[i] += kBigNumForMod - minus;
      rt.big_int_[i] %= kBigNumForMod;
    }
    *this = rt;
    sign_ = num.sign_;
  } else {
    for (size_t i = 0; i < big_int_.size(); ++i) {
      minus = delta;
      if (i < num.big_int_.size()) {
        minus += num.big_int_[i];
      }
      delta = (uint64_t)(big_int_[i] < minus);
      big_int_[i] += kBigNumForMod - minus;
      big_int_[i] %= kBigNumForMod;
    }
  }
}
BigInt BigInt::ToMultByDigit(const uint64_t& digit, const size_t& nulls) const {
  BigInt num;
  uint64_t delta = 0;
  uint64_t mul = 0;
  num.big_int_.clear();
  if (digit == 0 || *this == 0) {
    return 0;
  }
  for (size_t i = 0; i < nulls; ++i) {
    num.big_int_.push_back(0);
  }
  for (size_t i = 0; i < big_int_.size(); ++i) {
    mul = digit * big_int_[i] + delta;
    num.big_int_.push_back(mul % kBigNumForMod);
    delta = mul /= kBigNumForMod;
  }
  while (delta > 0) {
    num.big_int_.push_back(delta % kBigNumForMod);
    delta /= kBigNumForMod;
  }
  return num;
}
void BigInt::ToDivideByTwo(BigInt& num) const {
  uint64_t rod = 0;  // remainder of division
  const size_t kSize = num.big_int_.size();
  for (size_t i = kSize - 1; i < kSize; --i) {
    num.big_int_[i] += rod * kBigNumForMod;
    rod = num.big_int_[i] & 1;
    num.big_int_[i] /= 2;
  }
  num.ErasePrNulls();
}
BigInt BigInt::ExcInDiv(const BigInt& num) {
  if (num == 0) {
    throw std::runtime_error("You want to divide by zero\n");
  }
  if (Abs() < num.Abs()) {
    return *this = 0;
  }
  return *this;
}
//------------------------------------------------------
