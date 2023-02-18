#pragma once

#include <cstddef>
#include <vector>

template <typename T = int>
class RingBuffer {
 public:
  RingBuffer() = delete;
  explicit RingBuffer(size_t capacity) { arr_.reserve(capacity_ = capacity); }
  size_t Size() const { return size_; }
  bool Empty() const { return size_ == 0; }
  bool TryPush(const T& elem) {
    if (size_ == capacity_) {
      return false;
    }
    arr_[tail_] = elem;
    ++tail_;
    tail_ %= capacity_;
    ++size_;
    return true;
  }
  bool TryPop(T* elem) {
    if (size_ == 0) {
      return false;
    }
    *elem = arr_[head_];
    ++head_;
    head_ %= capacity_;
    --size_;
    return true;
  }

 private:
  std::vector<T> arr_;
  size_t head_ = 0, tail_ = 0, capacity_ = 0, size_ = 0;
};
