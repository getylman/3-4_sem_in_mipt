#include <algorithm>
#include <iostream>
#include <vector>
// P
class Sol {
 public:
  Sol(const std::vector<std::pair<int64_t, int64_t>>& sop) : sop_(sop) {
  }
  void ToMakeSol() {
    size_t id1 = 0;
    size_t id2 = 0;
    const size_t kLen = sop_.size();
    size_t id3 = 0;
    sop_.push_back({0, 0});
    std::vector<uint64_t> vec_;
    vec_.reserve(kLen);
    while (id1++ < kLen) {
      id2 = 0;
      while (id2++ < kLen) {
        if (id1 != id2) {
          vec_.push_back(
              (uint64_t)((sop_[id1 - 1].second - sop_[id2 - 1].second) *
                         (sop_[id1 - 1].second - sop_[id2 - 1].second)) +
              ((sop_[id1 - 1].first - sop_[id2 - 1].first) *
               (sop_[id1 - 1].first - sop_[id2 - 1].first)));
        }
      }
      id3 = 0;
      id2 = 0;
      std::sort(vec_.begin(), vec_.end());
      while (id2++ < vec_.size()) {
        if (vec_[id2 - 1] != vec_[id3 - 1]) {
          id3 = id2;
        }
        num_of_tr += (id2 - id3);
      }
      vec_.clear();
    }
    std::cout << num_of_tr;
  }

 private:
  std::vector<std::pair<int64_t, int64_t>> sop_;
  size_t num_of_tr = 0;
};

int main() {
  size_t size = 0;
  char com;
  std::cin >> size >> com;
  // sop -> set of points
  std::vector<std::pair<int64_t, int64_t>> sop(size);
  size_t id1 = 0;
  while (id1++ < size) {
    std::cin >> sop[id1 - 1].first >> sop[id1 - 1].second;
  }
  if (com == '2') {
    printf("0");
    return 0;
  }
  Sol sol(sop);
  sol.ToMakeSol();
}