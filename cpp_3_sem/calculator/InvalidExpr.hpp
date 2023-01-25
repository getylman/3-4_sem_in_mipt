#pragma once

#include <exception>
namespace calculator {

struct InvalidExpr : std::exception {
  [[nodiscard]] const char* what() const noexcept override {
    return "Invalid expression!";
  }
};
}  // namespace calculator