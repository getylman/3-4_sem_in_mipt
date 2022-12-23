#include <iostream>

int main() {
  int arr[10005] = {}, pr_arr[10005] = {};
  int size = 0, num = 0;
  std::string input, numm = "";
  std::getline(std::cin, input);
  for (auto i : input) {
    if (i == '[') {
      continue;
    }
    if (i == ']') {
      continue;
    }
    if (i == ',') {
      arr[size++] = std::stoi(numm);
      numm = "";
    } else {
      numm += i;
    }
  }
  arr[size++] = std::stoi(numm);
  int target = 0;
  std::cin >> target;
  for (int i = 0; i < size; i++) {
    pr_arr[i + 1] = pr_arr[i] + arr[i];
  }
  int lt = 0, rt = 0;
  for (int i = 1; i <= size; i++) {
    bool to_break = false;
    for (int j = 0; j < size; j++) {
      if (pr_arr[i] - pr_arr[j] == target) {
        lt = j;
        rt = i;
        to_break = true;
        break;
      }
    }
    if (to_break) {
      break;
    }
  }
  std::cout << '[' << lt << "," << rt << ']'; 
}