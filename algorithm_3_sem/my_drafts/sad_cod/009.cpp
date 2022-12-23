#include <iostream>

class MyStack {
 public:
  MyStack() {
    size_ = 0;
    stck_ = new std::pair<int, int>[kCapacity_];
  }
  void Push(int elem) {
    if (size_ == 0) {
      stck_[size_] = {elem, elem};
    } else {
      stck_[size_] = {elem, std::min(elem, stck_[size_ - 1].second)};
    }
    ++size_;
  }
  uint32_t Size() { return size_; }
  void Clear() { size_ = 0; }
  int Min() { return stck_[size_ - 1].second; }
  int Top() { return stck_[size_ - 1].first; }
  int Pop() { return stck_[--size_].first; }
  ~MyStack() {
    delete [] stck_;
  }
 private:
  std::pair<int, int>* stck_;
  uint32_t size_;
  const uint32_t kCapacity_ = 200000;
};

class MyQueue {
 public:
  MyQueue() = default;
  void Enqueue(int elem) { stck1_.Push(elem); }
  uint32_t Size() { return stck1_.Size() + stck2_.Size(); }
  void Clear() {
    stck1_.Clear();
    stck2_.Clear();
  }
  uint32_t Dequeue() {
    if (stck2_.Size() == 0) {
      Shift_();
    }
    return stck2_.Pop();
  }
  uint32_t Front() {
    if (stck2_.Size() == 0) {
      Shift_();
    }
    return stck2_.Top();
  }
  bool Empty() { return (bool)(!Size()); }
  uint32_t Min() {
    if (stck1_.Size() != 0 && stck2_.Size() != 0) {
      return std::min(stck1_.Min(), stck2_.Min());
    }
    if (stck2_.Size() == 0) {
      Shift_();
    }
    return stck2_.Min();
  }
  ~MyQueue() {
    stck1_.~MyStack();
    stck2_.~MyStack();
  }
 private:
  MyStack stck1_, stck2_;
  void Shift_() {
    while (stck1_.Size() > 0) {
      stck2_.Push(stck1_.Pop());
    }
  }
};

void PrintingAnswer(std::string com, MyQueue& my_queue) {
  if (com == "dequeue") {
    if (my_queue.Empty()) {
      std::cout << "error";
    } else {
      std::cout << my_queue.Dequeue();
    }
  } else if (com == "front") {
    if (my_queue.Empty()) {
      std::cout << "error";
    } else {
      std::cout << my_queue.Front();
    }
  } else if (com == "size") {
    std::cout << my_queue.Size();
  } else if (com == "clear") {
    my_queue.Clear();
    std::cout << "ok";
  } else if (com == "min") {
    if (my_queue.Empty()) {
      std::cout << "error";
    } else {
      std::cout << my_queue.Min();
    }
  }
}

int main() {
  MyQueue my_queue;
  uint32_t num_of_calls = 0;
  std::cin >> num_of_calls;
  int num_for_push = 0;
  std::string com;
  for (uint32_t i = 0; i < num_of_calls; ++i) {
    std::cin >> com;
    if (com == "enqueue") {
      std::cin >> num_for_push;
      my_queue.Enqueue(num_for_push);
      std::cout << "ok";
    } else {
      PrintingAnswer(com, my_queue);
    }
    std::cout << std::endl;
  }
  my_queue.~MyQueue();
}
