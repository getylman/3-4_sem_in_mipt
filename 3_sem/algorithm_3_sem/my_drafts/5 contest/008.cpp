#include <math.h>

#include <algorithm>
#include <iostream>
#include <vector>
// C
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

int64_t Dist(const std::pair<int64_t, int64_t>& lft,
             const std::pair<int64_t, int64_t>& rht) {
  int64_t delta_x = rht.first - lft.first;
  int64_t delta_y = rht.second - lft.second;
  return (delta_x * delta_x + delta_y * delta_y);
}

class Cmp {
 public:
  Cmp(const std::pair<int64_t, int64_t>& b_p) : b_p_(b_p) {}
  bool operator()(std::pair<int64_t, int64_t> lft,
                  std::pair<int64_t, int64_t> rht) const {
    std::pair<int64_t, int64_t> tbp = b_p_;
    double l_c =
        (double)((lft.first - tbp.first) * (lft.first - tbp.first)) /
        Dist(tbp, lft);
    double r_c =
        (double)((rht.first - tbp.first) * (rht.first - tbp.first)) /
        Dist(tbp, rht);
    // l_c -> left comparable same meaning with r_c
    return ((l_c == r_c) ? (Dist(tbp, lft) < Dist(tbp, rht)) : (l_c < r_c));
  }

 private:
  std::pair<int64_t, int64_t> b_p_;
};

class Task {
 public:
  Task(const std::vector<std::pair<int64_t, int64_t>>& sop) : sop_(sop) {}
  void ToBuildCHP() {
    b_p_ = {kKindOfInf, kKindOfInf};
    for (const auto& i : sop_) {
      b_p_ = ((i.first == b_p_.first)
                  ? ((i.second < b_p_.second) ? (i) : (b_p_))
                  : ((i.first < b_p_.first) ? (i) : (b_p_)));
    }
    std::sort(sop_.begin(), sop_.end(), Cmp(b_p_));
    chp_.reserve(sop_.size());
    chp_.push_back(sop_[0]);
    chp_.push_back(sop_[1]);
    for (size_t i = 2; i < sop_.size(); ++i) {
      while ((chp_.size() > 1) &&
             ((Vec(PLE(chp_), chp_.back()) ^ Vec(chp_.back(), sop_[i])) <= 0)) {
        chp_.pop_back();
      }
      chp_.push_back(sop_[i]);
    }
  }
  void PrintBPnSOP() const {
    std::cout << b_p_.first << ' ' << b_p_.second << '\n';
    std::cout << "--------\n";
    for (const auto& i : sop_) {
      std::cout << i.first << ' ' << i.second << '\n';
    }
  }
  void PrintCHP() const {
    std::cout << chp_.size() << '\n';
    for (const auto& i : chp_) {
      std::cout << i.first << ' ' << i.second << '\n';
    }
  }
  void SolveArea() const {
    int64_t area = 0;
    for (size_t i = 0; i < chp_.size() - 1; ++i) {
      area += chp_[i].first * chp_[i + 1].second -
              chp_[i + 1].first * chp_[i].second;
    }
    std::string f_p;  // f_p -> fractional part
    f_p = (area & 1 == 1) ? ".5" : ".0";
    std::cout << abs(area / 2) << f_p;
  }

 private:
  std::vector<std::pair<int64_t, int64_t>> sop_;  // sop -> set of points
  std::vector<std::pair<int64_t, int64_t>> chp_;  // chp -> convex hull points
  std::pair<int64_t, int64_t> b_p_;               // b_p_ -> bottom point
  const int64_t kKindOfInf = 1e12;                // kind of infinity
  static std::pair<int64_t, int64_t> PLE(
      std::vector<std::pair<int64_t, int64_t>>& chp) {
    // PLE -> prelast element
    std::pair<int64_t, int64_t> last = chp.back();
    chp.pop_back();
    std::pair<int64_t, int64_t> pre_last = chp.back();
    chp.push_back(last);
    return pre_last;
  }
};

int main() {
  size_t size = 0;
  std::cin >> size;
  std::vector<std::pair<int64_t, int64_t>> sop;  // sop -> set of points
  sop.reserve(size);
  int64_t x_c = 0;
  int64_t y_c = 0;
  size_t id = 0;
  while (id++ < size) {
    std::cin >> x_c >> y_c;
    sop.push_back({x_c, y_c});
  }
  Task task(sop);
  task.ToBuildCHP();
  task.PrintBPnSOP();
  task.PrintCHP();
  task.SolveArea();
}