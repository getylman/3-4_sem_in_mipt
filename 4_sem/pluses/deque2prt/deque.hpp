/**
 * @file deque.hpp
 * @author getylman
 * @date 15.05.2023
 */

#pragma once
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

template <typename TempT, typename Alloc = std::allocator<TempT>>
class Deque {
  static constexpr uint64_t set_chunk_rank() noexcept;
  // function to set a rank for chunk
  //====================Chunk=====================
  class Chunk;
  using chunk_pointer = Chunk*;
  //==============================================

  //=============Allocator Usings=================
  using type_alloc_type =
      typename std::allocator_traits<Alloc>::template rebind_alloc<TempT>;
  using type_alloc_traits = std::allocator_traits<type_alloc_type>;
  using chunk_alloc_type =
      typename type_alloc_traits::template rebind_alloc<Chunk>;
  using chunk_alloc_traits = std::allocator_traits<chunk_alloc_type>;
  //==============================================
  //============Allocator functions===============
  chunk_alloc_type& get_chunk_allocator() noexcept { return chunk_alloc_; }
  const chunk_alloc_type& get_chunk_allocator() const noexcept;
  type_alloc_type& get_type_allocator() noexcept { return type_alloc_; }
  const type_alloc_type& get_type_allocator() const noexcept;
  //==============================================
 public:
  //=================Usings=======================
  using value_type = TempT;
  using allocator_type = Alloc;
  using pointer = TempT*;
  using const_pointer = const TempT*;
  using reference = TempT&;
  using const_reference = const TempT&;
  using difference_type = ptrdiff_t;
  //==============================================
  //===============Iterator=======================
  template <bool IsConst>
  class common_iterator;
  //**************Iterator usings*****************
  using iterator = common_iterator<false>;
  using const_iterator = common_iterator<true>;
  using reverse_iterator = std::reverse_iterator<common_iterator<false>>;
  using const_reverse_iterator = std::reverse_iterator<common_iterator<true>>;
  //**********************************************
  //==============================================
  //=============Allocator functions==============
  Alloc get_allocator() noexcept { return type_alloc_; }
  Alloc get_allocator() const noexcept { return type_alloc_; }
  //==============================================
  //=================Constructors=================
  explicit Deque(const Alloc& alloc = Alloc());
  explicit Deque(const size_t& count, const Alloc& alloc = Alloc());
  explicit Deque(const size_t& count, const TempT& value,
                 const Alloc& alloc = Alloc());
  Deque(const Deque<TempT, Alloc>& deq);
  Deque(Deque<TempT, Alloc>&& deq);
  Deque(std::initializer_list<TempT> init, const Alloc& alloc = Alloc());
  ~Deque() noexcept;
  Deque<TempT, Alloc>& operator=(const Deque<TempT, Alloc>& deq) &;
  // will called only for lvalue
  Deque<TempT, Alloc>& operator=(Deque<TempT, Alloc>&& deq) & noexcept;
  // will called only for rvalue
  //==============================================
  //=============Accessing an element=============
  TempT& operator[](const size_t& index) noexcept;
  const TempT& operator[](const size_t& index) const noexcept;
  TempT& at(const size_t& index);
  const TempT& at(const size_t& index) const;
  TempT& front() noexcept;
  const TempT& front() const noexcept;
  TempT& back() noexcept;
  const TempT& back() const noexcept;
  //==============================================
  //==============SizeInformation=================
  size_t size() const noexcept;
  bool empty() const noexcept;
  //==============================================
  //===========Modification methods===============
  void push_back(const TempT& elem);
  void push_back(TempT&& elem);
  void pop_back() noexcept;
  void push_front(const TempT& elem);
  void push_front(TempT&& elem);
  void pop_front() noexcept;
  template <typename... Args>
  void emplace_back(Args&&... args);
  template <typename... Args>
  void emplace_front(Args&&... args);
  template <typename... Args>
  void emplace(const_iterator pos, Args&&... args);
  void insert(const_iterator pos, const TempT& value);
  void insert(const_iterator pos, TempT&& value);
  void erase(const_iterator pos) noexcept;
  //==============================================
  //================Iterator methods==============
  iterator begin() noexcept;
  const_iterator begin() const noexcept;
  const_iterator cbegin() const noexcept;
  iterator end() noexcept;
  const_iterator end() const noexcept;
  const_iterator cend() const noexcept;
  reverse_iterator rbegin() noexcept;
  const_reverse_iterator rbegin() const noexcept;
  const_reverse_iterator crbegin() const noexcept;
  reverse_iterator rend() noexcept;
  const_reverse_iterator rend() const noexcept;
  const_reverse_iterator crend() const noexcept;
  //==============================================

 private:
  //================Deque Fields==================
  const uint64_t kDefaultDequeLenth = 4;
  // --------------------------------
  // reason why default 4
  // # - using space, * - free space
  // ******
  // ****##
  // ##****
  // ******
  // --------------------------------
  uint64_t num_of_chunks_ = 0;
  size_t id_start_ = 0;
  // here an object exists
  size_t id_finish_ = 0;
  // here an object does not exist
  size_t size_ = 0;
  // total num of objects into container
  allocator_type type_alloc_;
  chunk_alloc_type chunk_alloc_;
  chunk_pointer vec_of_chunks_ = nullptr;
  //==============================================
  //==============Private Methods=================
  pointer pr_chunk_allocate() {
    return type_alloc_traits::allocate(get_type_allocator(), set_chunk_rank());
  }
  void pr_chunk_deallocate(pointer ptr) noexcept;
  // function to deallocate chunk vector
  chunk_pointer pr_vec_allocate(const size_t& len);
  // function to allocate memory for needable chunks
  void pr_move_construct_chunk_to_chunk(chunk_pointer src, chunk_pointer dst);
  // move values from source chunk to destination chunk
  void pr_copy_construct_chunk_to_chunk(chunk_pointer src, chunk_pointer dst);
  // copy values from source chunk to destination chunk
  void pr_chunk_destroy(chunk_pointer ptr) noexcept;
  // destroy all constructed objects into chunk
  void pr_vec_deallocate(chunk_pointer ptr, const size_t& len) noexcept;
  // deallocate all allocated memory
  void pr_clear_mem() noexcept;
  // clear all memory
  void pr_vec_reallocation(const size_t& new_len, const size_t& delta = 0);
  // reallocation memory to another place and increase capacity
  void pr_range_fill_init(const size_t& len, const TempT& value);
  // function to fill container with one value
  void pr_range_default_init(const size_t& len);
  // function to fill container with one value
  void pr_range_poper(const size_t& len) noexcept;
  // function to eraze last len objects
  template <typename Iter>
  void pr_range_copy_init(Iter start, Iter finish);
  // to init deque by to iterators
  void pr_clear() noexcept;
  // destroy all objects
  //==============================================
};

