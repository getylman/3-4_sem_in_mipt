#include <iostream>

bool IsOpenBracket(char bracket) {
  return (bracket == '(' || bracket == '{' || bracket == '[');
}

bool AreMatchingBrackts(char rb, char lb) {
  if (lb == ')') {
    return (rb == '(');
  }
  if (lb == '}') {
    return (rb == '{');
  }
  return (rb == '[');
}

int main() {
  char stck[100000] = {};
  int size = 0;
  std::string input;
  std::getline(std::cin, input);
  for (auto i : input) {
    if (IsOpenBracket(i)) {
      stck[size++] = i;
    } else {
      if (size == 0) {
        std::cout << "NO";
        return 0;
      }
      if (AreMatchingBrackts(stck[size - 1], i)) {
        size--;
      } else {
        std::cout << "NO";
        return 0;
      }
    }
  }
  std::cout << ((size == 0) ? ("YES") : ("NO"));
}