/**
 * @file deque.hpp
 * @author getylman
 * @date 13.03.2023
 */

/*
 * Я ЗАПРЕЩАЮ КОМУ ЛИБО МЕНЬ РАНГ ДЕКА 
 * ПРОБЛЕМЫ НА ВАЩУ СОВЕСТЬ
*/

#pragma once
#include <iterator>
#include <memory>
#include <type_traits>

template <typename TempT, typename Alloc = std::allocator<TempT>>
class Deque {
 protected:
  using value_type = TempT;
  // type of inserted type
  using allocator_type = Alloc;
  // type of inserted allocator
  using Type_alloc_type = typename std::allocator_traits<
      allocator_type>::template rebind<TempT>::other;
  // allocator type of inner type
  using Alloc_traits = std::allocator_traits<Type_alloc_type>;
  // allocator_traits
  using PrPtr = Alloc_traits::pointer;
  // private allocator traits pointer
  using PrPtr_const = Alloc_traits::const_pointer;
  // private allocator traits constant pointer
  using pointer = Alloc_traits::pointer;
  using const_pointer = Alloc_traits::const_pointer;
  using reference = Alloc_traits::reference;
  using const_reference = Alloc_traits::const_reference;
  using difference_type = ptrdiff_t;

 public:
  //===================Iterator===================
  template <bool IsConst>
  struct common_iterator;
  //**************Iterator usings*****************
  using iterator = common_iterator<false>;
  using const_iterator = common_iterator<true>;
  using reverse_iterator = std::reverse_iterator<common_iterator<false>>;
  using const_reverse_iterator = std::reverse_iterator<common_iterator<true>>;
  //**********************************************
  //==============================================

 private:
  const uint64_t kChunkSize = 512;  // size of chunks
                                    // I cannot explane why 512
  static constexpr const uint64_t set_chunk_rank() noexcept {
    const uint64_t kChunkRank = 512;
    return kChunkRank;
  }  // function to set a rank for chunk
  static constexpr const uint64_t set_chunk_rank(
      const uint64_t& rank) noexcept {
    return rank;
  }
  //====================Chunk=====================
  template <uint64_t ChunkRank = set_chunk_rank()>
  class Chunk;
  //==============================================
 protected:
  using Chunk_alloc_type =
      typename Alloc_traits::template rebind<Chunk<>>::other;
  // allocator type of chunk
  using Chunk_alloc_traits = std::allocator_traits<Chunk_alloc_type>;
  // allocator trits for chunk
  using Chunk_pointer = Chunk_alloc_traits::pointer;
  // pointer for chunks
 public:
  //=================Constructors=================
  Deque();  // check https://habr.com/ru/post/505632/
  explicit Deque(const size_t& count, const Alloc& alloc);
  explicit Deque(const Alloc& alloc);
  explicit Deque(const size_t& count, const TempT& value,
                 const Alloc& alloc = Alloc());
  template <typename AnotherAlloc = std::allocator<TempT>>
  Deque(const Deque<TempT, AnotherAlloc>& deq);
  template <typename AnotherAlloc = std::allocator<TempT>>
  Deque(Deque<TempT, AnotherAlloc>&& deq);
  ~Deque();
  template <typename AnotherAlloc = std::allocator<TempT>>
  Deque<TempT, AnotherAlloc>& operator=(
      const Deque<TempT, AnotherAlloc>& deq) &;
  // will called only for lvalue
  template <typename AnotherAlloc = std::allocator<TempT>>
  Deque<TempT, AnotherAlloc>& operator=(Deque<TempT, AnotherAlloc>&& deq) &&;
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
  void pop_back();
  void push_front(const TempT& elem);
  void push_front(TempT&& elem);
  void pop_front();
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
  uint64_t total_size_ = 0;       // size of in full container
  uint64_t num_of_chunks_ = 0;    // how many chunks have a container
  Chunk_pointer body_ = nullptr;  // pointer of full deque. the end is just
                                  // body plus size
  Chunk_pointer head_chunk_ = nullptr;  // pointer of current head
  Chunk_pointer tail_chunk_ = nullptr;  // pointer of current tail
};

