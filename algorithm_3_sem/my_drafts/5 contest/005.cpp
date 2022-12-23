#include <iostream>
#include <vector>
// B

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

/*
#include <iostream>
#include <vector>

bool RangeIntersec(int64_t comp_beg_1, int64_t comp_nd_1, int64_t comp_beg_2,
                   int64_t comp_nd_2) {
  if (comp_beg_1 > comp_nd_1) {
    std::swap(comp_beg_1, comp_nd_1);
  }
  if (comp_beg_2 > comp_nd_2) {
    std::swap(comp_beg_2, comp_nd_2);
  }
  return std::max(comp_beg_1, comp_beg_2) <= std::min(comp_nd_1, comp_nd_2);
  // эту функцию я взял у себя же из контеста по плюсам
}

class Vec {
 public:
  Vec() : body_({0, 0}) {}
  Vec(const std::pair<int64_t, int64_t>& b_v,
      const std::pair<int64_t, int64_t>& e_v) {
    body_.first = e_v.first - b_v.first;
    body_.second = e_v.second - b_v.second;
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

class Seg {
 public:
  Seg() : beg_({0, 0}), nd_({0, 0}) {}
  Seg(const std::pair<int64_t, int64_t>& strt,
      const std::pair<int64_t, int64_t>& fnsh)
      : beg_(strt), nd_(fnsh) {}
  bool ContainsPoint(const std::pair<int64_t, int64_t>& pnt) const {
    Vec p_to_a(pnt, beg_);
    Vec p_to_b(pnt, nd_);
    if ((p_to_a ^ p_to_b) == 0) {
      return (std::min(beg_.first, nd_.first) <= pnt.first &&
              std::max(beg_.first, nd_.first) >= pnt.first) &&
             (std::min(beg_.second, nd_.second) <= pnt.second &&
              std::max(beg_.second, nd_.second) >= pnt.second);
    }
    return false;
  }
  bool CrossSegment(const Seg& sgmnt) const {
    if (RangeIntersec(beg_.first, nd_.first, sgmnt.beg_.first,
                      sgmnt.nd_.first) &&
        RangeIntersec(beg_.second, nd_.second, sgmnt.beg_.second,
                      sgmnt.nd_.second)) {
      Vec vec_of_this(beg_, nd_);
      Vec vec_of_sgmnt(sgmnt.beg_, sgmnt.nd_);
      return ((vec_of_this ^ Vec(beg_, sgmnt.beg_)) *
                  (vec_of_this ^ Vec(beg_, sgmnt.nd_)) <=
              0) &&
             ((vec_of_sgmnt ^ Vec(sgmnt.beg_, beg_)) *
                  (vec_of_sgmnt ^ Vec(sgmnt.beg_, nd_)) <=
              0);
    }
    return false;
  }

 private:
  std::pair<int64_t, int64_t> beg_;
  std::pair<int64_t, int64_t> nd_;
};

size_t InRing(const int64_t& num, const size_t& mod) {
  return (mod + num) % mod;
}

int main() {
  size_t num_of_p = 0;
  std::cin >> num_of_p;
  std::vector<std::pair<int64_t, int64_t>> points(num_of_p);
  std::pair<int64_t, int64_t> pokemon;
  std::cin >> pokemon.first >> pokemon.second;
  bool flag = false;
  for (size_t i = 0; i < num_of_p; ++i) {
    std::cin >> points[i].first >> points[i].second;
    flag = (pokemon == points[i]) ? true : flag;
  }
  const int64_t kBigNum = 1e9;
  Seg ray(pokemon, std::make_pair(pokemon.first + 1, kBigNum));
  Seg sgmnt;
  bool ans = false;
  for (size_t i = 0; i < num_of_p; ++i) {
    sgmnt = Seg(points[InRing(i, num_of_p)], points[(InRing(i + 1, num_of_p))]);
    flag = (sgmnt.ContainsPoint(pokemon) ? true : flag);
    ans = (ray.CrossSegment(sgmnt)) ? !ans : ans;
  }
  std::cout << ((flag) ? "YES" : ((ans) ? "YES" : "NO"));
}
*/



/*
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
        if (arr1_[i] != arr2_[j]) {
          if (dp_[i - 1][j] > dp_[i][j - 1]) {
            dp_[i][j] = dp_[i - 1][j];
            ids_[i][i] = {i - 1, j};
          } else {
            dp_[i][j] = dp_[i][j - 1];
            ids_[i][i] = {i, j - 1};
          }
        } else {
          dp_[i][j] = dp_[i - 1][j - 1] + 1;
          ids_[i][j] = {i - 1, j - 1};
        }
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
*/