#include <iostream>
#include <vector>

class Dp {
 public:
  Dp(const std::vector<uint16_t>& vec) : body_(vec) {}
  void ToMakeSol() {
    const unsigned short kLen = body_.size();
    dp_.resize(kLen + 1, std::vector<double>(kLen + 1));
    unsigned short id1 = 0;
    while (id1++ < kLen) {
      dp_[id1 - 1][id1 - 1] = body_[id1 - 1];
    }
    unsigned short id2 = 0;
    id1 = 0;
    double sum = 0;
    while (++id1 < kLen) {
      for (unsigned short i = 0, j = id1; i < kLen and j < kLen; ++i, ++j) {
        if (i == j) {
          continue;
        }
        id2 = 0;
        sum = 0;
        while (id2++ < kLen) {
          sum = (sum > (dp_[id2 - 1][i] + dp_[j][id2]) / 2)
                    ? sum
                    : ((dp_[id2 - 1][i] + dp_[j][id2]) / 2);
        }
        dp_[j][i] = sum;
      }
    }
    std::cout << std::fixed;
    std::cout.precision(6);
    std::cout << dp_[kLen - 1][0];
  }

 private:
  std::vector<unsigned short> body_;
  std::vector<std::vector<double>> dp_;
};

int main() {
  size_t size = 0;
  std::cin >> size;
  std::vector<unsigned short> nums(size);
  size_t ind = 0;
  while (ind++ < size) {
    std::cin >> nums[ind - 1];
  }
  Dp task(nums);
  task.ToMakeSol();
}