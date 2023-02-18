#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

void PrintResult(std::vector<std::vector<std::pair<int32_t, int32_t>>>&
                     intermediate_results) {
  int32_t index_of_res = 0;
  int32_t index_for_mask = 0;
  int32_t minimal_res =
      intermediate_results[intermediate_results.size() - 1][0].first;
  for (size_t i = 0; i < intermediate_results[0].size(); ++i) {
    if (intermediate_results[intermediate_results.size() - 1][i].first <
        minimal_res) {
      minimal_res =
          intermediate_results[intermediate_results.size() - 1][i].first;
      index_of_res = i;
    }
  }
  std::cout << minimal_res << '\n';
  std::vector<int32_t> result;
  result.reserve(intermediate_results[0].size());
  index_for_mask = intermediate_results.size() - 1;
  while (index_of_res != -2) {
    result.push_back(index_of_res + 1);
    int32_t hold_index = index_of_res;
    index_of_res = intermediate_results[index_for_mask][index_of_res].second;
    index_for_mask &= ~(1 << hold_index);
  }
  for (size_t i = 0; i < result.size(); ++i) {
    std::cout << result[i] << '\n';
  }
}

int main() {
  int32_t n = 0;
  std::cin >> n;
  std::vector<std::vector<std::pair<int32_t, int32_t>>> intermediate_results;
  std::vector<std::vector<int32_t>> input;
  intermediate_results.resize((1 << n));
  for (size_t i = 0; i < intermediate_results.size(); ++i) {
    intermediate_results[i].resize(n,
                                   {std::numeric_limits<int32_t>::max(), -2});
  }
  input.resize(n);
  for (int32_t i = 0; i < n; ++i) {
    input[i].resize(n);
  }
  for (int32_t i = 0; i < n; ++i) {
    intermediate_results[1 << i][i].first = 0;
    intermediate_results[0][i].first = 0;
  }
  int32_t interm = 0;
  for (int32_t i = 0; i < n; ++i) {
    for (int32_t j = 0; j < n; ++j) {
      std::cin >> interm;
      input[i][j] = interm;
    }
  }
  for (int32_t mask = 1; mask < (1 << n); ++mask) {
    for (int32_t u = 0; u < n; ++u) {
      if (((1 << u) & mask) == (1 << u)) {
        continue;
      }
      for (int32_t v = 0; v < n; ++v) {
        if (((1 << v) & mask) == 0) {
          continue;
        }
        if (intermediate_results[mask][v].first + input[v][u] <
            intermediate_results[mask | (1 << u)][u].first) {
          intermediate_results[mask | (1 << u)][u].first =
              intermediate_results[mask][v].first + input[v][u];
          intermediate_results[mask | (1 << u)][u].second = v;
        }
      }
    }
  }
  PrintResult(intermediate_results);
}

/*
6
0 10 62 4 134 5
10 0 33 24 55 14
62 33 0 16 55 15
4 24 16 0 2 55 55
134 55 55 2 0 1e6
5 14 15 55 1e6 0
*/