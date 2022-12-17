
#include <iostream>
#include <vector>

class MyHeap {
 public:
  MyHeap() { heap_.reserve(200000); }
  size_t Size() { return heap_.size(); }
  void Clear() { heap_.clear(); }
  int GetHead() { return heap_[0]; }
  void Insert(int elem) {
    heap_.push_back(elem);
    SiftUpMin(heap_.size() - 1);
  }
  void ExtractHead() {
    std::swap(heap_[0], heap_[heap_.size() - 1]);
    heap_.pop_back();
    SiftDownMin(0);
  }
  ~MyHeap() = default;

 private:
  std::vector<int> heap_;
  void SiftUpMin(size_t id) {
    while (id > 0 && heap_[id] < heap_[(id - 1) / 2]) {
      std::swap(heap_[id], heap_[(id - 1) / 2]);
      id = (id - 1) / 2;
    }
  }
  void SiftDownMin(size_t id) {
    size_t kid_id = 0;
    while ((2 * id + 1) < heap_.size()) {
      kid_id = 2 * id + 1;
      if (kid_id + 1 < heap_.size() && heap_[kid_id + 1] < heap_[kid_id]) {
        ++kid_id;
      }
      if (heap_[id] <= heap_[kid_id]) {
        break;
      }
      std::swap(heap_[id], heap_[kid_id]);
      id = kid_id;
    }
  }
};

int main() {
  size_t num_of_calls = 0;
  uint32_t size = 0;
  int num = 0;
  std::cin >> num_of_calls;
  MyHeap my_heap;
  for (uint32_t i = 0; i < num_of_calls; ++i) {
    std::cin >> size;
    for (uint32_t j = 0; j < size; ++j) {
      std::cin >> num;
      my_heap.Insert(num);
    }
  }
  while (my_heap.Size() != 0) {
    std::cout << my_heap.GetHead() << ' ';
    my_heap.ExtractHead();
  }
}