#pragma once

#include <string>
#include <vector>

#include "AbstractToken.hpp"
#include "BracketToken.hpp"
#include "ExprInPolishNotation.hpp"
#include "InvalidExpr.hpp"
#include "OperandToken.hpp"
#include "OperatorToken.hpp"

namespace calculator {
template <typename T>
class Calculator {
 public:
  static T CalculateExpr(const std::string& expression) {
    auto tokens = ExprInPolishNotation<T>(expression).GetTokens();
    std::reverse(tokens.begin(), tokens.end());
    size_t begin = 0;
    size_t end = tokens.size();
    OperandToken<T> answer = Calculate(tokens, begin, end);
    Clear(tokens);
    if (begin != end) {
      throw InvalidExpr();
    }
    return answer.GetValue();
  }

 private:
  static OperandToken<T> Calculate(std::vector<AbstractToken*>& polish_form,
                                   size_t& begin, size_t& end) {
    if (begin == end) {
      Clear(polish_form);
      throw InvalidExpr();
    }
    size_t type = (polish_form[begin])->GetType();
    if (type == kBracketType) {
      Clear(polish_form);
      throw InvalidExpr();
    }
    if (type == kOperandType) {
      OperandToken<T> answer = (polish_form[begin])->GetStringToken();
      ++begin;
      return answer;
    }
    size_t next_token = begin;
    ++next_token;
    OperandToken<T> first = Calculate(polish_form, next_token, end);
    OperandToken<T> answer;
    if (type == kBinOperatorType) {
      OperandToken<T> second = Calculate(polish_form, next_token, end);
      answer = reinterpret_cast<OperatorToken<T, true>*>(polish_form[begin])
              ->Calculate(second, first);
    } else {
      answer = reinterpret_cast<OperatorToken<T, false>*>(polish_form[begin])
              ->Calculate(first);
    }
    begin = next_token;
    return answer;
  }

  static void Clear(std::vector<AbstractToken*>& tokens) {
    for (auto& token : tokens) {
      delete token;
    }
  }

  static const size_t kBracketType = 0;
  static const size_t kOperandType = 1;
  static const size_t kBinOperatorType = 2;
  static const size_t kMonOperatorType = 3;
};
}  // namespace calculator