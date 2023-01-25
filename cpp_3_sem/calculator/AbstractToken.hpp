#pragma once

#include <string>

namespace calculator {
class AbstractToken {
 public:
  AbstractToken() = default;
  AbstractToken(const AbstractToken&) = default;
  AbstractToken(AbstractToken&&) = default;
  AbstractToken& operator=(const AbstractToken&) = default;
  AbstractToken& operator=(AbstractToken&&) = default;
  explicit AbstractToken(const std::string& token) : token_(token) {}
  virtual ~AbstractToken() = default;
  const std::string& GetStringToken() const { return token_; };

  virtual size_t GetType() const = 0;
 protected:
  std::string token_;
};
}  // namespace calculator