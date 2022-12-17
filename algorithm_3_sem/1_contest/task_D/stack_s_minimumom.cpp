#include <iostream>
int main() {
  int num_of_calls = 0;
  std::cin >> num_of_calls;
  int stck[200000];
  int mn[200000];
  int size = 0;
  for (int i = 0; i < num_of_calls; i++) {
    std::string com;
    std::cin >> com;
    if (com == "push") {
      std::cin >> stck[size++];
      mn[size - 1] = (size == 1)
                         ? (stck[size - 1])
                         : ((mn[size - 2] > stck[size - 1]) ? (stck[size - 1])
                                                            : (mn[size - 2]));
      std::cout << "ok";
    } else if (com == "pop") {
      std::cout << ((size == 0) ? ("error") : std::to_string(stck[--size]));
    } else if (com == "back") {
      std::cout << ((size == 0) ? ("error") : std::to_string(stck[size - 1]));
    } else if (com == "size") {
      std::cout << size;
    } else if (com == "clear") {
      size = 0;
      std::cout << "ok";
    } else if (com == "min") {
      std::cout << ((size == 0) ? ("error") : std::to_string(mn[size - 1]));
    }
    std::cout << std::endl;
  }
}