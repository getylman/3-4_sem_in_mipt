#include <iostream>
#include <vector>

class TripleFen {
 public:
  TripleFen(const size_t& size) : size_(size) {
    body_.resize(size, std::vector<std::vector<int64_t>>(
                           size, std::vector<int64_t>(size, 0)));
  }
  void UpDate(const int& x, const int& y, const int& z, const int64_t& delta) {
    for (int i = x; i < size_; i = BitOr(i)) {
      for (int j = y; j < size_; j = BitOr(j)) {
        for (int k = z; k < size_; k = BitOr(k)) {
          body_[i][j][k] += delta;
        }
      }
    }
  }
  int64_t GetSum(const int& x, const int& y, const int& z) const {
    int64_t res = 0;
    for (int i = x; i >= 0; i = BitAnd(i) - 1) {
      for (int j = y; j >= 0; j = BitAnd(j) - 1) {
        for (int k = z; k >= 0; k = BitAnd(k) - 1) {
          res += body_[i][j][k];
        }
      }
    }
    return res;
  }

 private:
  std::vector<std::vector<std::vector<int64_t>>> body_;
  int size_ = 0;
  static int BitOr(const int& num) { return (num | (num + 1)); }
  static int BitAnd(const int& num) { return (num & (num + 1)); }
};

int main() {
  size_t size = 0;
  std::cin >> size;
  TripleFen t(size);
  uint32_t com = 0;
  const uint32_t kToUpdate = 1;
  const uint32_t kToGetSum = 2;
  const uint32_t kToDie = 3;
  int x1 = 0;
  int x2 = 0;
  int y1 = 0;
  int y2 = 0;
  int z1 = 0;
  int z2 = 0;
  int64_t delta = 0;
  while (com != kToDie) {
    std::cin >> com;
    if (com == kToUpdate) {
      std::cin >> x1 >> y1 >> z1 >> delta;
      t.UpDate(x1, y1, z1, delta);
    } else if (com == kToGetSum) {
      std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
      delta = t.GetSum(x2, y2, z2) - t.GetSum(x1 - 1, y2, z2) -
              t.GetSum(x2, y1 - 1, z2) - t.GetSum(x2, y2, z1 - 1) +
              t.GetSum(x1 - 1, y1 - 1, z2) + t.GetSum(x1 - 1, y2, z1 - 1) +
              t.GetSum(x2, y1 - 1, z1 - 1) - t.GetSum(x1 - 1, y1 - 1, z1 - 1);
      std::cout << delta << '\n';
    }
  }
}
