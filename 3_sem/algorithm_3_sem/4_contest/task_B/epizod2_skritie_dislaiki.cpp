#include <iostream>
#include <vector>

class DO {
 public:
  DO(const uint32_t& size) : size_(size) {
    while (two_in_power_ < size) {
      two_in_power_ *= 2;
    }
    body_.reserve(2 * two_in_power_);
  }
  void Insert(const int& elem) { body_.push_back(elem); }
  void ToBuildTree() {
    while (body_.size() < two_in_power_) {
      body_.push_back(kNeitralElem);
    }
    size_t id = 0;
    for (size_t i = 0; i < body_.size(); ++i) {
      l_r_.push_back({i, i + 1});
      kids_.push_back({-1, -1});
    }
    while (true) {
      if (id + 1 == body_.size()) {
        break;
      }
      body_.push_back(body_[id] + body_[id + 1]);
      l_r_.push_back({l_r_[id].first, l_r_[id + 1].second});
      kids_.push_back({id, id + 1});
      id += 2;
    }
  }
  void PrintDO() {
    for (size_t i = 0; i < body_.size(); ++i) {
      std::cout << body_[i] << ' ' << l_r_[i].first << ' ' << l_r_[i].second
                << '\n';
    }
  }
  void Set(size_t id, const int& new_val) {
    const int kDelta = new_val - body_[id];
    body_[id] = new_val;
    size_t dist = body_.size() / 2;
    size_t way = dist + 1;
    while (true) {
      if (dist <= 1) {
        break;
      }
      body_[way + id / 2] += kDelta;
      ++dist /= 2;
      id /= 2;
      way += dist;
    }
  }
  int GetSum(const size_t& lt, const size_t& rt) {
    return Sum(body_.size() - 1, lt, rt);
  }

 private:
  std::vector<int> body_;
  std::vector<std::pair<size_t, size_t>> l_r_;
  std::vector<std::pair<int, int>> kids_;
  const uint16_t kNeitralElem = 0;
  uint32_t two_in_power_ = 1;
  uint32_t size_ = 0;

  int Sum(size_t id, size_t ltx, size_t rtx) {
    if (l_r_[id].first >= rtx || ltx >= l_r_[id].second) {
      return 0;
    }
    if (ltx <= l_r_[id].first && rtx >= l_r_[id].second) {
      return body_[id];
    }
    int s1 = ((kids_[id].first == -1) ? 0 : Sum(kids_[id].first, ltx, rtx));
    int s2 = ((kids_[id].second == -1) ? 0 : Sum(kids_[id].second, ltx, rtx));
    return s1 + s2;
  }
};

int main() {
  uint32_t size = 0;
  std::cin >> size;
  DO t1(size);
  DO t2(size);
  int elem = 0;
  for (size_t i = 0; i < size; ++i) {
    std::cin >> elem;
    t1.Insert(elem * (((i & 1) == 1) ? 1 : -1));
    t2.Insert(elem * (((i & 1) == 1) ? -1 : 1));
  }
  t1.ToBuildTree();
  t2.ToBuildTree();
  uint32_t num_of_calls = 0;
  std::cin >> num_of_calls;
  size_t com;
  uint32_t lt = 0;
  uint32_t rt = 0;
  for (uint32_t i = 0; i < num_of_calls; ++i) {
    std::cin >> com;
    std::cin >> lt >> rt;
    if (com == 0) {
      t2.Set(lt - 1, rt * (((lt & 1) == 1) ? 1 : -1));
      t1.Set(lt - 1, rt * (((lt & 1) == 1) ? -1 : 1));
    } else {
      if ((lt & 1) == 1) {
        std::cout << t2.GetSum(lt - 1, rt) << '\n';
      } else {
        std::cout << t1.GetSum(lt - 1, rt) << '\n';
      }
    }
  }
}