// WA 1 test
#include <iostream>
#include <vector>

class MyHeap {
 public:
  MyHeap() {
    heap_min_.reserve(200000);
    heap_max_.reserve(200000);
    id_min_in_max_.reserve(200000);
    id_max_in_min_.reserve(200000);
  }
  size_t Size() { return heap_min_.size(); }
  void Clear() {
    heap_min_.clear();
    heap_max_.clear();
    id_min_in_max_.clear();
    id_max_in_min_.clear();
  }
  int GetMin() { return heap_min_[0].first; }
  int GetMax() { return heap_max_[0].first; }
  void Insert(int elem, int id) {
    heap_min_.push_back({elem, id});
    heap_max_.push_back({elem, id});
    id_max_in_min_[id] = SiftDownMin(heap_min_.size() - 1);
    id_min_in_max_[id] = SiftDownMax(heap_max_.size() - 1);
  }
  int ExtractMin() {
    int id_mx = id_min_in_max_[heap_min_[0].second];
    std::swap(id_max_in_min_[heap_min_[0].second],
              id_max_in_min_[heap_min_[heap_min_.size() - 1].second]);
    std::swap(heap_min_[0], heap_min_[heap_min_.size() - 1]);
    heap_min_.pop_back();
    int id = heap_min_[0].second;
    id_max_in_min_[id] = SiftDownMin(0);
    std::swap(id_min_in_max_[heap_max_[id_mx].second],
              id_min_in_max_[heap_max_[heap_max_.size() - 1].second]);
    std::swap(heap_max_[id_mx], heap_max_[heap_max_.size() - 1]);
    heap_max_.pop_back();
    id = heap_max_[id_mx].second;
    id_min_in_max_[id] =
        ((heap_max_[id_mx].first > heap_max_[(id_mx - 1) / 2].first)
             ? SiftUpMax(id_mx)
             : SiftDownMax(id_mx));
    return GetMin();
  }
  int ExtractMax() {
    int id_mn = id_max_in_min_[heap_max_[0].second];
    std::swap(id_min_in_max_[heap_max_[0].second],
              id_min_in_max_[heap_max_[heap_max_.size() - 1].second]);
    std::swap(heap_max_[0], heap_max_[heap_max_.size() - 1]);
    heap_max_.pop_back();
    int id = heap_max_[0].second;
    id_min_in_max_[id] = SiftDownMax(0);
    std::swap(id_max_in_min_[heap_min_[id_mn].second],
              id_max_in_min_[heap_min_[heap_min_.size() - 1].second]);
    std::swap(heap_min_[id_mn], heap_min_[heap_min_.size() - 1]);
    heap_min_.pop_back();
    id = heap_min_[id_mn].second;
    id_max_in_min_[id] =
        ((heap_min_[id_mn].first < heap_min_[(id_mn - 1) / 2].first)
             ? SiftUpMin(id_mn)
             : SiftDownMin(id_mn));
    return GetMax();
  }
  ~MyHeap() = default;

 private:
  std::vector<std::pair<int, int>> heap_min_, heap_max_;
  std::vector<int> id_min_in_max_, id_max_in_min_;
  size_t SiftUpMin(size_t id) {
    while (id > 0 && heap_min_[id].first < heap_min_[(id - 1) / 2].first) {
      id_max_in_min_[heap_min_[(id - 1) / 2].second] = id;
      std::swap(heap_min_[id], heap_min_[(id - 1) / 2]);
      id = (id - 1) / 2;
    }
    return id;
  }
  size_t SiftUpMax(size_t id) {
    while (id > 0 && heap_max_[id].first > heap_max_[(id - 1) / 2].first) {
      id_min_in_max_[heap_max_[(id - 1) / 2].second] = id;
      std::swap(heap_max_[id], heap_max_[(id - 1) / 2]);
      id = (id - 1) / 2;
    }
    return id;
  }
  size_t SiftDownMax(size_t id) {
    while ((2 * id + 1) < heap_max_.size()) {
      size_t kid_id = 2 * id + 1;
      if (kid_id + 1 < heap_max_.size() &&
          heap_max_[kid_id + 1].first > heap_max_[kid_id].first) {
        ++kid_id;
      }
      if (heap_max_[id].first >= heap_max_[kid_id].first) {
        break;
      }
      id_min_in_max_[heap_max_[kid_id].second] = id;
      std::swap(heap_max_[id], heap_max_[kid_id]);
      id = kid_id;
    }
    return id;
  }
  size_t SiftDownMin(size_t id) {
    while ((2 * id + 1) < heap_min_.size()) {
      size_t kid_id = 2 * id + 1;
      if (kid_id + 1 < heap_min_.size() &&
          heap_min_[kid_id + 1].first < heap_min_[kid_id].first) {
        ++kid_id;
      }
      if (heap_min_[id] <= heap_min_[kid_id]) {
        break;
      }
      id_max_in_min_[heap_min_[kid_id].second] = id;
      std::swap(heap_min_[id], heap_min_[kid_id]);
      id = kid_id;
    }
    return id;
  }
};

void PrintAns(MyHeap& my_heap, std::string com, int i) {
  if (com == "clear") {
    my_heap.Clear();
    std::puts("ok");
  } else if (com == "get_min") {
    std::cout << ((my_heap.Size() > 0) ? std::to_string(my_heap.GetMin())
                                       : "error")
              << std::endl;
  } else if (com == "get_max") {
    std::cout << ((my_heap.Size() > 0) ? std::to_string(my_heap.GetMax())
                                       : "error")
              << std::endl;
  } else if (com == "size") {
    std::cout << my_heap.Size() << std::endl;
  } else if (com == "insert") {
    int num;
    std::cin >> num;
    my_heap.Insert(num, i);
    std::puts("ok");
  } else if (com == "extract_max") {
    std::cout << ((my_heap.Size() > 0) ? std::to_string(my_heap.ExtractMax())
                                       : "error")
              << std::endl;
  } else if (com == "extract_min") {
    std::cout << ((my_heap.Size() > 0) ? std::to_string(my_heap.ExtractMin())
                                       : "error")
              << std::endl;
  }
}

int main() {
  int num_of_calls = 0;
  std::cin >> num_of_calls;
  std::string com;
  MyHeap my_heap;
  for (int i = 0; i < num_of_calls; i++) {
    std::cin >> com;
    PrintAns(my_heap, com, i);
  }
}