// free space to my orientation
// free space to my orientation
// free space to my orientation
// free space to my orientation
// free space to my orientation
// free space to my orientation

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//    NO MEMBER DEFINITION AND DECLARATION
//=================ITERATOR==================
template <typename TempT, typename Alloc>
template <bool IsConst>
struct Deque<TempT, Alloc>::common_iterator {
  //***************Public usings****************
  using iterator_category = std::random_access_iterator_tag;
  using value_type = TempT;
  using pointer = TempT*;
  using reference = TempT&;
  using difference_type = ptrdiff_t;
  //********************************************
 private:
  //**************Private usings****************
  using conditional_ptr = std::conditional_t<IsConst, const pointer, pointer>;
  using conditional_ref =
      std::conditional_t<IsConst, const reference, reference>;
  // size_t index_;                // index of current nod in container
  conditional_ptr ptr_ = nullptr;         // pointer of current node
  conditional_ptr top_ptr_ = nullptr;     // pointer of upper bound
  conditional_ptr bottom_ptr_ = nullptr;  // pointer of lower bound
  Deque<TempT, Alloc>::Chunk<>* cur_chunk_ptr_ = nullptr;
  // pointer of current chunk

  //********************************************
 public:
  //****************Constructors****************
  common_iterator() noexcept {
  }
  common_iterator(const pointer& ptr,
                  const Deque<TempT, Alloc>::Chunk<>*& curnk_ptr) noexcept
      : ptr_(ptr),
        top_ptr_(curnk_ptr->chunk_body_),
        bottom_ptr_(curnk_ptr->chunk_body_ + curnk_ptr->get_chunk_rank()),
        cur_chunk_ptr_(curnk_ptr) {
  }  // constructor from two pointers
  template <bool IsConstTmp>
  common_iterator(const common_iterator<IsConstTmp>& iter) noexcept
      : ptr_(iter.ptr_),
        top_ptr_(iter.top_ptr_),
        bottom_ptr_(iter.bottom_ptr_),
        cur_chunk_ptr_(iter.cur_chunk_ptr_) {
    static_assert(std::is_same<decltype(*ptr_), decltype(*(iter.ptr_))>::value);
    // make CE if value types of iterators are not same
  }  // copy costructor
  ~common_iterator() {
  }
  template <bool IsConstTmp>
  common_iterator<IsConst>& operator=(const common_iterator<IsConstTmp>& iter) {
    ptr_ = iter.ptr_;
    cur_chunk_ptr_ = iter.cur_chunk_ptr_;
    top_ptr_ = iter.top_ptr_;
    bottom_ptr_ = iter.bottom_ptr_;
    return *this;
  }
  //********************************************
  //****************Memory operators************
  operator conditional_ptr() {
    return &**this;
  }  // operator c-style cast of iterator
  conditional_ref operator*() const noexcept {
    return *ptr_;
  }
  conditional_ptr operator->() const noexcept {
    return ptr_;
  }
  // template <typename IterT1>
  // IterT& operator->*(IterT1 IterT::*another_ptr) const noexcept {
  //   return (*ptr_).*another_ptr;
  // }
  // commented to the best times
  conditional_ref operator[](const int64_t& idx) const noexcept {
    return *(*this + idx);
  }
  void change_chunk(Chunk<> new_chunk) noexcept {
    cur_chunk_ptr_ = new_chunk;
    top_ptr_ = new_chunk.chunk_body_;
    bottom_ptr_ =
        new_chunk.chunk_body_ + static_cast<difference_type>(set_chunk_rank());
  }
  //********************************************
  //************Aithmetic operators*************
  common_iterator<IsConst>& operator+=(const int64_t& delta) noexcept {
    const int64_t distance = static_cast<int64_t>(ptr_ - top_ptr_) + delta;
    if (distance >= 0 and distance < static_cast<int64_t>(set_chunk_rank())) {
      ptr_ += distance;
    } else {
      const int64_t chunk_distance =
          (distance > 0)
              ? distance / static_cast<int64_t>(set_chunk_rank())
              : -((-distance - 1) / static_cast<int64_t>(set_chunk_rank())) - 1;
      change_chunk(cur_chunk_ptr_ + chunk_distance);
      ptr_ = top_ptr_ + distance -
             chunk_distance * static_cast<int64_t>(set_chunk_rank());
    }
    return *this;
  }
  common_iterator<IsConst>& operator-=(const int64_t& delta) noexcept {
    return *this += -delta;
  }
  common_iterator<IsConst> operator+(const int64_t& delta) noexcept {
    common_iterator<IsConst> tmp = *this;
    tmp += delta;
    return tmp;
  }
  common_iterator<IsConst> operator-(const int64_t& delta) noexcept {
    common_iterator<IsConst> tmp = *this;
    tmp -= delta;
    return tmp;
  }
  common_iterator<IsConst>& operator++() noexcept {
    return *this += 1;
  }
  common_iterator<IsConst>& operator--() noexcept {
    return *this -= 1;
  }
  common_iterator<IsConst> operator++(int) noexcept {
    return ++common_iterator<IsConst>(*this);
  }
  common_iterator<IsConst> operator--(int) noexcept {
    return --common_iterator<IsConst>(*this);
  }

