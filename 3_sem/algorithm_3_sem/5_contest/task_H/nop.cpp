#include <iostream>
#include <vector>

class Dp {
 public:
  Dp(const std::vector<int>& arr1, const std::vector<int>& arr2)
      : arr1_(arr1), arr2_(arr2) {}
  void ToBuildSol() {
    ids_.resize(arr1_.size(),
                std::vector<std::pair<size_t, size_t>>(arr2_.size(), {0, 0}));
    dp_.resize(arr1_.size(), std::vector<size_t>(arr2_.size(), 0));
    for (size_t i = 1; i < arr1_.size(); ++i) {
      for (size_t j = 1; j < arr2_.size(); ++j) {
        dp_[i][j] = (arr1_[i] != arr2_[j])
                        ? ((dp_[i - 1][j] > dp_[i][j - 1]) ? dp_[i - 1][j]
                                                           : dp_[i][j - 1])
                        : (dp_[i - 1][j - 1] + 1);
        ids_[i][j] = (arr1_[i] != arr2_[j]) ? ((dp_[i - 1][j] > dp_[i][j - 1])
                                                   ? std::make_pair(i - 1, j)
                                                   : std::make_pair(i, j - 1))
                                            : (std::make_pair(i - 1, j - 1));
      }
    }
  }
  void ToPrintSol() {
    answer_.reserve(std::max(arr1_.size(), arr2_.size()) + 1);
    ToBuildAns(arr1_.size() - 1, arr2_.size() - 1);
    size_t ind = 0;
    while (ind < answer_.size()) {
      std::cout << answer_[ind++] << ' ';
    }
  }
  void ToPrintDp() {
    for (size_t i = 0; i < dp_.size(); ++i) {
      for (size_t j = 0; j < dp_[i].size(); ++j) {
        std::cout << dp_[i][j] << ' ';
      }
      std::cout << '\n';
    }
  }

 private:
  std::vector<std::vector<std::pair<size_t, size_t>>> ids_;
  std::vector<int> answer_;
  std::vector<std::vector<size_t>> dp_;
  std::vector<int> arr1_;
  std::vector<int> arr2_;
  void ToBuildAns(size_t col, size_t row) {
    if (col > 0 && row > 0) {
      if (ids_[col][row] == std::make_pair(col - 1, row - 1)) {
        ToBuildAns(col - 1, row - 1);
        answer_.push_back(arr1_[col]);
      } else {
        ToBuildAns(
            ((ids_[col][row] != std::make_pair(col - 1, row)) ? col : col - 1),
            ((ids_[col][row] != std::make_pair(col - 1, row)) ? row - 1 : row));
      }
    }
  }
};

int main() {
  size_t size1 = 0;
  size_t size2 = 0;
  std::cin >> size1;
  std::vector<int> arr1(size1 + 1, 0);
  for (size_t i = 1; i <= size1; ++i) {
    std::cin >> arr1[i];
  }
  std::cin >> size2;
  std::vector<int> arr2(size2 + 1, 0);
  for (size_t i = 1; i <= size2; ++i) {
    std::cin >> arr2[i];
  }
  Dp ans(arr1, arr2);
  ans.ToBuildSol();
  ans.ToPrintSol();
  return 0;
}