#include <iostream>
#include <vector>

template<typename T, class Comparator>
class SegmentTree{
 public:
  SegmentTree(const std::vector<T>& vec, const T& neutral) : tree_(vec), neutral_(neutral) {
    tree_.resize(vec.size() * 2);
    std::copy(vec.begin(), vec.end(), tree_.begin() + vec.size());
    for (ssize_t i = vec.size() - 1; i >= 0; --i) {
      tree_[i] = Comparator(tree_[2 * i + 1], tree_[2 * i + 2]);
    }
  }

  /// возвращеяемый тип технически не важен
  T query(size_t left, size_t right, size_t index = 0) {

    if () {
      return T();
    }
  }

  void update(size_t left, size_t right, T value, size_t index = 0);


  SegmentTree(std::vector<T>& vec) : tree_(std::move(vec)) {
    tree_.resize(vec.size() * 2);
  }
 private:
  std::vector<T> tree_;
  T neutral_;
};


int main() {

}