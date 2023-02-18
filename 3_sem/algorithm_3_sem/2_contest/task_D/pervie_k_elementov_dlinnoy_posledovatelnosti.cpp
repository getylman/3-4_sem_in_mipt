#include <iostream>
#include <vector>
#include <stack>

class MyHeap {
 public:
  MyHeap() { heap_.reserve(1000); }
  size_t Size() { return heap_.size(); }
  void Clear() { heap_.clear(); }
  size_t GetHead() { return heap_[0]; }
  void Insert(size_t elem) {
    heap_.push_back(elem);
    SiftUpMax(heap_.size() - 1);
  }
  void ExtractHead() {
    std::swap(heap_[0], heap_[heap_.size() - 1]);
    heap_.pop_back();
    SiftDownMax(0);
  }
  ~MyHeap() = default;
 private:
  std::vector<size_t> heap_;
  void SiftUpMax(size_t id) {
    while (id > 0 && heap_[id] > heap_[(id - 1) / 2]) {
      std::swap(heap_[id], heap_[(id - 1) / 2]);
      id = (id - 1) / 2;
    }
  }
  void SiftDownMax(size_t id) {
    while ((2 * id + 1) < heap_.size()) {
      size_t kid_id = 2 * id + 1;
      if (kid_id + 1 < heap_.size() && heap_[kid_id + 1] > heap_[kid_id]) {
        ++kid_id;
      }
      if (heap_[id] >= heap_[kid_id]) {
        break;
      }
      std::swap(heap_[id], heap_[kid_id]);
      id = kid_id;
    }
  }
};

int main() {
  const size_t kMod2in30 = 1073741824;
  size_t size = 0;
  size_t num = 0;
  size_t arr0 = 0;
  size_t mult = 0;
  size_t summ = 0;
  size_t elem = 0;
  std::cin >> size >> num >> arr0 >> mult >> summ;
  MyHeap my_heap;
  for (size_t i = 0; i < size; ++i) {
    elem = mult * arr0 + summ;
    arr0 = elem & (kMod2in30 - 1);
    if (my_heap.Size() == num) {
      if (my_heap.GetHead() > arr0) {
        my_heap.ExtractHead();
        my_heap.Insert(arr0);
      }
    } else {
      my_heap.Insert(arr0);
    }
  }
  std::stack<size_t> stck;
  while ((bool)my_heap.Size()) {
    stck.push(my_heap.GetHead());
    my_heap.ExtractHead();
  }
  while (!stck.empty()) {
    std::cout << stck.top() << ' ';
    stck.pop();
  }
}