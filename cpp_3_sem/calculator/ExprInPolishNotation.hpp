#pragma once

#include <set>
#include <stack>

#include "BracketToken.hpp"
#include "Calculator.hpp"
#include "InvalidExpr.hpp"
#include "OperandToken.hpp"
#include "OperatorToken.hpp"

namespace calculator {
template <typename T>
class ExprInPolishNotation {
 public:
  ExprInPolishNotation(const std::string& str) {
    tokens_ = ParseToTokens(str);
    MakePolish();
  }

  ~ExprInPolishNotation() {
    int error = std::uncaught_exceptions();
    if (error != exceptions_count_) {
      for (auto& token : tokens_) {
        delete token;
      }
    }
  }

  const std::vector<AbstractToken*>& GetTokens() const { return tokens_; }

 private:
  // function for string parser
  bool LastSituationInStringParser(const std::string& str, size_t& start_pos,
                                   size_t& pos,
                                   std::vector<AbstractToken*>& tokens) {
    start_pos = pos;
    pos = str.find_first_not_of(kDigits_, start_pos);
    if (pos == kError) {
      tokens.push_back(
              new OperandToken<T>(
                      str.substr(start_pos, str.size() - start_pos)));
    } else {
      tokens.push_back(
              new OperandToken<T>(str.substr(start_pos, pos - start_pos)));
    }
    --pos;
    return true;
  }

  std::vector<AbstractToken*> ParseToTokens(const std::string& str) {
    std::vector<AbstractToken*> tokens;
    size_t start_pos = 0;
    size_t pos = 0;
    bool last_was_number = false;
    while (start_pos != str.size()) {
      pos = str.find_first_not_of(' ', start_pos);
      if (pos == kError) {
        break;
      }
      if (str[pos] == '(' || str[pos] == ')') {
        tokens.push_back(new BracketToken(str.substr(pos, 1)));
        last_was_number = str[pos] == ')';
      } else if ((str[pos] == '+' || str[pos] == '-') && !last_was_number) {
        tokens.push_back(new OperatorToken<T, false>(str.substr(pos, 1)));
      } else if ((str[pos] == '*' || str[pos] == '/') && !last_was_number) {
        throw InvalidExpr();
      } else if (kSigns.contains(str[pos])) {
        tokens.push_back(new OperatorToken<T, true>(str.substr(pos, 1)));
        last_was_number = false;
      } else {
        last_was_number =
                LastSituationInStringParser(str, start_pos, pos, tokens);
      }
      start_pos = pos + 1;
    }
    return tokens;
  }

  // function to make polish notation
  void ClosingBracketSituation(std::vector<AbstractToken*>& result,
                               std::stack<AbstractToken*>& operators) {
    if (operators.empty()) {
      throw InvalidExpr();
    }
    while (operators.top()->GetType() != kBracketType) {
      result.push_back(operators.top());
      operators.pop();
      if (operators.empty()) {
        throw InvalidExpr();
      }
    }
    delete operators.top();
    operators.pop();
  }

  // function to make polish notation
  void BinOperatorSituation(std::vector<AbstractToken*>& result,
                            std::stack<AbstractToken*>& operators,
                            size_t& idx) {
    while (!operators.empty() && operators.top()->GetType() != kBracketType) {
      if (operators.top()->GetType() == kMonOperatorType ||
          reinterpret_cast<OperatorToken<T, true>*>(operators.top())->GetPriority() >=
          reinterpret_cast<OperatorToken<T, true>*>(tokens_[idx])->GetPriority()) {
        result.push_back(operators.top());
        operators.pop();
      } else {
        break;
      }
    }
    operators.push(tokens_[idx]);
  }

  void MakePolish() {
    std::stack<AbstractToken*> operators;
    std::vector<AbstractToken*> result;
    for (size_t i = 0; i < tokens_.size(); ++i) {
      size_t type = (tokens_[i])->GetType();
      if (type == kOperandType) {
        result.push_back(tokens_[i]);
      } else if (type == kBracketType) {
        if (reinterpret_cast<BracketToken*>(tokens_[i])->IsOpening()) {
          operators.push(tokens_[i]);
        } else {
          ClosingBracketSituation(result, operators);
          delete tokens_[i];
        }
      } else if (type == kMonOperatorType) {
        operators.push(tokens_[i]);
      } else {
        BinOperatorSituation(result, operators, i);
      }
    }
    while (!operators.empty()) {
      result.push_back(operators.top());
      operators.pop();
    }
    tokens_ = result;
  }

  std::vector<AbstractToken*> tokens_;
  const char* kDigits_ = "0123456789.";
  const std::set<char> kSigns = {'+', '-', '*', '/'};
  const size_t kError = std::string::npos;
  const size_t kBracketType = 0;
  const size_t kOperandType = 1;
  const size_t kBinOperatorType = 2;
  const size_t kMonOperatorType = 3;
  int exceptions_count_ = std::uncaught_exceptions();
};
}  // namespace calculator