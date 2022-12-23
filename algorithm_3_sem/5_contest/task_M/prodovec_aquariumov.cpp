#include <iostream>
#include <limits>
#include <stack>
#include <vector>

class Dp {
 public:
  Dp(const std::vector<std::vector<uint32_t>>& vec)
      : body_(vec), kLen(vec.size()) {}
  void ToMakeSol() {
    const uint32_t k2inkLen = 1 << kLen;
    dp_.resize(kLen, std::vector<std::pair<size_t, int>>(
                         k2inkLen, {std::numeric_limits<int32_t>::max(), -1}));
    uint32_t id1 = 0;
    while (id1++ < kLen) {
      dp_[id1 - 1][1 << (id1 - 1)] = {0, -1};
    }
    uint32_t tmp_mask = 0;
    for (uint32_t i = 1; i < k2inkLen; ++i) {
      for (uint32_t j = 0; j < kLen; ++j) {
        id1 = 0;
        while (id1++ < kLen) {
          if ((i & (1 << (id1 - 1))) == 1) {
            continue;
          }
          tmp_mask = i | (1 << (id1 - 1));
          if (dp_[id1 - 1][tmp_mask].first >
              body_[j][id1 - 1] + dp_[j][i].first) {
            dp_[id1 - 1][tmp_mask] = {body_[j][id1 - 1] + dp_[j][i].first, j};
          }
        }
      }
    }
    std::pair<size_t, size_t> ans = {1e15, 0};
    for (size_t i = 0; i < kLen; ++i) {
      if (ans.first > dp_[i][k2inkLen - 1].first) {
        ans = {dp_[i][k2inkLen - 1].first, i};
      }
    }
    std::cout << ans.first << '\n';
    // std::cout << dp_[ans.second][k2inkLen - 1].second;
    ToSerchAns(ans.second, k2inkLen - 1);
    while (!way_.empty()) {
      std::cout << way_.top() + 1 << ' ';
      way_.pop();
    }
  }

 private:
  std::vector<std::vector<uint32_t>> body_;
  std::vector<std::vector<std::pair<size_t, int>>> dp_;
  std::stack<size_t> way_;
  const uint16_t kLen = 0;
  const size_t kKindOfInf = 1e15;
  void ToSerchAns(size_t last, uint32_t mask) {
    if (mask == 0) {
      return;
    }
    way_.push(last);
    ToSerchAns(dp_[last][mask].second, mask & (~(1 << last)));
  }
};

int main() {
  size_t size = 0;
  std::cin >> size;
  std::vector<std::vector<uint32_t>> cities(size, std::vector<uint32_t>(size));
  size_t id1 = 0;
  size_t id2 = 0;
  while (id1++ < size) {
    id2 = 0;
    while (id2++ < size) {
      std::cin >> cities[id1 - 1][id2 - 1];
    }
  }
  Dp task(cities);
  task.ToMakeSol();
}