//====================Chunk=======================
template <typename TempT, typename Alloc>
class Deque<TempT, Alloc>::Chunk {
  // there will be no constructor or destructor
  // since everything will be performed one level higher
 public:
  uint64_t chunk_size() const noexcept;
  // number of using units in current chunk
  bool chunk_is_full() const noexcept;
  // is full current chunk
  bool chunk_is_empty() const noexcept { return chunk_size() == 0; }
  // is empty current chunk
  TempT& operator[](const size_t& index) noexcept;
  // to take unit which is in 'index' position from chunk head
  TempT& operator[](const size_t& index) const noexcept;
  // to take unit which is in 'index' position from chunk head
  pointer chunk_get_body_ptr() noexcept { return chunk_body_; }
  pointer chunk_get_body_ptr() const noexcept { return chunk_body_; }
  // to get pointer of chunk body
  pointer chunk_get_back_ptr() noexcept;
  pointer chunk_get_back_ptr() const noexcept;
  // to get pointer of last unit in chunk
  pointer chunk_get_front_ptr() noexcept;
  pointer chunk_get_front_ptr() const noexcept;
  // to get pointer of first unit in chunk
  friend Deque;

 private:
  //****************Fields***********************
  pointer chunk_body_ = nullptr;    // pointer of chunk body
  const uint64_t kChunkRank = 512;  // chunk max capacity
  uint64_t chunk_head_id_ = 0;      // head index
  uint64_t chunk_tail_id_ = 0;      // tail index
  type_alloc_type type_alloc_;      // allocator of type

  //*********************************************
};
//*******Chunk's functions implementation********
template <typename TempT, typename Alloc>
uint64_t Deque<TempT, Alloc>::Chunk::chunk_size() const noexcept {
  return chunk_tail_id_ - chunk_head_id_;
}
template <typename TempT, typename Alloc>
bool Deque<TempT, Alloc>::Chunk::chunk_is_full() const noexcept {
  return chunk_tail_id_ - chunk_head_id_ == set_chunk_rank();
}
template <typename TempT, typename Alloc>
TempT& Deque<TempT, Alloc>::Chunk::operator[](const size_t& index) noexcept {
  return *(chunk_body_ + chunk_head_id_ + index);
}
template <typename TempT, typename Alloc>
TempT& Deque<TempT, Alloc>::Chunk::operator[](
    const size_t& index) const noexcept {
  return *(chunk_body_ + chunk_head_id_ + index);
}
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::pointer
Deque<TempT, Alloc>::Chunk::chunk_get_back_ptr() noexcept {
  return chunk_body_ + chunk_tail_id_ - 1;
}
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::pointer
Deque<TempT, Alloc>::Chunk::chunk_get_back_ptr() const noexcept {
  return chunk_body_ + chunk_tail_id_ - 1;
}
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::pointer
Deque<TempT, Alloc>::Chunk::chunk_get_front_ptr() noexcept {
  return chunk_body_ + chunk_head_id_ - 1;
}
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::pointer
Deque<TempT, Alloc>::Chunk::chunk_get_front_ptr() const noexcept {
  return chunk_body_ + chunk_head_id_ - 1;
}
//***********************************************
//===============================================

