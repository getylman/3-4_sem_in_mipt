/**
 * @file deque.hpp
 * @author getylman
 * @date 22.02.2023
 */
#pragma once
#include <memory>


template <typename T, typename Allocator = std::allocator<T>>
class Deque {
 public:

 private:
  const uint64_t kChunkSize = 0; // size of chunks
  uint64_t total_size_ = 0; // size of in full container
  uint64_t fc_size_ = 0; // first chunk size
  uint64_t lc_size_ = 0; // last chunk size
};
//using Deque = std::deque<T>;
