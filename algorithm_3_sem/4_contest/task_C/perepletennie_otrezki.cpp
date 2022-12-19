#include <algorithm>
#include <iostream>
#include <vector>

class FeNweekENd {
 public:
  FeNweekENd(const int& size) { telo_.resize(size); }
  int ToMakeASum(int l, int r) { return ToMakeASum(r - 1) - ToMakeASum(l - 1); }
  void Add(const int& r, const int& delta) {
    for (int i = r; i < (int)telo_.size(); i = BitOr(i)) {
      telo_[i] += delta;
    }
  }

 private:
  std::vector<int> telo_;
  int BitOr(const int& num) const { return (num | (num + 1)); }
  int BitAnd(const int& num) const { return (num & (num + 1)); }
  int ToMakeASum(const int& r) {
    int result = 0;
    for (int i = r; i >= 0; i = BitAnd(i) - 1) {
      result += telo_[i];
    }
    return result;
  }
};

bool LengthComp(const std::pair<int, int>& a, const std::pair<int, int>& b) {
  if ((a.second - a.first) == (b.second - b.first)) {
    return a.first < b.first;
  }
  return (a.second - a.first) < (b.second - b.first);
}

int BS(const int* b, const int* e, const int& target) {
  const int* s = b;
  while (b != e - 1) {
    const int* md = b + (e - b) / 2;
    if (target >= *md) {
      b = md;
    } else {
      e = md;
    }
  }
  return b - s;
}
int main() {
  const size_t kBigNum = 600000;
  int size;
  int64_t l, r;
  std::cin >> size;
  FeNweekENd left(kBigNum);
  FeNweekENd right(kBigNum);
  uint64_t res = 0;
  uint64_t bord_c, prev_res = 0;
  uint64_t bord_a = 0;
  uint64_t bord_ab = 0;
  uint64_t bord_b = 0;
  uint64_t bord_bc = 0;
  std::vector<std::pair<int, int>> i_npu_t;
  std::vector<int> c_p_s;
  i_npu_t.reserve(size);
  for (int i = 0; i < size; ++i) {
    std::cin >> l >> r;
    i_npu_t.push_back({l, r});
  }
  for (const auto& iter : i_npu_t) {
    c_p_s.push_back(iter.first);
    c_p_s.push_back(iter.second);
  }
  std::sort(i_npu_t.begin(), i_npu_t.end(), LengthComp);
  std::sort(c_p_s.begin(), c_p_s.end());
  for (auto& it : i_npu_t) {
    it.first = BS(&c_p_s[0], &c_p_s[0] + 2 * size, it.first);
    it.second = BS(&c_p_s[0], &c_p_s[0] + 2 * size, it.second);
  }
  left.Add(i_npu_t[0].first, 1);
  right.Add(i_npu_t[0].second, 1);
  for (size_t i = 1; i < i_npu_t.size(); ++i) {
    l = i_npu_t[i].first;
    r = i_npu_t[i].second;
    std::pair<int, int> prev = i_npu_t[i - 1], cur = i_npu_t[i];
    if (prev.first != cur.first || prev.second != cur.second) {
      bord_a = right.ToMakeASum(0, l + 1);
      bord_ab = left.ToMakeASum(0, l);
      bord_b = bord_ab - bord_a;
      bord_bc = right.ToMakeASum(l + 1, r + 1);
      bord_c = bord_bc - bord_b;
      res += bord_c;
      prev_res = bord_c;
    } else {
      res += prev_res;
    }
    left.Add(i_npu_t[i].first, 1);
    right.Add(i_npu_t[i].second, 1);
  }
  std::cout << res << "\n";
}