//====================Iterator===================
template <typename TempT, typename Alloc>
template <bool IsConst>
class Deque<TempT, Alloc>::common_iterator
    : public std::iterator<std::random_access_iterator_tag,
                           std::conditional_t<IsConst, const TempT, TempT>> {
  //**************Private usings****************
  using conditional_ptr = std::conditional_t<IsConst, const TempT*, TempT*>;
  using conditional_ref = std::conditional_t<IsConst, const TempT&, TempT&>;
  //********************************************
  //*****************Fields**********************
  chunk_pointer ptr_chunk_ = nullptr;
  uint64_t idx_in_chunk_ = 0;
  //*********************************************
 public:
  //********************Getters******************
  uint64_t get_id_in_current_chunk() const noexcept { return idx_in_chunk_; }
  chunk_pointer get_ptr_of_current_chunk() const noexcept { return ptr_chunk_; }
  chunk_pointer get_ptr_of_current_chunk() noexcept { return ptr_chunk_; }
  //*********************************************
  //******************Constructors***************
  common_iterator() noexcept : ptr_chunk_(), idx_in_chunk_(0) {}
  explicit common_iterator(const Chunk* other_chunk_ptr,
                           const uint64_t& other_id) noexcept;
  template <bool IsConstTmp>
  common_iterator(const common_iterator<IsConstTmp>& other) noexcept;
  template <bool IsConstTmp>
  common_iterator<IsConst>& operator=(
      const common_iterator<IsConstTmp>& other) noexcept;
  //*********************************************
  //************Memory Access Operators**********
  conditional_ptr operator->() const noexcept;
  conditional_ptr operator->() noexcept;
  conditional_ref operator*() const noexcept;
  conditional_ref operator*() noexcept;
  //*********************************************
  //*************Arithmetic operators************
  common_iterator<IsConst>& operator+=(const difference_type& delta) noexcept;
  common_iterator<IsConst>& operator-=(const difference_type& delta) noexcept;
  common_iterator<IsConst> operator+(
      const difference_type& delta) const noexcept;
  common_iterator<IsConst> operator-(
      const difference_type& delta) const noexcept;
  common_iterator<IsConst>& operator++() noexcept { return *this += 1; }
  common_iterator<IsConst>& operator--() noexcept { return *this -= 1; }
  common_iterator<IsConst> operator++(int) noexcept;
  common_iterator<IsConst> operator--(int) noexcept;
  template <bool IsConstTmp>
  difference_type operator-(const common_iterator<IsConstTmp>& iter) noexcept {
    return (*this == iter)
               ? 0
               : static_cast<difference_type>(set_chunk_rank()) *
                         static_cast<difference_type>(
                             ptr_chunk_ - iter.get_ptr_of_current_chunk() -
                             static_cast<difference_type>(ptr_chunk_ !=
                                                          nullptr)) +
                     (idx_in_chunk_) +
                     (set_chunk_rank() - iter.get_id_in_current_chunk());
  }
  //*********************************************
  //************Compare operators****************
  template <bool IsConstTmp>
  friend bool operator<(const common_iterator<IsConst>& it1,
                        const common_iterator<IsConstTmp>& it2) noexcept {
    return (it1.ptr_chunk_ == it2.ptr_chunk_)
               ? it1.idx_in_chunk_ < it2.idx_in_chunk_
               : it1.ptr_chunk_ < it2.ptr_chunk_;
  }
  template <bool IsConstTmp>
  friend bool operator>(const common_iterator<IsConst>& it1,
                        const common_iterator<IsConstTmp>& it2) noexcept {
    return (it1.ptr_chunk_ == it2.ptr_chunk_)
               ? it1.idx_in_chunk_ > it2.idx_in_chunk_
               : it1.ptr_chunk_ > it2.ptr_chunk_;
  }
  template <bool IsConstTmp>
  friend bool operator==(const common_iterator<IsConst>& it1,
                         const common_iterator<IsConstTmp>& it2) noexcept {
    return (it1.get_ptr_of_current_chunk() == it2.get_ptr_of_current_chunk()) &&
           (it1.get_id_in_current_chunk() == it2.get_id_in_current_chunk());
  }
  template <bool IsConstTmp>
  friend bool operator!=(const common_iterator<IsConst>& it1,
                         const common_iterator<IsConstTmp>& it2) noexcept {
    return !(it1 == it2);
  }
  template <bool IsConstTmp>
  friend bool operator<=(const common_iterator<IsConst>& it1,
                         const common_iterator<IsConstTmp>& it2) noexcept {
    return !(it1 > it2);
  }
  template <bool IsConstTmp>
  friend bool operator>=(const common_iterator<IsConst>& it1,
                         const common_iterator<IsConstTmp>& it2) noexcept {
    return !(it1 < it2);
  }
  //*********************************************
};

//***************Constructors********************
template <typename TempT, typename Alloc>
template <bool IsConst>
Deque<TempT, Alloc>::common_iterator<IsConst>::common_iterator(
    const Chunk* other_chunk_ptr, const uint64_t& other_id) noexcept
    : ptr_chunk_(const_cast<Chunk*>(other_chunk_ptr)),
      idx_in_chunk_(other_id) {}

template <typename TempT, typename Alloc>
template <bool IsConst>
template <bool IsConstTmp>
Deque<TempT, Alloc>::common_iterator<IsConst>::common_iterator(
    const common_iterator<IsConstTmp>& other) noexcept
    : ptr_chunk_(other.get_ptr_of_current_chunk()),
      idx_in_chunk_(other.get_id_in_current_chunk()) {}

