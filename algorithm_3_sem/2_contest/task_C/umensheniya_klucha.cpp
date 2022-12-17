// WA in 6 test
#include <iostream>
#include <vector>

class MyHeap {
 public:
  MyHeap() {
    heap_.reserve(2000000);
    ids_.reserve(2000000);
  }
  int GetHead() { return heap_[0].first; }
  void Insert(int elem, int id) {
    ids_[id] = heap_.size();
    heap_.push_back({elem, id});
    SiftUpMin(ids_[id]);
  }
  void ExtractHead() {
    Swap(heap_[0], heap_[heap_.size() - 1]);
    heap_.pop_back();
    SiftDownMin(0);
  }
  void DecreaseKey(int id, int delta) {
    heap_[ids_[id]].first -= delta;
    SiftUpMin(ids_[id]);
  }
  ~MyHeap() = default;

 private:
  std::vector<std::pair<int, int>> heap_;
  std::vector<int> ids_;
  void Swap(std::pair<int, int>& lt, std::pair<int, int>& rt) {
    std::swap(ids_[lt.second], ids_[rt.second]);
    std::swap(rt, lt);
  }
  void SiftUpMin(int id) {
    while (id > 0 && heap_[id].first < heap_[(id - 1) / 2].first) {
      Swap(heap_[id], heap_[(id - 1) / 2]);
      id = (id - 1) / 2;
    }
  }
  void SiftDownMin(int id) {
    while ((2 * id + 1) < (int)heap_.size()) {
      int kid_id = 2 * id + 1;
      if (kid_id + 1 < (int)heap_.size() &&
          heap_[kid_id + 1].first < heap_[kid_id].first) {
        ++kid_id;
      }
      if (heap_[id] <= heap_[kid_id]) {
        break;
      }
      Swap(heap_[id], heap_[kid_id]);
      id = kid_id;
    }
  }
};

int main() {
  MyHeap my_heap;
  int num_of_calls = 0;
  std::cin >> num_of_calls;
  std::string com;
  for (int i = 0; i < num_of_calls; i++) {
    std::cin >> com;
    if (com == "insert") {
      int num = 0;
      std::cin >> num;
      my_heap.Insert(num, i);
    } else if (com == "getMin") {
      std::cout << my_heap.GetHead() << std::endl;
    } else if (com == "extractMin") {
      my_heap.ExtractHead();
    } else if (com == "decreaseKey") {
      int id = 0, delta = 0;
      std::cin >> id >> delta;
      my_heap.DecreaseKey(id - 1, delta);
    }
  }
}