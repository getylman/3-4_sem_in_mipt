#include <cctype>
#include <iostream>

void FillStack(int* stck, int& size, std::string& num_or_sim) {
  if (std::isdigit(num_or_sim[0]) != 0) {
    stck[size++] = std::stoi(num_or_sim);
  } else {
    if (num_or_sim[0] == '+') {
      int tmp = stck[size - 1] + stck[size - 2];
      stck[size - 2] = tmp;
      size--;
    } else if (num_or_sim[0] == '-') {
      int tmp = stck[size - 2] - stck[size - 1];
      stck[size - 2] = tmp;
      size--;
    } else if (num_or_sim[0] == '*') {
      int tmp = stck[size - 1] * stck[size - 2];
      stck[size - 2] = tmp;
      size--;
    }
  }
}

int main() {
  int stck[100] = {};
  int size = 0;
  std::string input;
  std::getline(std::cin, input);
  std::string num_or_sim;
  for (auto i : input) {
    if (i == ' ') {
      FillStack(stck, size, num_or_sim);
      num_or_sim = "";
    } else {
      num_or_sim += i;
    }
  }
  FillStack(stck, size, num_or_sim);
  std::cout << stck[size - 1];
}