#include <iostream>
#include <vector>

class MyHeap {
 public:
  MyHeap() {
    heap_max_.reserve(200010);
    heap_min_.reserve(200010);
    mx_to_mn_.reserve(200010);
    mn_to_mx_.reserve(200010);
  }
  void Insert(int elem, int id) {
    heap_max_.push_back({elem, id});
    heap_min_.push_back({elem, id});
    mx_to_mn_[id] = SiftDownMin(size_min_ - 1);
    mn_to_mx_[id] = SiftDownMax(size_max_ - 1);
  }
  int Size() { return size_min_; }
  void Clear() {
    size_max_ = 0;
    size_min_ = 0;
  }
  int GetMin() { return heap_min_[0].first; }
  int GetMax() { return heap_max_[0].first; }
  int ExtractMax() {
    int ans = heap_max_[0].first;
    int id_mn = mn_to_mx_[heap_max_[0].second];
    std::swap(mn_to_mx_[heap_max_[0].second],
              mn_to_mx_[heap_max_[size_max_ - 1].second]);
    std::swap(heap_max_[0], heap_max_[size_max_ - 1]);
    size_max_--;
    int id = heap_max_[0].second;
    mn_to_mx_[id] = SiftDownMax(0);
    std::swap(mx_to_mn_[heap_min_[id_mn].second],
              mx_to_mn_[heap_min_[size_min_ - 1].second]);
    std::swap(heap_min_[id_mn], heap_min_[size_min_ - 1]);
    size_min_--;
    id = heap_min_[id_mn].second;
    if (heap_min_[id_mn].first < heap_min_[(id_mn - 1) / 2].first) {
      mx_to_mn_[id] = SiftUpMin(id_mn);
    } else {
      mx_to_mn_[id] = SiftDownMin(id_mn);
    }
    return ans;
  }
  int ExtractMin() {
    int ans = heap_min_[0].first;
    int id_mx = mn_to_mx_[heap_min_[0].second];
    std::swap(mx_to_mn_[heap_min_[0].second],
              mx_to_mn_[heap_min_[size_min_ - 1].second]);
    std::swap(heap_min_[0], heap_min_[size_min_ - 1]);
    size_min_--;
    int id = heap_min_[0].second;
    mx_to_mn_[id] = SiftDownMin(0);
    std::swap(mn_to_mx_[heap_max_[id_mx].second],
              mn_to_mx_[heap_max_[size_max_ - 1].second]);
    std::swap(heap_max_[id_mx], heap_max_[size_max_ - 1]);
    size_max_--;
    id = heap_max_[id_mx].second;
    if (heap_max_[id_mx].first > heap_max_[(id_mx - 1) / 2].first) {
      mn_to_mx_[id] = SiftUpMax(id_mx);
    } else {
      mn_to_mx_[id] = SiftDownMax(id_mx);
    }
    return ans;
  }
 private:
  std::vector<std::pair<int, int>> heap_min_, heap_max_;
  int size_min_ = 0;
  int size_max_ = 0;
  std::vector<int> mx_to_mn_, mn_to_mx_;
  int SiftUpMax(int id) {
    while (id > 0) {
      if (heap_max_[id].first > heap_max_[(id - 1) / 2].first) {
        mn_to_mx_[heap_max_[(id - 1) / 2].second] = id;
        std::swap(heap_max_[id], heap_max_[(id - 1) / 2]);
        id = (id - 1) / 2;
      } else {
        break;
      }
    }
    return id;
  }
  int SiftUpMin(int id) {
    while (id > 0) {
      if (heap_min_[id].first < heap_min_[(id - 1) / 2].first) {
        mx_to_mn_[heap_min_[(id - 1) / 2].second] = id;
        std::swap(heap_min_[id], heap_min_[(id - 1) / 2]);
        id = (id - 1) / 2;
      } else {
        break;
      }
    }
    return id;
  }
  int SiftDownMin(int id) {
  while (id < size_min_) {
    int kid_id1 = (2 * id + 1 < size_min_ ? 2 * id + 1 : id);
    int kid_id2 = (2 * id + 2 < size_min_ ? 2 * id + 2 : id);
    if (heap_min_[id].first >
        std::min(heap_min_[kid_id1].first, heap_min_[kid_id2].first)) {
      if (heap_min_[kid_id1].first <= heap_min_[kid_id2].first) {
        mx_to_mn_[heap_min_[kid_id1].second] = id;
        std::swap(heap_min_[id], heap_min_[kid_id1]);
        id = kid_id1;
      } else {
        mx_to_mn_[heap_min_[kid_id2].second] = id;
        std::swap(heap_min_[id], heap_min_[kid_id2]);
        id = kid_id2;
      }
    } else {
      break;
    }
  }
  return id;
  }
  int SiftDownMax(int id) {
  while (id < size_max_) {
    int kid_id1 = (2 * id + 1 < size_max_ ? 2 * id + 1 : id);
    int kid_id2 = (2 * id + 2 < size_max_ ? 2 * id + 2 : id);
    if (heap_max_[id].first <
        std::min(heap_max_[kid_id1].first, heap_max_[kid_id2].first)) {
      if (heap_max_[kid_id1].first >= heap_max_[kid_id2].first) {
        mn_to_mx_[heap_max_[kid_id1].second] = id;
        std::swap(heap_max_[id], heap_max_[kid_id1]);
        id = kid_id1;
      } else {
        mn_to_mx_[heap_max_[kid_id2].second] = id;
        std::swap(heap_max_[id], heap_max_[kid_id2]);
        id = kid_id2;
      }
    } else {
      break;
    }
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
    std::cout << "\t ~ " << std::endl;
  }
}