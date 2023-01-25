#pragma once

#include <type_traits>

#include "AbstractToken.hpp"

namespace calculator {
template <typename T>
class OperandToken : public AbstractToken {
 public:
  OperandToken() = default;
  OperandToken(const std::string& str) : AbstractToken(str) {
    if (std::is_floating_point<T>::value) {
      value_ = std::stod(str);
    } else {
      value_ = std::stoi(str);
    }
  };
  OperandToken(const T& value) : value_(value) {}
  OperandToken& operator=(const T& value) {
    value_ = value;
    return *this;
  }
  const T& GetValue() const { return value_; }
  size_t GetType() const { return kType; }

 private:
  T value_ = 0;
  const size_t kType = 1;
};
}  // namespace calculator