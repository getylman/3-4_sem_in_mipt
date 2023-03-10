/**
 * @file deque.hpp
 * @author getylman
 * @date 08.03.2023
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
  template <bool IsConst, typename IterT>
  struct common_iterator;
  //==============================================

 private:
  const uint64_t kChunkSize = 512;  // size of chunks
                                    // I cannot explane why 512
  static constexpr const uint64_t set_chunk_rank() noexcept {
    const uint64_t kChunkRank = 512;
    return kChunkRank;
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
  template <typename AnotherAlloc = std::allocator<TempT>>
  explicit Deque(const Alloc& alloc);
  template <typename AnotherAlloc = std::allocator<TempT>>
  explicit Deque(const size_t& count, const TempT& value,
                 const AnoterAlloc& alloc = AnoterAlloc());
  template <typename AnotherAlloc = std::allocator<TempT>>
  Deque(const Deque<TempT, AnotherAlloc>& deq);
  template <typename AnotherAlloc = std::allocator<TempT>>
  Deque(Deque<TempT, AnotherAlloc>&& deq);
  ~Deque();
  template <typename AnotherAlloc = std::allocator<TempT>>
  Deque<TempT, AnotherAlloc>& operator=(const Deque<TempT, AnotherAlloc>& deq) &;
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
  //==================ITERATOR====================

  // отдельный итератор на чанк и чанкходер через юзинги

  //**************Iterator usings*****************
  using iterator = common_iterator<false, TempT>;
  using const_iterator = common_iterator<true, TempT>;
  using reverse_iterator = std::reverse_iterator<common_iterator<false, TempT>>;
  using const_reverse_iterator = std::reverse_iterator<common_iterator<
      true, TempT>>;  /// TODO: вместо TempT вставить норм типы а так я оставил
                      /// как затычку чтоб исправить CE
  //**********************************************
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
template <bool IsConst, typename IterT>
struct Deque<TempT, Alloc>::common_iterator {
  //***************Public usings****************
  using iterator_category = std::random_access_iterator_tag;
  using value_type = IterT;
  using pointer = IterT*;
  using reference = IterT&;
  using difference_type = ptrdiff_t;
  //********************************************
 private:
  //**************Private usings****************
  using conditional_ptr = std::conditional_t<IsConst, const pointer, pointer>;
  using conditional_ref =
      std::conditional_t<IsConst, const reference, reference>;
  conditional_ptr ptr_;  // pointer of current node
  size_t index_;         // index of current nod in container

  //********************************************
 public:
  //****************Constructors****************
  common_iterator<IsConst, IterT>() noexcept : ptr_(nullptr), index_(0) {
  }
  ~common_iterator<IsConst, IterT>() {
    ptr_ = nullptr;
    index_ = 0;
  }
  common_iterator<true, IterT>& operator=(const common_iterator<>)
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
  template <typename IterT1>
  IterT& operator->*(IterT1 IterT::*another_ptr) const noexcept {
    return (*ptr_).*another_ptr;
  }
  conditional_ref operator[](const int64_t& idx) const noexcept {
    return *(*this + idx);
  }
  //********************************************
  //************Aithmetic operators*************
  common_iterator<IsConst, IterT>& operator+=(const int64_t& delta) noexcept;
  common_iterator<IsConst, IterT>& operator-=(const int64_t& delta) noexcept;
  common_iterator<IsConst, IterT> operator+(const int64_t& delta) noexcept;
  common_iterator<IsConst, IterT> operator-(const int64_t& delta) noexcept;
  common_iterator<IsConst, IterT>& operator++() noexcept;
  common_iterator<IsConst, IterT>& operator--() noexcept;
  common_iterator<IsConst, IterT>& operator++(int) noexcept;
  common_iterator<IsConst, IterT>& operator--(int) noexcept;
  friend  // блять снова переделывать после добавления второго шаблонного
          // аргумента
      typename Deque<TempT, Alloc>::common_iterator<IsConst,
                                                    IterT>::difference_type
      operator-(
          const typename Deque<TempT, Alloc>::common_iterator<IsConst, IterT>&
              it1,
          const typename Deque<TempT, Alloc>::common_iterator<IsConst, IterT>&
              it2) noexcept {
    return (it1.index_ == it2.index_)
               ? it2.ptr_ - it2.ptr_
               : it2.ptr_ - it1.ptr_ * (it2.index_ - it1.index_ -
                                        1);  // надо переделать тк передумал над
                                             // концепцией итератора
    // тут совсем не верная логика надо сделать отдельную специализацию под
    // итератор типа и отдельна для чанка
  }
  //********************************************
  //**************Compare operators*************
  friend bool operator<(
      const typename Deque<TempT, Alloc>::common_iterator<IsConst, IterT>&
          iter_left,
      const typename Deque<TempT, Alloc>::common_iterator<IsConst, IterT>&
          iter_right) noexcept {
    return (iter_left.index_ == iter_right.index_)
               ? iter_left.ptr_ < iter_right.ptr_
               : iter_left.index_ < iter_right.index_;
    // тут надо проверять находимся ли мы в одном чанке или нет
  }
  //********************************************
};

//************Aithmetic operators************
// template <typename TempT, bool IsConst, typename IterT,
//           typename Alloc =
//               std::allocator<TempT> >>
//               typename Deque<TempT, Alloc>::common_iterator<IsConst, IterT>
//               operator+(const int64_t& delta,
//                         const typename Deque<TempT, Alloc>::common_iterator<
//                             IsConst, IterT>& iter) noexcept {
//   return iter + delta;
// }
// template <typename TempT, typename Alloc = std::allocator<TempT>, bool
// IsConst> typename Deque<TempT, Alloc>::common_iterator<IsConst> operator-(
//     const int64_t& delta,
//     const typename Deque<TempT, Alloc>::common_iterator<IsConst>&
//         iter) noexcept;
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
  uint64_t chunk_size_ = 0;       // current size
  int16_t changing_delta_ = -1;   // delta of changing the index
  uint64_t boundary_wall_ = 0;    // the limit of increasing index
  pointer chunk_body_ = nullptr;  // pointer of head
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