template <typename TempT, typename Alloc>
template <bool IsConst>
template <bool IsConstTmp>
typename Deque<TempT, Alloc>::template common_iterator<IsConst>&
Deque<TempT, Alloc>::common_iterator<IsConst>::operator=(
    const common_iterator<IsConstTmp>& other) noexcept {
  ptr_chunk_ = other.get_ptr_of_current_chunk();
  idx_in_chunk_ = other.get_id_in_current_chunk();
  return *this;
}
//***********************************************
//*************Memory Access Operators***********
template <typename TempT, typename Alloc>
template <bool IsConst>
typename Deque<TempT, Alloc>::template common_iterator<IsConst>::conditional_ptr
Deque<TempT, Alloc>::common_iterator<IsConst>::operator->() const noexcept {
  return (ptr_chunk_->chunk_body_ + idx_in_chunk_);
}
template <typename TempT, typename Alloc>
template <bool IsConst>
typename Deque<TempT, Alloc>::template common_iterator<IsConst>::conditional_ptr
Deque<TempT, Alloc>::common_iterator<IsConst>::operator->() noexcept {
  return (ptr_chunk_->chunk_body_ + idx_in_chunk_);
}
template <typename TempT, typename Alloc>
template <bool IsConst>
typename Deque<TempT, Alloc>::template common_iterator<IsConst>::conditional_ref
Deque<TempT, Alloc>::common_iterator<IsConst>::operator*() const noexcept {
  return ptr_chunk_->operator[](idx_in_chunk_ - ptr_chunk_->chunk_head_id_);
}
template <typename TempT, typename Alloc>
template <bool IsConst>
typename Deque<TempT, Alloc>::template common_iterator<IsConst>::conditional_ref
Deque<TempT, Alloc>::common_iterator<IsConst>::operator*() noexcept {
  return ptr_chunk_->operator[](idx_in_chunk_ - ptr_chunk_->chunk_head_id_);
}
//***********************************************
//*************Aithmetic operators***************
template <typename TempT, typename Alloc>
template <bool IsConst>
typename Deque<TempT, Alloc>::template common_iterator<IsConst>&
Deque<TempT, Alloc>::common_iterator<IsConst>::operator+=(
    const difference_type& delta) noexcept {
  const difference_type kDist =
      static_cast<difference_type>(idx_in_chunk_ + delta);
  if (kDist >= 0 and kDist < static_cast<difference_type>(set_chunk_rank())) {
    idx_in_chunk_ += delta;
  } else {
    const difference_type kChunkDist =
        (kDist > 0)
            ? kDist / static_cast<difference_type>(set_chunk_rank())
            : -((-kDist - 1) / static_cast<difference_type>(set_chunk_rank())) -
                  1;
    ptr_chunk_ += kChunkDist;
    idx_in_chunk_ = (kDist - kChunkDist * set_chunk_rank());
  }
  return *this;
}
template <typename TempT, typename Alloc>
template <bool IsConst>
typename Deque<TempT, Alloc>::template common_iterator<IsConst>&
Deque<TempT, Alloc>::common_iterator<IsConst>::operator-=(
    const difference_type& delta) noexcept {
  return *this += -delta;
}
template <typename TempT, typename Alloc>
template <bool IsConst>
typename Deque<TempT, Alloc>::template common_iterator<IsConst>
Deque<TempT, Alloc>::common_iterator<IsConst>::operator+(
    const difference_type& delta) const noexcept {
  common_iterator<IsConst> tmp = *this;
  tmp += delta;
  return tmp;
}
template <typename TempT, typename Alloc>
template <bool IsConst>
typename Deque<TempT, Alloc>::template common_iterator<IsConst>
Deque<TempT, Alloc>::common_iterator<IsConst>::operator-(
    const difference_type& delta) const noexcept {
  common_iterator<IsConst> tmp = *this;
  tmp -= delta;
  return tmp;
}
template <typename TempT, typename Alloc>
template <bool IsConst>
typename Deque<TempT, Alloc>::template common_iterator<IsConst>
Deque<TempT, Alloc>::common_iterator<IsConst>::operator++(int) noexcept {
  common_iterator<IsConst> tmp(*this);
  *this += 1;
  return tmp;
}
template <typename TempT, typename Alloc>
template <bool IsConst>
typename Deque<TempT, Alloc>::template common_iterator<IsConst>
Deque<TempT, Alloc>::common_iterator<IsConst>::operator--(int) noexcept {
  common_iterator<IsConst> tmp(*this);
  *this -= 1;
  return tmp;
}
template <typename TempT, typename Alloc = std::allocator<TempT>, bool IsConst>
typename Deque<TempT, Alloc>::template common_iterator<IsConst> operator+(
    const typename Deque<TempT, Alloc>::template common_iterator<
        IsConst>::difference_type& delta,
    const typename Deque<TempT, Alloc>::template common_iterator<IsConst>&
        iter) noexcept {
  return iter + delta;
}
template <typename TempT, typename Alloc = std::allocator<TempT>, bool IsConst>
typename Deque<TempT, Alloc>::template common_iterator<IsConst> operator-(
    const typename Deque<TempT, Alloc>::template common_iterator<
        IsConst>::difference_type& delta,
    const typename Deque<TempT, Alloc>::template common_iterator<IsConst>&
        iter) noexcept {
  return iter - delta;
}
//***********************************************
//===============================================
//=====================Deque=====================
//***************Set Chunk Rank******************
template <typename TempT, typename Alloc>
constexpr uint64_t Deque<TempT, Alloc>::set_chunk_rank() noexcept {
  const uint64_t kChunkRank = 512;
  return kChunkRank;
}
//***********************************************
//***************Allocator Fuctions**************
template <typename TempT, typename Alloc>
const typename Deque<TempT, Alloc>::chunk_alloc_type&
Deque<TempT, Alloc>::get_chunk_allocator() const noexcept {
  return chunk_alloc_;
}
template <typename TempT, typename Alloc>
const typename Deque<TempT, Alloc>::type_alloc_type&
Deque<TempT, Alloc>::get_type_allocator() const noexcept {
  return type_alloc_;
}
//***********************************************
//******************Constructors*****************
template <typename TempT, typename Alloc>
Deque<TempT, Alloc>::Deque(const Alloc& alloc)
    : type_alloc_(alloc),
      chunk_alloc_(chunk_alloc_type(alloc)),
      vec_of_chunks_() {}
