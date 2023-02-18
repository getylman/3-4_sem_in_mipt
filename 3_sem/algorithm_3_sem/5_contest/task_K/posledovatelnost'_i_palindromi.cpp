#include <iostream>
#include <vector>

class Dp {
 public:
  Dp(const std::vector<int32_t>& vec) : body_(vec), kSizeVec(vec.size()) {}
  void ToBuildDp() {
    dp_.resize(kSizeVec + 1, std::vector<int64_t>(kSizeVec + 1));
    body_.push_back(-1);
    for (size_t i = 0; i < kSizeVec; ++i) {
      dp_[i][i] = 1;
      dp_[i][i + 1] = 2 + (size_t)(body_[i] == body_[i + 1]);
    }
    int64_t sum = 0;
    if (kSizeVec > 2) {
      for (size_t i = kSizeVec - 3; i < kSizeVec; --i) {
        for (size_t j = i + 2; j < kSizeVec; ++j) {
          sum = InRing(dp_[i + 1][j]);
          sum += InRing(dp_[i][j - 1]);
          sum = InRing(sum);
          sum += ((body_[i] == body_[j]) ? (1) : (-(int64_t)dp_[i + 1][j - 1]));
          dp_[i][j] = InRing(sum);
        }
      }
    }
  }
  void ToPrintDp() const {
    for (size_t i = 0; i < dp_.size(); ++i) {
      for (size_t j = 0; j < dp_[i].size(); ++j) {
        std::cout << dp_[i][j] << ' ';
      }
      std::cout << '\n';
    }
  }
  void ToPrintAns() const { std::cout << dp_[0][kSizeVec - 1]; }

 private:
  std::vector<std::vector<int64_t>> dp_;
  std::vector<int32_t> body_;
  const int kMagicNumber = 1e9;
  const size_t kSizeVec = 0;
  int64_t InRing(const int64_t& num) const {
    int64_t tmp_num = num;
    while (tmp_num <= 0) {
      tmp_num += kMagicNumber;
    }
    return tmp_num % kMagicNumber;
  }
};

int main() {
  size_t size = 0;
  std::cin >> size;
  std::vector<int32_t> poks(size);
  size_t id = 0;
  while (id++ < size) {
    std::cin >> poks[id - 1];
  }
  Dp task(poks);
  task.ToBuildDp();
  task.ToPrintAns();
}