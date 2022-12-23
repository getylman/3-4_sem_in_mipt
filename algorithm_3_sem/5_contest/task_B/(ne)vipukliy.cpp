#include <iostream>
#include <vector>

class Vec {
 public:
  Vec() : body_({0, 0}) {}
  Vec(const std::pair<int64_t, int64_t>& b_v,
      const std::pair<int64_t, int64_t>& e_v) {
    body_.first = e_v.first - b_v.first;
    body_.second = e_v.second - e_v.second;
  }
  Vec& operator=(const Vec& vec) {
    body_ = vec.body_;
    return *this;
  }
  int64_t operator^(const Vec& vec) {
    return body_.first * vec.body_.second - vec.body_.first * body_.second;
  }

 private:
  std::pair<int64_t, int64_t> body_;
};

size_t InRing(const int64_t& num, const size_t& mod) {
  return (mod + num) % mod;
}

int main() {
  size_t num_of_p = 0;
  std::cin >> num_of_p;
  std::vector<std::pair<int64_t, int64_t>> points(num_of_p);
  for (size_t i = 0; i < num_of_p; ++i) {
    std::cin >> points[i].first >> points[i].first;
  }
  Vec vec1;
  Vec vec2;
  Vec vec3;
  for (size_t i = 0; i < num_of_p; ++i) {
    vec1 = Vec(points[InRing(i, num_of_p)], points[InRing(i + 1, num_of_p)]);
    vec2 =
        Vec(points[InRing(i + 1, num_of_p)], points[InRing(i + 2, num_of_p)]);
    vec3 =
        Vec(points[InRing(i + 2, num_of_p)], points[InRing(i + 3, num_of_p)]);
    if ((vec1 ^ vec2) * (vec2 ^ vec3) < 0) {
      std::cout << "NO";
      return 0;
    }
  }
  std::cout << "YES";
}