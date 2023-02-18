#include <iostream>
#include <vector>

class ST {
 public:
  ST(const std::vector<std::pair<int, int>>& arr) {
    size_t sz = arr.size();
    ToBuildLogArray(sz);
    s_t_.push_back(arr);
    for (size_t level = 1, width = 2; width <= sz; ++level, width <<= 1) {
      s_t_.emplace_back();
      for (size_t i = 0; i + (1 << level) <= sz; ++i) {
        s_t_[level].push_back(
            std::min(s_t_[level - 1][i + width / 2], s_t_[level - 1][i]));
      }
    }
  }
  int ToGetMin(const size_t& lt, const size_t& rt) {
    size_t level = logs_of_2_[rt - lt + 1];
    size_t width = 1 << level;
    if (s_t_[level][lt].first > s_t_[level][rt - width + 1].first) {
      return s_t_[level][rt - width + 1].second;
    }
    return s_t_[level][lt].second;
  }
  int ToGetMinPro(const size_t& lt, const size_t& rt) {
    size_t level = logs_of_2_[rt - lt + 1];
    size_t width = 1 << level;
    return std::min(s_t_[level][rt - width + 1].first, s_t_[level][lt].first);
  }

 private:
  std::vector<size_t> logs_of_2_;
  std::vector<std::vector<std::pair<int, int>>> s_t_;
  void ToBuildLogArray(const size_t& sz) {
    logs_of_2_.resize(sz + 1);
    for (size_t i = 2; i <= sz; ++i) {
      logs_of_2_[i] = logs_of_2_[i / 2] + 1;
    }
  }
};

int main() {
  size_t size = 0;
  size_t num_of_calls = 0;
  std::cin >> size >> num_of_calls;
  std::vector<std::pair<int, int>> arr;
  arr.reserve(size);
  int tmp = 0;
  for (size_t i = 0; i < size; ++i) {
    std::cin >> tmp;
    arr.push_back({tmp, i + 1});
  }
  ST s_t(arr);
  size_t lt = 0;
  size_t rt = 0;
  for (size_t i = 0; i < num_of_calls; ++i) {
    std::cin >> lt >> rt;
    tmp = s_t.ToGetMin(lt - 1, rt - 1);
    if (tmp != (int)lt && tmp != (int)rt) {
      std::cout << std::min(s_t.ToGetMinPro(tmp, rt - 1),
                            s_t.ToGetMinPro(lt - 1, tmp - 2));
    } else {
      if (tmp == (int)rt) {
        std::cout << s_t.ToGetMinPro(lt - 1, tmp - 2);
      } else {
        std::cout << s_t.ToGetMinPro(tmp, rt - 1);
      }
    }
    std::cout << '\n';
  }
}
