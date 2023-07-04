#include <iostream>
int main() {
  const size_t kT26 = 26;
  std::string str;
  getline(std::cin, str);
  size_t kkk = 1;
  for (size_t i = 0; i != kT26; ++i) {
    kkk = str.find(std::string(1, char(i + 'a')));
    if (kkk != std::string::npos) {
      printf("%c : %lu\n", char(i + 'a'), kkk + 1);
    }
  }
}