  difference_type operator-(const common_iterator<IsConst>& iter) noexcept {
    return static_cast<difference_type>(set_chunk_rank()) *
           static_cast<difference_type>(
               cur_chunk_ptr_ - iter.cur_chunk_ptr_ -
               static_cast<int64_t>(cur_chunk_ptr_ != nullptr) +
               (ptr_ - top_ptr_) + (iter.bottom_ptr_ - iter.ptr_));
  }
  //********************************************
  //**************Compare operators*************
  friend bool operator<(const common_iterator<IsConst>& it1,
                        const common_iterator<IsConst>& it2) noexcept {
    return (it1.cur_chunk_ptr_ == it2.cur_chunk_ptr_)
               ? it1.ptr_ < it2.ptr_
               : it1.cur_chunk_ptr_ < it2.cur_chunk_ptr_;
    // тут надо проверять находимся ли мы в одном чанке или нет
  }
  //********************************************
};

//************Aithmetic operators************
template <typename TempT, typename Alloc = std::allocator<TempT>, bool IsConst>
typename Deque<TempT, Alloc>::common_iterator<IsConst> operator+(
    const int64_t& delta,
    const typename Deque<TempT, Alloc>::common_iterator<IsConst>&
        iter) noexcept {
  return iter + delta;
}
template <typename TempT, typename Alloc = std::allocator<TempT>, bool IsConst>
typename Deque<TempT, Alloc>::common_iterator<IsConst> operator-(
    const int64_t& delta,
    const typename Deque<TempT, Alloc>::common_iterator<IsConst>&
        iter) noexcept {
  return iter - delta;
}
//*******************************************
//*************Compare operators*************
template <typename TempT, typename Alloc = std::allocator<TempT>, bool IsConst>
bool operator>(
    const typename Deque<TempT, Alloc>::common_iterator<IsConst>& it1,
    const typename Deque<TempT, Alloc>::common_iterator<IsConst>&
        it2) noexcept {
  return it2 < it1;
}
template <typename TempT, typename Alloc = std::allocator<TempT>, bool IsConst>
bool operator>=(
    const typename Deque<TempT, Alloc>::common_iterator<IsConst>& it1,
    const typename Deque<TempT, Alloc>::common_iterator<IsConst>&
        it2) noexcept {
  return !(it1 < it2);
}
template <typename TempT, typename Alloc = std::allocator<TempT>, bool IsConst>
bool operator<=(
    const typename Deque<TempT, Alloc>::common_iterator<IsConst>& it1,
    const typename Deque<TempT, Alloc>::common_iterator<IsConst>&
        it2) noexcept {
  return !(it1 > it2);
}
template <typename TempT, typename Alloc = std::allocator<TempT>, bool IsConst>
bool operator==(
    const typename Deque<TempT, Alloc>::common_iterator<IsConst>& it1,
    const typename Deque<TempT, Alloc>::common_iterator<IsConst>&
        it2) noexcept {
  return !(it1 < it2 || it1 > it2);
}
template <typename TempT, typename Alloc = std::allocator<TempT>, bool IsConst>
bool operator!=(
    const typename Deque<TempT, Alloc>::common_iterator<IsConst>& it1,
    const typename Deque<TempT, Alloc>::common_iterator<IsConst>&
        it2) noexcept {
  return !(it1 == it2);
}
//*******************************************
//===========================================

