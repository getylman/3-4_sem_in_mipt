#include <iostream>
#include <stack>
#include <vector>

struct Four {
  int64_t one = 0;
  int64_t two = 0;
  int64_t three = 0;
  int64_t four = 0;
};

class Sol {
 public:
  Sol(const std::vector<int32_t>& vec) : arr_(vec) {}
  void ToMakeSol() {
    const size_t kLen = arr_.size() - 1;
    four_.resize(kLen + 1);
    int64_t sup = 0;
    size_t id = 0;
    std::stack<int64_t> ans_id;
    size_t id1 = 0;
    size_t id2 = 0;
    while (id1++ < kLen) {
      four_[id1].one = four_[id1].two = 1;
      id2 = 0;
      while (id2++ < id1 - 1) {
        if (arr_[id1] < arr_[id2]) {
          four_[id1].two = (four_[id2].two > four_[id2].one + 1)
                               ? four_[id2].two
                               : four_[id2].one + 1;
          if (four_[id1].two == four_[id2].one + 1) {
            four_[id1].four = id2;
          }
        } else if (arr_[id1] > arr_[id2]) {
          four_[id1].one = (four_[id2].one > four_[id2].two + 1)
                               ? four_[id2].one
                               : four_[id2].two + 1;
          if (four_[id1].one == four_[id2].two + 1) {
            four_[id1].three = id2;
          }
        }
      }
      sup = (sup > four_[id1].one) ? sup : four_[id1].one;
      sup = (sup > four_[id1].two) ? sup : four_[id1].two;
      id = (sup == four_[id1].one || sup == four_[id1].two) ? id1 : id;
    }
    bool flag = (sup == four_[id].two);
    while (id > 0) {
      ans_id.push(id);
      id = (flag) ? four_[id].four : four_[id].three;
      flag = !flag;
    }
    std::cout << sup << '\n';
    while (!ans_id.empty()) {
      std::cout << arr_[ans_id.top()] << ' ';
      ans_id.pop();
    }
  }

 private:
  std::vector<int32_t> arr_;
  std::vector<Four> four_;
};

int main() {
  size_t size = 0;
  std::cin >> size;
  std::vector<int32_t> arr(size + 1);
  size_t id1 = 0;
  while (id1++ < size) {
    std::cin >> arr[id1];
  }
  Sol sol(arr);
  sol.ToMakeSol();
}