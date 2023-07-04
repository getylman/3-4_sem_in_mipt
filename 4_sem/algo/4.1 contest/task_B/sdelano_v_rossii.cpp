#include <iostream>
#include <locale>

int main() {
  std::locale::global(std::locale{std::string().data()});
  std::wstring str;
  std::wcin.imbue(std::locale{});
  std::wcout.imbue(std::locale{});
  getline(std::wcin, str);
  for (size_t i = 0; i < str.size(); ++i) {
    std::wcout << *(str.rbegin() + i);
  }
}