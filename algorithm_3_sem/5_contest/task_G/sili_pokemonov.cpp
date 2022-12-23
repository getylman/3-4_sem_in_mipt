#include <algorithm>
#include <iostream>
#include <vector>

int main() {
  const int kKindOfInf = 1e9;
  size_t size = 0;
  std::cin >> size;
  std::vector<int> arr;
  arr.reserve(size);
  int elem;
  for (size_t i = 0; i < size; ++i) {
    std::cin >> elem;
    arr.push_back(elem);
  }
  std::vector<int> dp = {-kKindOfInf, kKindOfInf};
  dp.reserve(size);
  for (size_t i = 0; i < size; ++i) {
    *std::lower_bound(dp.begin(), dp.end(), arr[i]) = arr[i];
    if (dp[dp.size() - 1] != kKindOfInf) {
      dp.emplace_back(kKindOfInf);
    }
  }
  std::cout << dp.size() - 2;
  return 0;
}