//==================Chunk====================
template <typename TempT, typename Alloc>
template <uint64_t ChunkRank>
class Deque<TempT, Alloc>::Chunk : public Type_alloc_type {
 private:
  uint64_t chunk_size_ = 0;      // current size
  int16_t changing_delta_ = -1;  // delta of changing the index
  uint64_t boundary_wall_ = 0;   // the limit of increasing index
  Deque<TempT, Alloc>::pointer chunk_body_ = nullptr;  // pointer of head
  bool is_head_ = true;     // means is this chunk is head node or tail node
  Type_alloc_type all_tp_;  // object of our allocator

 public:
  //****************Constructors***************
  Chunk(const bool& is_head) noexcept(std::is_nothrow_constructible_v<TempT>)
      : is_head_(is_head) {
    try {
      chunk_body_ = Alloc_traits::allocate(all_tp_, ChunkRank);
    } catch (...) {
      throw;  // if allocation of memory failed it will stop constructing
              // and throw the exception of this situation
    }
    changing_delta_ = (is_head_) ? -1 : 1;
    boundary_wall_ = (is_head_) ? 0 : ChunkRank - 1;
    chunk_size_ = (is_head_) ? ChunkRank - 1 : 0;
  }
  template <typename AnotherAlloc>
  Chunk(const typename Deque<TempT, AnotherAlloc>::Chunk<>& chnk) {
  }
  ~Chunk() {
    Alloc_traits::deallocate(all_tp_, chunk_body_, ChunkRank);
  }
  //*******************************************
  //*****************Getters*******************
  uint64_t get_chunk_size() const noexcept {
    return (is_head_) ? boundary_wall_ - chunk_size_ : chunk_size_;
  }  // <- how many cells are filled
  bool chunk_empty() const noexcept {
    return get_chunk_size() == 0;
  }
  bool is_ghunk_full() const noexcept {
    return boundary_wall_ == chunk_size_;
  }
  constexpr const uint64_t get_chunk_rank() const noexcept {
    return ChunkRank;
  }
  //*******************************************
  //*****************Setters*******************
  void set_in_chunk(const value_type& value) {
    try {
      Alloc_traits::construct(all_tp_, chunk_body_ + chunk_size_, value);
    } catch (...) {
      throw;  // if construction of object failed it will stop constructing
              // and throw the exception of this situation
    }
    chunk_size_ += changing_delta_;
  }
  void set_in_chunk(value_type&& value) {
    try {
      Alloc_traits::construct(all_tp_, chunk_body_ + chunk_size_,
                              std::move_if_noexcept(value));
    } catch (...) {
      throw;  // if construction of object failed it will stop constructing
              // and throw the exception of this situation
    }
    chunk_size_ += changing_delta_;
  }
  //*******************************************
  //*****************Erasers*******************
  void pop_from_chunk() noexcept {
    Alloc_traits::destroy(all_tp_, chunk_body_ + chunk_size_ - 1);
    // if chunk was empty it will be UB
    chunk_size_ -= changing_delta_;
  }
  //*******************************************
};
//===========================================
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//               DECLARATION

//================Iterator methods===========
//===========================================

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^