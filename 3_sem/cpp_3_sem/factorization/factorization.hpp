#pragma once
#include <vector>

std::vector<int> Factorize(long num) {
  std::vector<int> arr;
  for (long i = 2; i <= num; ++i) {
    while (num % i == 0) {
      arr.push_back((int)i);
      num /= i;
    }
  }
  return arr;
}
