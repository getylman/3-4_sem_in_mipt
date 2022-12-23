#include <iostream>
#include <vector>
// A
struct GeomTask {
 private:
  class Seg;
  class Vec;
  // std::pair<int64_t, int64_t> pok_ = {0, 0};
  // std::vector<std::pair<int64_t, int64_t>> fld_ = {};

 public:
  GeomTask(const std::vector<std::pair<int64_t, int64_t>>& fld_,
           const std::pair<int64_t, int64_t>& pok_) {
    bool flag = false;
    for (size_t i = 0; i < fld_.size(); ++i) {
      flag = Seg(fld_[InRing(i, fld_.size())], fld_[InRing(i + 1, fld_.size())])
                 .ContainsPoint(pok_);
    }
    if (flag) {
      std::cout << "YES";
    } else {
      flag = false;
      for (size_t i = 0, j = fld_.size() - 1; i < fld_.size(); j = i++) {
        if ((((fld_[i].second <= pok_.second) &&
              (pok_.second < fld_[j].second)) ||
             ((fld_[j].second <= pok_.second) &&
              (pok_.second < fld_[i].second))) &&
            (((fld_[j].second - fld_[i].second) != 0) &&
             ((double)pok_.first > ((double)(fld_[j].first - fld_[i].first) *
                                        (pok_.second - fld_[i].second) /
                                        (fld_[j].second - fld_[i].second) +
                                    fld_[i].first)))) {
          flag = !flag;
        }
      }
      std::cout << ((flag) ? "YES" : "NO");
    }
  }

 private:
  static size_t InRing(const int64_t& num, const size_t& mod) {
    return (mod + num) % mod;
  }
  class Seg {
   public:
    Seg(const std::pair<int64_t, int64_t>& strt,
        const std::pair<int64_t, int64_t>& nnd)
        : beg_(strt), nd_(nnd) {}
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

   private:
    std::pair<int64_t, int64_t> beg_;
    std::pair<int64_t, int64_t> nd_;
  };

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
};

int main() {
  size_t size = 0;
  std::cin >> size;
  std::vector<std::pair<int64_t, int64_t>> field(size);
  std::pair<int64_t, int64_t> pok;
  std::cin >> pok.first >> pok.second;
  size_t id = 0;
  while (id++ < size) {
    std::cin >> field[id - 1].first >> field[id - 1].second;
  }
  GeomTask g_t(field, pok);
}