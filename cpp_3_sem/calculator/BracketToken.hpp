#pragma once

#include "AbstractToken.hpp"

namespace calculator {
class BracketToken : public AbstractToken {
 public:
  BracketToken(const std::string& token) : AbstractToken(token) {
    is_opening_ = token[0] == '(';
  }
  bool IsOpening() const { return is_opening_; }

  size_t GetType() const { return kType; }

 private:
  bool is_opening_;
  const size_t kType = 0;
};
}  // namespace calculator