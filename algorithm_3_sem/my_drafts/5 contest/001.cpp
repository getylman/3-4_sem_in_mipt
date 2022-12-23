#include <iostream>
#include <vector>

void print(size_t i, size_t j,
           const std::vector<std::vector<std::pair<size_t, size_t>>>& ids,
           const std::vector<int>& arr) {
  if (i != 0 && j != 0) {
    if (ids[i][j] == std::make_pair(i - 1, j - 1)) {
      print(i - 1, j - 1, ids, arr);
      std::cout << arr[i] << ' ';
    } else {
      if (ids[i][j] == std::make_pair(i - 1, j)) {
        print(i - 1, j, ids, arr);
      } else {
        print(i, j - 1, ids, arr);
      }
    }
  }
}

int main() {
  size_t size1 = 0;
  size_t size2 = 0;
  std::cin >> size1;
  std::vector<int> arr1(size1 + 1, 0);
  std::vector<int> arr2(size2 + 1, 0);
  for (size_t i = 1; i <= size1; ++i) {
    std::cin >> arr1[i];
  }
  std::cin >> size2;
  for (size_t i = 1; i <= size2; ++i) {
    std::cin >> arr2[i];
  }
  std::vector<std::vector<int>> dp(size1 + 1, std::vector<int>(size2 + 1, 0));
  std::vector<std::vector<std::pair<size_t, size_t>>> ids(
      size1 + 1, std::vector<std::pair<size_t, size_t>>(size2 + 1, {0, 0}));
  for (size_t i = 1; i <= size1; ++i) {
    for (size_t j = 1; j <= size2; ++j) {
      if (arr1[i] == arr2[j]) {
        dp[i][j] = dp[i - 1][j - 1] + 1;
        ids[i][j] = {i - 1, j - 1};
      } else {
        if (dp[i - 1][j] >= dp[i][j - 1]) {
          dp[i][j] = dp[i - 1][j];
          ids[i][i] = {i - 1, j};
        } else {
          dp[i][j] = dp[i][j - 1];
          ids[i][i] = {i, j - 1};
        }
      }
    }
  }
  print(size1, size2, ids, arr1);
  return 0;
}