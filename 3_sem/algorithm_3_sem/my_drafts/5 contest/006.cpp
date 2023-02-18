#include <iostream>
#include <vector>
// A
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
    ans = ((((points[i].second <= pokemon.second) &&
             (pokemon.second < points.back().second)) ||
            ((points.back().second <= pokemon.second) &&
             (pokemon.second < points[i].second))) &&
           (((points.back().second - points[i].second) != 0) &&
            (pokemon.first > ((points.back().first - points[i].first) *
                                  (pokemon.second - points[i].second) /
                                  (points.back().second - points[i].second) +
                              points[i].first))))
              ? !ans
              : ans;
  }
  std::cout << ((ans) ? "YES" : ((flag) ? "YES" : "NO"));
}