template <typename TempT, typename Alloc>
Deque<TempT, Alloc>::~Deque() noexcept {
  if (vec_of_chunks_ != nullptr) {
    pr_clear();
    pr_vec_deallocate(vec_of_chunks_, num_of_chunks_);
  }
}
template <typename TempT, typename Alloc>
Deque<TempT, Alloc>::Deque(const size_t& count, const Alloc& alloc)
    : type_alloc_(alloc), chunk_alloc_(chunk_alloc_type(alloc)) {
  const size_t kSize = count / set_chunk_rank() + 1;
  const size_t kFullSize = std::max(kSize + 2, kDefaultDequeLenth);
  id_start_ = kFullSize / 2 - 1;
  id_finish_ = kFullSize / 2 + ((count == 1) ? 0 : 1);
  vec_of_chunks_ = pr_vec_allocate(kFullSize);
  num_of_chunks_ = kFullSize;
  try {
    pr_range_default_init(count);
  } catch (...) {
    pr_vec_deallocate(vec_of_chunks_, kFullSize);
    throw;
  }
}
template <typename TempT, typename Alloc>
Deque<TempT, Alloc>::Deque(const size_t& count, const TempT& value,
                           const Alloc& alloc)
    : type_alloc_(alloc), chunk_alloc_(chunk_alloc_type(alloc)) {
  const size_t kSize = count / set_chunk_rank() + 1;
  const size_t kFullSize = std::max(kSize + 2, kDefaultDequeLenth);
  id_start_ = kFullSize / 2 - 1;
  id_finish_ = kFullSize / 2 + ((count == 1) ? 0 : 1);
  vec_of_chunks_ = pr_vec_allocate(kFullSize);
  num_of_chunks_ = kFullSize;
  try {
    pr_range_fill_init(count, value);
  } catch (...) {
    pr_vec_deallocate(vec_of_chunks_, kFullSize);
    throw;
  }
}
template <typename TempT, typename Alloc>
Deque<TempT, Alloc>::Deque(std::initializer_list<TempT> init,
                           const Alloc& alloc)
    : type_alloc_(alloc), chunk_alloc_(chunk_alloc_type(alloc)) {
  const size_t kLen = init.size();
  const size_t kSize = kLen / set_chunk_rank() + 1;
  const size_t kFullSize = std::max(kSize + 2, kDefaultDequeLenth);
  id_start_ = 0;
  id_finish_ = 1;
  num_of_chunks_ = kFullSize;
  vec_of_chunks_ = pr_vec_allocate(kFullSize);
  try {
    pr_range_copy_init(init.begin(), init.end());
  } catch (...) {
    pr_vec_deallocate(vec_of_chunks_, kFullSize);
    throw;
  }
  ++id_start_;
}
template <typename TempT, typename Alloc>
Deque<TempT, Alloc>::Deque(const Deque<TempT, Alloc>& deq)
    : type_alloc_(type_alloc_traits::select_on_container_copy_construction(
          deq.get_type_allocator())),
      chunk_alloc_(chunk_alloc_traits::select_on_container_copy_construction(
          deq.get_chunk_allocator())) {
  if (deq.vec_of_chunks_ != nullptr) {
    const size_t kLen = deq.size();
    const size_t kSize = kLen / set_chunk_rank() + 1;
    const size_t kFullSize = std::max(kSize + 2, kDefaultDequeLenth);
    id_start_ = 0;
    id_finish_ = 1;
    num_of_chunks_ = kFullSize;
    vec_of_chunks_ = pr_vec_allocate(kFullSize);
    try {
      pr_range_copy_init(deq.begin(), deq.end());
    } catch (...) {
      pr_vec_deallocate(vec_of_chunks_, kFullSize);
      throw;
    }
    ++id_start_;
  }
}
template <typename TempT, typename Alloc>
Deque<TempT, Alloc>::Deque(Deque<TempT, Alloc>&& deq)
    : num_of_chunks_(deq.num_of_chunks_),
      id_start_(deq.id_start_),
      id_finish_(deq.id_finish_),
      size_(deq.size_),
      type_alloc_(deq.type_alloc_),
      chunk_alloc_(deq.chunk_alloc_),
      vec_of_chunks_(deq.vec_of_chunks_) {
  deq.id_finish_ = kDefaultDequeLenth / 2;
  deq.id_start_ = kDefaultDequeLenth / 2 - 1;
  deq.num_of_chunks_ = kDefaultDequeLenth;
  deq.size_ = 0;
  try {
    deq.vec_of_chunks_ = pr_vec_allocate(kDefaultDequeLenth);
    ++deq.id_start_;
  } catch (...) {
    deq.id_start_ = id_start_;
    deq.id_finish_ = id_finish_;
    deq.size_ = size_;
    deq.vec_of_chunks_ = vec_of_chunks_;
    deq.num_of_chunks_ = num_of_chunks_;
    throw;
  }
}
template <typename TempT, typename Alloc>
Deque<TempT, Alloc>& Deque<TempT, Alloc>::operator=(
    const Deque<TempT, Alloc>& deq) & {
  if (type_alloc_traits::propagate_on_container_copy_assignment::value) {
    type_alloc_ = deq.type_alloc_;
    chunk_alloc_ = deq.chunk_alloc_;
  }
  if (deq.empty()) {
    pr_clear();
    return *this;
  }
  const_iterator iter2 = deq.begin();
  if (!empty()) {
    size_t min_of_two = std::min(size(), deq.size());
    iterator iter1 = begin();
    for (size_t i = 0; i < min_of_two; ++i, ++iter1, ++iter2) {
      *iter1 = *iter2;
    }
    while (size() > deq.size()) {
      pop_back();
    }
  }
  try {
    for (; size() < deq.size(); ++iter2) {
      push_back(*iter2);
    }
  } catch (...) {
    pr_clear();
    throw;
  }
  return *this;
}
template <typename TempT, typename Alloc>
Deque<TempT, Alloc>& Deque<TempT, Alloc>::operator=(
    Deque<TempT, Alloc>&& deq) & noexcept {
  if (type_alloc_traits::propagate_on_container_move_assignment::value) {
    type_alloc_ = deq.type_alloc_;
    chunk_alloc_ = deq.chunk_alloc_;
  }
  if (vec_of_chunks_ != nullptr) {
    pr_clear();
    pr_vec_deallocate(vec_of_chunks_, num_of_chunks_);
  }
  vec_of_chunks_ = deq.vec_of_chunks_;
  id_finish_ = deq.id_finish_;
  id_start_ = deq.id_start_;
  num_of_chunks_ = deq.num_of_chunks_;
  size_ = deq.size_;
  deq.id_finish_ = deq.id_start_ = deq.num_of_chunks_ = deq.size_ = 0;
  deq.vec_of_chunks_ = nullptr;
  return *this;
}
//***********************************************
//***************Accessing an element************
template <typename TempT, typename Alloc>
TempT& Deque<TempT, Alloc>::operator[](const size_t& index) noexcept {
  return *(begin() + index);
}
template <typename TempT, typename Alloc>
const TempT& Deque<TempT, Alloc>::operator[](
    const size_t& index) const noexcept {
  return *(begin() + index);
}
template <typename TempT, typename Alloc>
TempT& Deque<TempT, Alloc>::at(const size_t& index) {
  if (index < size()) {
    return *(begin() + index);
  }
  throw std::out_of_range("you went out of allowable area\n");
}
template <typename TempT, typename Alloc>
const TempT& Deque<TempT, Alloc>::at(const size_t& index) const {
  if (index < size()) {
    return *(begin() + index);
  }
  throw std::out_of_range("you went out of allowable area\n");
}
template <typename TempT, typename Alloc>
TempT& Deque<TempT, Alloc>::front() noexcept {
  return operator[](0);
}
template <typename TempT, typename Alloc>
const TempT& Deque<TempT, Alloc>::front() const noexcept {
  return operator[](0);
}
template <typename TempT, typename Alloc>
TempT& Deque<TempT, Alloc>::back() noexcept {
  return operator[](size() - 1);
}
template <typename TempT, typename Alloc>
const TempT& Deque<TempT, Alloc>::back() const noexcept {
  return operator[](size() - 1);
}
//***********************************************
//*****************SizeInformation***************
template <typename TempT, typename Alloc>
size_t Deque<TempT, Alloc>::size() const noexcept {
  return size_;
}
template <typename TempT, typename Alloc>
bool Deque<TempT, Alloc>::empty() const noexcept {
  return size() == 0;
}
//***********************************************
//**************Modification methods*************
template <typename TempT, typename Alloc>
template <typename... Args>
void Deque<TempT, Alloc>::emplace_back(Args&&... args) {
  if (vec_of_chunks_ == nullptr) {
    id_finish_ = kDefaultDequeLenth / 2;
    id_start_ = kDefaultDequeLenth / 2 - 2;
    vec_of_chunks_ = pr_vec_allocate(kDefaultDequeLenth);
    ++id_start_;
    num_of_chunks_ = kDefaultDequeLenth;
  }
  if ((vec_of_chunks_ + id_finish_ - 1)->chunk_tail_id_ == set_chunk_rank()) {
    if (id_finish_ == num_of_chunks_) {
      pr_vec_reallocation(num_of_chunks_ * 2);
    }
    ++id_finish_;
  }
  try {
    type_alloc_traits::construct(
        get_type_allocator(),
        (vec_of_chunks_ + id_finish_ - 1)->chunk_body_ +
            (vec_of_chunks_ + id_finish_ - 1)->chunk_tail_id_,
        std::forward<Args>(args)...);
  } catch (...) {
    if ((vec_of_chunks_ + id_finish_ - 1)->chunk_is_empty()) {
      --id_finish_;
    }
    throw;
  }
  ++(vec_of_chunks_ + id_finish_ - 1)->chunk_tail_id_;
  ++size_;
}
template <typename TempT, typename Alloc>
template <typename... Args>
void Deque<TempT, Alloc>::emplace_front(Args&&... args) {
  if (vec_of_chunks_ == nullptr) {
    id_finish_ = kDefaultDequeLenth / 2;
    id_start_ = kDefaultDequeLenth / 2 - 1;
    vec_of_chunks_ = pr_vec_allocate(kDefaultDequeLenth);
    num_of_chunks_ = kDefaultDequeLenth;
  }
  if ((vec_of_chunks_ + id_start_)->chunk_head_id_ == 0) {
    if (id_start_ == 0) {
      pr_vec_reallocation(num_of_chunks_ * 2, num_of_chunks_);
    }
    --id_start_;
  }
  try {
    type_alloc_traits::construct(
        get_type_allocator(),
        (vec_of_chunks_ + id_start_)->chunk_body_ +
            (vec_of_chunks_ + id_start_)->chunk_head_id_ - 1,
        std::forward<Args>(args)...);
  } catch (...) {
    if ((vec_of_chunks_ + id_start_)->chunk_is_empty()) {
      ++id_start_;
    }
    throw;
  }
  --(vec_of_chunks_ + id_start_)->chunk_head_id_;
  ++size_;
}
template <typename TempT, typename Alloc>
template <typename... Args>
void Deque<TempT, Alloc>::emplace(const_iterator pos, Args&&... args) {
  const size_t kDelta = size() - (pos - begin());
  emplace_back(std::forward<Args>(args)...);
  iterator iter = end() - 1;
  for (size_t i = 0; i < kDelta; ++i, --iter) {
    std::swap(*iter, *(iter - 1));
  }
}
template <typename TempT, typename Alloc>
void Deque<TempT, Alloc>::push_back(TempT&& elem) {
  emplace_back(std::move(elem));
}
template <typename TempT, typename Alloc>
void Deque<TempT, Alloc>::push_front(TempT&& elem) {
  emplace_front(std::move(elem));
}
template <typename TempT, typename Alloc>
void Deque<TempT, Alloc>::push_back(const TempT& elem) {
  emplace_back(elem);
}
template <typename TempT, typename Alloc>
void Deque<TempT, Alloc>::push_front(const TempT& elem) {
  emplace_front(elem);
}
template <typename TempT, typename Alloc>
void Deque<TempT, Alloc>::insert(const_iterator pos, const TempT& value) {
  emplace(pos, value);
}
template <typename TempT, typename Alloc>
void Deque<TempT, Alloc>::insert(const_iterator pos, TempT&& value) {
  emplace(pos, std::move(value));
}
template <typename TempT, typename Alloc>
void Deque<TempT, Alloc>::pop_back() noexcept {
  if ((vec_of_chunks_ + id_finish_ - 1)->chunk_is_empty()) {
    --id_finish_;
  }
  type_alloc_traits::destroy(
      get_type_allocator(),
      (vec_of_chunks_ + id_finish_ - 1)->chunk_get_back_ptr());
  --(vec_of_chunks_ + id_finish_ - 1)->chunk_tail_id_;
  if ((vec_of_chunks_ + id_finish_ - 1)->chunk_is_empty()) {
    --id_finish_;
  }
  --size_;
}
template <typename TempT, typename Alloc>
void Deque<TempT, Alloc>::pop_front() noexcept {
  if ((vec_of_chunks_ + id_start_)->chunk_is_empty()) {
    ++id_start_;
  }
  type_alloc_traits::destroy(
      get_type_allocator(),
      (vec_of_chunks_ + id_start_)->chunk_get_front_ptr());
  ++(vec_of_chunks_ + id_start_)->chunk_head_id_;
  if ((vec_of_chunks_ + id_start_)->chunk_is_empty()) {
    ++id_start_;
  }
  --size_;
}
template <typename TempT, typename Alloc>
void Deque<TempT, Alloc>::erase(const_iterator pos) noexcept {
  const size_t kDelta = size() - (pos - begin()) - 1;
  iterator iter = begin() + (pos - begin());
  for (size_t i = 0; i < kDelta; ++i) {
    std::swap(*iter, *(iter + 1));
  }
  pop_back();
}
//***********************************************
//***************Iterator methods****************
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::iterator Deque<TempT, Alloc>::begin() noexcept {
  return (vec_of_chunks_ == nullptr)
             ? iterator()
             : iterator(vec_of_chunks_ + id_start_,
                        (vec_of_chunks_ + id_start_)->chunk_head_id_);
}
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::const_iterator Deque<TempT, Alloc>::begin()
    const noexcept {
  return (vec_of_chunks_ == nullptr)
             ? const_iterator()
             : const_iterator(vec_of_chunks_ + id_start_,
                              (vec_of_chunks_ + id_start_)->chunk_head_id_);
}
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::const_iterator Deque<TempT, Alloc>::cbegin()
    const noexcept {
  return (vec_of_chunks_ == nullptr)
             ? const_iterator()
             : const_iterator(vec_of_chunks_ + id_start_,
                              (vec_of_chunks_ + id_start_)->chunk_head_id_);
}
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::iterator Deque<TempT, Alloc>::end() noexcept {
  return (vec_of_chunks_ == nullptr)
             ? iterator()
             : iterator(vec_of_chunks_ + id_finish_ - 1,
                        (vec_of_chunks_ + id_finish_ - 1)->chunk_tail_id_ - 1) +
                   1;
}
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::const_iterator Deque<TempT, Alloc>::end()
    const noexcept {
  return (vec_of_chunks_ == nullptr)
             ? const_iterator()
             : const_iterator(
                   vec_of_chunks_ + id_finish_ - 1,
                   (vec_of_chunks_ + id_finish_ - 1)->chunk_tail_id_ - 1) +
                   1;
}
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::const_iterator Deque<TempT, Alloc>::cend()
    const noexcept {
  return (vec_of_chunks_ == nullptr)
             ? const_iterator()
             : const_iterator(
                   vec_of_chunks_ + id_finish_ - 1,
                   (vec_of_chunks_ + id_finish_ - 1)->chunk_tail_id_ - 1) +
                   1;
}
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::reverse_iterator
Deque<TempT, Alloc>::rbegin() noexcept {
  return typename Deque<TempT, Alloc>::reverse_iterator(end());
}
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::const_reverse_iterator
Deque<TempT, Alloc>::rbegin() const noexcept {
  return typename Deque<TempT, Alloc>::const_reverse_iterator(end());
}
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::const_reverse_iterator
Deque<TempT, Alloc>::crbegin() const noexcept {
  return typename Deque<TempT, Alloc>::const_reverse_iterator(end());
}
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::reverse_iterator
Deque<TempT, Alloc>::rend() noexcept {
  return typename Deque<TempT, Alloc>::reverse_iterator(begin());
}
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::const_reverse_iterator Deque<TempT, Alloc>::rend()
    const noexcept {
  return typename Deque<TempT, Alloc>::const_reverse_iterator(begin());
}
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::const_reverse_iterator
Deque<TempT, Alloc>::crend() const noexcept {
  return typename Deque<TempT, Alloc>::const_reverse_iterator(begin());
}
//***********************************************
//**************Compare Operators****************
template <typename TempT, typename Alloc = std::allocator<TempT>>
bool operator==(const Deque<TempT, Alloc>& left,
                const Deque<TempT, Alloc>& right) noexcept {
  if (left.size() != right.size()) {
    return false;
  }
  typename Deque<TempT, Alloc>::const_iterator itl(left.begin());
  typename Deque<TempT, Alloc>::const_iterator itr(right.begin());
  while (itl != left.end() && itr != right.end() && *itl == *itr) {
    ++itl;
    ++itr;
  }
  return itl == left.end() && itr == right.end();
}
template <typename TempT, typename Alloc = std::allocator<TempT>>
bool operator!=(const Deque<TempT, Alloc>& left,
                const Deque<TempT, Alloc>& right) noexcept {
  return !(left == right);
}
//***********************************************
//**************Private Methods******************
template <typename TempT, typename Alloc>
void Deque<TempT, Alloc>::pr_chunk_deallocate(pointer ptr) noexcept {
  if (ptr == nullptr) {
    return;
  }
  type_alloc_traits::deallocate(get_type_allocator(), ptr, set_chunk_rank());
}
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::chunk_pointer
Deque<TempT, Alloc>::pr_vec_allocate(const size_t& len) {
  chunk_pointer tmp = chunk_alloc_traits::allocate(get_chunk_allocator(), len);
  size_t idx = 0;
  try {
    for (; idx < len; ++idx) {
      (tmp + idx)->chunk_body_ = pr_chunk_allocate();
      if (idx <= id_start_) {
        (tmp + idx)->chunk_head_id_ = (tmp + idx)->chunk_tail_id_ =
            set_chunk_rank();
      } else {
        (tmp + idx)->chunk_head_id_ = (tmp + idx)->chunk_tail_id_ = 0;
      }
      /// <- here can be quaestions
    }
  } catch (...) {
    for (size_t i = 0; i < idx; ++i) {
      pr_chunk_deallocate((tmp + i)->chunk_body_);
    }
    chunk_alloc_traits::deallocate(get_chunk_allocator(), tmp, len);
    throw;
  }
  return tmp;
}
template <typename TempT, typename Alloc>
void Deque<TempT, Alloc>::pr_move_construct_chunk_to_chunk(chunk_pointer src,
                                                           chunk_pointer dst) {
  size_t idx = src->chunk_head_id_;
  try {
    for (; idx < src->chunk_tail_id_; ++idx) {
      type_alloc_traits::construct(
          get_type_allocator(), dst->chunk_body_ + idx,
          std::move_if_noexcept(*(src->chunk_body_ + idx)));
    }
  } catch (...) {
    for (size_t i = src->chunk_head_id_; i < idx; ++i) {
      type_alloc_traits::destroy(get_type_allocator(), dst + i);
    }
    throw;
  }
  dst->chunk_head_id_ = src->chunk_head_id_;
  dst->chunk_tail_id_ = src->chunk_tail_id_;
}
template <typename TempT, typename Alloc>
void Deque<TempT, Alloc>::pr_copy_construct_chunk_to_chunk(chunk_pointer src,
                                                           chunk_pointer dst) {
  size_t idx = src->chunk_head_id_;
  try {
    for (; idx < src->chunk_tail_id; ++idx) {
      type_alloc_traits::construct(get_type_allocator(), dst->chunk_body_ + idx,
                                   *(src->chunk_body_ + idx));
    }
  } catch (...) {
    for (size_t i = src->chunk_head_id_; i < idx; ++i) {
      type_alloc_traits::destroy(get_type_allocator(), dst->chunk_body_ + i);
    }
    throw;
  }
  dst->chunk_head_id_ = src->chunk_head_id_;
  dst->chunk_tail_id_ = src->chunk_tail_id_;
}
template <typename TempT, typename Alloc>
void Deque<TempT, Alloc>::pr_chunk_destroy(chunk_pointer ptr) noexcept {
  if (ptr == nullptr) {
    return;
  }
  for (size_t i = ptr->chunk_head_id_; i < ptr->chunk_tail_id_; ++i) {
    type_alloc_traits::destroy(get_type_allocator(), ptr->chunk_body_ + i);
  }
}
template <typename TempT, typename Alloc>
void Deque<TempT, Alloc>::pr_vec_deallocate(chunk_pointer ptr,
                                            const size_t& len) noexcept {
  if (ptr == nullptr) {
    return;
  }
  for (size_t i = 0; i < len; ++i) {
    pr_chunk_deallocate((ptr + i)->chunk_body_);
  }
  chunk_alloc_traits::deallocate(get_chunk_allocator(), ptr, len);
}
template <typename TempT, typename Alloc>
void Deque<TempT, Alloc>::pr_clear_mem() noexcept {
  for (size_t i = id_start_; i < id_finish_; ++i) {
    pr_chunk_destroy(vec_of_chunks_ + i);
  }
  pr_vec_deallocate(vec_of_chunks_, num_of_chunks_);
}
template <typename TempT, typename Alloc>
void Deque<TempT, Alloc>::pr_vec_reallocation(const size_t& new_len,
                                              const size_t& delta) {
  id_start_ += delta;
  chunk_pointer tmp = pr_vec_allocate(new_len);
  id_start_ -= delta;
  size_t idx = id_start_;
  try {
    for (; idx < id_finish_; ++idx) {
      pr_move_construct_chunk_to_chunk(vec_of_chunks_ + idx, tmp + idx + delta);
    }
  } catch (...) {
    for (size_t i = id_start_; i < idx; ++i) {
      pr_chunk_destroy(tmp + i + delta);
    }
    pr_vec_deallocate(tmp, new_len);
    throw;
  }
  pr_clear_mem();
  vec_of_chunks_ = tmp;
  id_start_ += delta;
  id_finish_ += delta;
  num_of_chunks_ = new_len;
}
template <typename TempT, typename Alloc>
void Deque<TempT, Alloc>::pr_range_fill_init(const size_t& len,
                                             const TempT& value) {
  size_t idx = 0;
  const size_t kNumToBack = len / 2;
  const size_t kNumToFront = len - kNumToBack;
  try {
    for (; idx < kNumToFront; ++idx) {
      emplace_front(value);
    }
  } catch (...) {
    for (size_t i = 0; i < idx; ++i) {
      pop_front();
    }
    throw;
  }
  try {
    for (idx = 0; idx < kNumToBack; ++idx) {
      emplace_back(value);
    }
  } catch (...) {
    for (size_t i = 0; i < idx; ++i) {
      pop_back();
    }
    for (size_t i = 0; i < kNumToFront; ++i) {
      pop_front();
    }
    throw;
  }
}
template <typename TempT, typename Alloc>
void Deque<TempT, Alloc>::pr_range_default_init(const size_t& len) {
  size_t idx = 0;
  const size_t kNumToBack = len / 2;
  const size_t kNumToFront = len - kNumToBack;
  try {
    for (; idx < kNumToFront; ++idx) {
      emplace_front();
    }
  } catch (...) {
    for (size_t i = 0; i < idx; ++i) {
      pop_front();
    }
    throw;
  }
  try {
    for (idx = 0; idx < kNumToBack; ++idx) {
      emplace_back();
    }
  } catch (...) {
    for (size_t i = 0; i < idx; ++i) {
      pop_back();
    }
    for (size_t i = 0; i < kNumToFront; ++i) {
      pop_front();
    }
    throw;
  }
}
template <typename TempT, typename Alloc>
void Deque<TempT, Alloc>::pr_range_poper(const size_t& len) noexcept {
  for (size_t i = 0; i < len; ++i) {
    pop_back();
  }
}
template <typename TempT, typename Alloc>
template <typename Iter>
void Deque<TempT, Alloc>::pr_range_copy_init(Iter start, Iter finish) {
  size_t idx = 0;
  try {
    for (; start != finish; ++start) {
      emplace_back(*start);
      ++idx;
    }
  } catch (...) {
    pr_range_poper(idx);
    throw;
  }
}
template <typename TempT, typename Alloc>
void Deque<TempT, Alloc>::pr_clear() noexcept {
  if (vec_of_chunks_ != nullptr) {
    while (!empty()) {
      pop_back();
    }
  }
}
//***********************************************
//===============================================
