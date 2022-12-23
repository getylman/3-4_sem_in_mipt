#include <algorithm>
#include <iostream>
#include <vector>

int main() {
  size_t size = 0;
  size_t max_cap = 0;
  std::cin >> size >> max_cap;
  std::vector<size_t> mass(size + 1, 0);
  std::vector<size_t> price(size + 1, 0);
  for (size_t i = 1; i <= size; ++i) {
    std::cin >> mass[i];
  }
  for (size_t i = 1; i <= size; ++i) {
    std::cin >> price[i];
  }
  std::vector<std::vector<std::pair<size_t, std::vector<size_t>>>> dp(
      size + 1, std::vector<std::pair<size_t, std::vector<size_t>>>(
                    max_cap + 1, {0, std::vector<size_t>()}));
  for (size_t i = 1; i <= size; ++i) {
    for (size_t j = 1; j <= max_cap; ++j) {
      dp[i][j] = dp[i - 1][j];
      if (j >= mass[i] &&
          dp[i - 1][j - mass[i]].first + price[i] > dp[i][j].first) {
        dp[i][j].first = dp[i - 1][j - mass[i]].first + price[i];
        dp[i][j].second = dp[i - 1][j - mass[i]].second;
        dp[i][j].second.push_back(i);
      }
    }
  }
  for (size_t i = 0; i < dp[size][max_cap].second.size(); ++i) {
    std::cout << dp[size][max_cap].second[i] << '\n';
  }
}