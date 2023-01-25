#pragma once

#include <functional>
#include <map>

#include "OperandToken.hpp"

namespace calculator {
template <typename T, bool IsBin>
class OperatorToken : public AbstractToken {
 public:
  OperatorToken(const std::string& token) : AbstractToken(token) {
    if (!IsBin) {
      priority_ = 3;
      type_ = 3;
    } else {
      if (token[0] == '*' || token[0] == '/') {
        priority_ = 2;
      } else {
        priority_ = 1;
      }
    }
  }
  bool IsBinary() const { return IsBin; }
  T Calculate(OperandToken<T>& left, OperandToken<T>& right) {
    return bin_functions_[token_[0]](left, right);
  }
  T Calculate(OperandToken<T>& operand) {
    return mon_functions_[token_[0]](operand);
  }

  size_t GetPriority() const { return priority_; }
  size_t GetType() const { return type_; }

 private:
  std::map<char, std::function<T(OperandToken<T>&, OperandToken<T>&)>>
          bin_functions_ = {
          {'+',
                  [](OperandToken<T>& left, OperandToken<T>& right) {
                    return left.GetValue() + right.GetValue();
                  }},
          {'-',
                  [](OperandToken<T>& left, OperandToken<T>& right) {
                    return left.GetValue() - right.GetValue();
                  }},
          {'*',
                  [](OperandToken<T>& left, OperandToken<T>& right) {
                    return left.GetValue() * right.GetValue();
                  }},
          {'/', [](OperandToken<T>& left, OperandToken<T>& right) {
            return left.GetValue() / right.GetValue();
          }}};

  std::map<char, std::function<T(OperandToken<T>&)>> mon_functions_ = {
          {'+', [](OperandToken<T>& operand) { return operand.GetValue(); }},
          {'-', [](OperandToken<T>& operand) { return operand.GetValue() * -1; }},
  };

  size_t type_ = 2;
  size_t priority_;
};
}  // namespace calculator