/**
 * @file deque.hpp
 * @author getylman
 * @date 3.04.2023
 */

/*
 * Я ЗАПРЕЩАЮ КОМУ ЛИБО МЕНЬ РАНГ ДЕКА
 * ПРОБЛЕМЫ НА ВАЩУ СОВЕСТЬ
 */

#pragma once
#include <iterator>
#include <memory>
// #include "allocator_traits.h"
#include <type_traits>
#include <vector>

template <typename TempT, typename Alloc = std::allocator<TempT>>
class Deque {
 protected:
  using value_type = TempT;
  // type of inserted type
  using allocator_type = Alloc;
  // type of inserted allocator
  // using Type_alloc_type = typename
  // std::allocator_traits<allocator_type>::template rebind<TempT>::other;
  // allocator type of inner type
  using alloc_traits = std::allocator_traits<allocator_type>;
  // allocator_traits
  using pointer = TempT*;
  using const_pointer = const TempT*;
  using reference = TempT&;
  using const_reference = const TempT&;
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
  // const uint64_t kChunkSize = 512;  // size of chunks
  //                                   // I cannot explane why 512
  static constexpr uint64_t set_chunk_rank() noexcept {
    const uint64_t kChunkRank = 512;
    return kChunkRank;
  }  // function to set a rank for chunk
  // static constexpr const uint64_t set_chunk_rank(
  //     const uint64_t& rank) noexcept {
  //   return rank;
  // }
  //====================Chunk=====================
  class Chunk;
  using chunk_pointer = Chunk*;
  //==============================================
  //===============Deque Body=====================
  struct DequeBody;
  // struct to keep all Deque fields
  //==============================================

 public:
  //=================Constructors=================
  static_assert(std::is_same_v<typename Alloc::value_type, TempT>);
  // static_assert(std::is_default_constructible_v<TempT>);
  // if types are not same will be CE
  Deque();
  explicit Deque(const size_t& count, const Alloc& alloc = Alloc());
  // explicit Deque(const Alloc& alloc);
  explicit Deque(const size_t& count, const TempT& value,
                 const Alloc& alloc = Alloc());
  Deque(const Deque<TempT, Alloc>& deq);
  Deque(Deque<TempT, Alloc>&& deq);
  ~Deque() noexcept;
  // template <typename AnotherAlloc = std::allocator<TempT>>
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
  iterator insert(const_iterator pos, const TempT& value);
  iterator insert(const_iterator pos, TempT&& value);
  iterator erase(const_iterator pos);
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
  //================Deque Fields=================
  DequeBody body_;  // full body of our container

  //=============================================
};

//===================CHUNK=======================
template <typename TempT, typename Alloc>
class Deque<TempT, Alloc>::Chunk {
 private:
  uint64_t chunk_size_ = 0;            // current size
  int16_t l_changing_delta_ = -1;      // delta for moving to left
  int16_t r_changing_delta_ = 1;       // delta for moving to right
  pointer l_chunk_confine_ = nullptr;  // left confine
  pointer r_chunk_confine_ = nullptr;  // right confine
  pointer chunk_body_ = nullptr;       // pointer of head
  pointer chunk_head_ = nullptr;       // head end of using chunk
  pointer chunk_tail_ = nullptr;       // tail end of using chunk
  // bool is_head_ = true;     // means is this chunk is head node or tail node
  allocator_type all_tp_;  // object of our allocator
  //**************Private Functions************
  void allocation_attempt_of_chunk(pointer& ptr) {
    try {
      ptr = alloc_traits::allocate(all_tp_, set_chunk_rank());
    } catch (...) {  // if allocation of memory failed it will stop
      throw;         // constructing and throw the exception of this situation
    }
  }  // tring to allocate memeory for chunk
  void coping_chunk_inside(const Chunk& src, Chunk& dst) noexcept {
    // src -> source   dst -> destination
    dst.chunk_head_ = dst.chunk_body_ + (src.chunk_head_ - src.chunk_body_);
    dst.chunk_tail_ = dst.chunk_body_ + (src.chunk_tail_ - src.chunk_body_);
    dst.r_chunk_confine_ =
        dst.chunk_body_ + (src.r_chunk_confine_ - src.chunk_body_);
    dst.l_chunk_confine_ =
        dst.chunk_body_ + (src.l_chunk_confine_ - src.chunk_body_);
    dst.chunk_size_ = src.chunk_size_;
  }  // function for copy chunk fields
  /// TODO: исправь эту функцию
  void copy_attempt_of_chunk(const Chunk& src, Chunk& dst) {
    // src -> source   dst -> destination
    uint64_t idx = static_cast<uint64_t>(src.chunk_head_ - src.chunk_body_);
    try {
      for (; idx <= static_cast<uint64_t>(src.chunk_tail_ - src.chunk_body_);
           ++idx) {
        alloc_traits::construct(all_tp_, dst.chunk_body_ + idx,
                                *(src.chunk_body_ + idx));
      }
    } catch (...) {
      for (uint64_t i =
               static_cast<uint64_t>(src.chunk_head_ - src.chunk_body_);
           i < idx; ++i) {
        alloc_traits::destroy(all_tp_, dst.chunk_body_ + i);
      }
      alloc_traits::deallocate(all_tp_, dst.chunk_body_, set_chunk_rank());
      throw;
    }
  }  // tring to copy from src to dst chunk
  void construct_unit_attempt_of_chunk(pointer& ptr, const TempT& value,
                                       int16_t& changing_delta) {
    if (chunk_size_ != 0) {
      ptr += changing_delta;
    }
    try {
      alloc_traits::construct(all_tp_, ptr, value);
      // если передастся lvalue то просто скопируется а при rvalue будет просто
      // перемещение
    } catch (...) {
      if (chunk_size_ != 0) {
        ptr -= changing_delta;
      }
      throw;  // if construction of object failed it will stop constructing
              // and throw the exception of this situation
    }
    ++chunk_size_;
  }
  void construct_unit_attempt_of_chunk(pointer& ptr, TempT&& value,
                                       int16_t& changing_delta) {
    if (chunk_size_ != 0) {
      ptr += changing_delta;
    }
    try {
      alloc_traits::construct(all_tp_, ptr, std::move_if_noexcept(value));
      // если передастся lvalue то просто скопируется а при rvalue будет просто
      // перемещение
    } catch (...) {
      if (chunk_size_ != 0) {
        ptr -= changing_delta;
      }
      throw;  // if construction of object failed it will stop constructing
              // and throw the exception of this situation
    }
    ++chunk_size_;
  }
  //*******************************************

 public:
  //****************Constructors***************
  Chunk() = default;
  // noexcept(std::is_nothrow_constructible_v<TempT>)
  Chunk(const bool& is_head) {
    // не может быть noexcept тк при попытки аллоцирования пула может вылететь
    // исключение
    allocation_attempt_of_chunk(chunk_body_);
    l_chunk_confine_ = chunk_body_;
    r_chunk_confine_ = chunk_body_ + set_chunk_rank() - 1;
    chunk_tail_ = chunk_head_ = (is_head) ? r_chunk_confine_ : l_chunk_confine_;
    // I hope that you remember the construction which one you used
  }
  ~Chunk() {
    if (chunk_size_ != 0) {
      // if one of elements of chunk is constructed
      pointer cur_node = chunk_head_;
      for (; cur_node <= chunk_tail_; ++cur_node) {
        alloc_traits::destroy(all_tp_, cur_node);
      }
    }
    if (chunk_body_ != nullptr) {
      alloc_traits::deallocate(all_tp_, chunk_body_, set_chunk_rank());
    }
    chunk_body_ = chunk_head_ = chunk_tail_ = r_chunk_confine_ =
        l_chunk_confine_ = nullptr;
    chunk_size_ = 0;
  }
  Chunk(const typename Deque<TempT, Alloc>::Chunk& chnk) {
    if (chnk.chunk_body_ != nullptr) {
      allocation_attempt_of_chunk(chunk_body_);
      copy_attempt_of_chunk(chnk, *this);
      chunk_size_ = chnk.chunk_size_;
      chunk_head_ = chunk_body_ + (chnk.chunk_head_ - chnk.chunk_body_);
      chunk_tail_ = chunk_body_ + (chnk.chunk_tail_ - chnk.chunk_body_);
      r_chunk_confine_ =
          (l_chunk_confine_ = chunk_body_) + set_chunk_rank() - 1;
    }
  }
  Chunk(Chunk&& chnk) noexcept
      : chunk_body_(std::move(chnk.chunk_body_)),
        chunk_size_(std::move(chnk.chunk_size_)),
        chunk_head_(std::move(chnk.chunk_head_)),
        chunk_tail_(std::move(chnk.chunk_tail_)),
        l_chunk_confine_(std::move(chnk.l_chunk_confine_)),
        r_chunk_confine_(std::move(chnk.r_chunk_confine_)) {
    chnk.chunk_body_ = chnk.chunk_head_ = chnk.chunk_tail_ =
        chnk.r_chunk_confine_ = chnk.l_chunk_confine_ = nullptr;
    chnk.chunk_size_ = 0;
  }
  Chunk& operator=(const Chunk& chnk) & {
    if (chunk_body_ != chnk.chunk_body_) {
      Chunk tmp_chunk;
      allocation_attempt_of_chunk(tmp_chunk.chunk_body_);
      copy_attempt_of_chunk(chnk, tmp_chunk);
      for (uint64_t i = static_cast<uint64_t>(chunk_head_ - chunk_head_);
           i <= static_cast<uint64_t>(chunk_tail_ - chunk_body_); ++i) {
        alloc_traits::destroy(all_tp_, chunk_body_ + i);
      }
      alloc_traits::deallocate(all_tp_, chunk_body_, set_chunk_rank());
      chunk_body_ = tmp_chunk.chunk_body_;
      coping_chunk_inside(chnk, *this);
    }
    return *this;
  }
  Chunk& operator=(Chunk&& chnk) & noexcept {
    if (chunk_body_ != chnk.chunk_body_) {
      if (chunk_size_ != 0) {
        // if one of elements of chunk is constructed
        for (uint64_t i = static_cast<uint64_t>(chunk_head_ - chunk_body_);
             i <= static_cast<uint64_t>(chunk_tail_ - chunk_body_); ++i) {
          alloc_traits::destroy(all_tp_, chunk_body_ + i);
        }
      }
      if (chunk_body_ != nullptr) {
        alloc_traits::deallocate(all_tp_, chunk_body_, set_chunk_rank());
      }
      chunk_body_ = chunk_head_ = chunk_tail_ = r_chunk_confine_ =
          l_chunk_confine_ = nullptr;
      chunk_size_ = 0;
      chunk_body_ = std::move(chnk.chunk_body_);
      chunk_size_ = std::move(chnk.chunk_size_);
      chunk_head_ = std::move(chnk.chunk_head_);
      chunk_tail_ = std::move(chnk.chunk_tail_);
      l_chunk_confine_ = std::move(chnk.l_chunk_confine_);
      r_chunk_confine_ = std::move(chnk.r_chunk_confine_);
      chnk.chunk_body_ = chnk.chunk_head_ = chnk.chunk_tail_ =
          chnk.r_chunk_confine_ = chnk.l_chunk_confine_ = nullptr;
      chnk.chunk_size_ = 0;
    }
    return *this;
  }

  //*******************************************
  //*****************Getters*******************
  uint64_t get_chunk_size() const noexcept {
    return chunk_size_;
  }  // <- how many cells are filled
  bool chunk_empty() const noexcept { return get_chunk_size() == 0; }
  bool chunk_full() const noexcept { return chunk_size_ == set_chunk_rank(); }
  constexpr uint64_t get_chunk_rank() const noexcept {
    return set_chunk_rank();
  }
  TempT& get_chunk_head() noexcept { return *chunk_head_; }
  const TempT& get_chunk_head() const noexcept { return *chunk_head_; }
  TempT& get_chunk_tail() noexcept { return *chunk_tail_; }
  const TempT& get_chunk_tail() const noexcept { return *chunk_tail_; }
  pointer& get_left_chunk_confine() noexcept { return l_chunk_confine_; }
  const pointer& get_left_chunk_confine() const noexcept {
    return l_chunk_confine_;
  }
  pointer& get_right_chunk_confine() noexcept { return r_chunk_confine_; }
  const pointer& get_right_chunk_confine() const noexcept {
    return r_chunk_confine_;
  }
  TempT& operator[](const size_t& index) noexcept {
    return *(chunk_head_ + index);
  }
  const TempT& operator[](const size_t& index) const noexcept {
    return *(chunk_head_ + index);
  }
  //*******************************************
  //*****************Setters*******************
  void left_set_chunk(const value_type& value) {
    construct_unit_attempt_of_chunk(chunk_head_, value, l_changing_delta_);
    // if in construct function throws exception it will forward further
  }
  void right_set_chunk(const value_type& value) {
    construct_unit_attempt_of_chunk(chunk_tail_, value, r_changing_delta_);
    // if in construct function throws exception it will forward further
  }
  void left_set_chunk(value_type&& value) {
    construct_unit_attempt_of_chunk(chunk_head_, std::move_if_noexcept(value),
                                    l_changing_delta_);
    // что там после перемещения останется неопределенное значени то мне без
    // разницы
    // if in construct function throws exception it will forward further
  }
  void right_set_chunk(value_type&& value) {
    construct_unit_attempt_of_chunk(chunk_tail_, std::move_if_noexcept(value),
                                    r_changing_delta_);
    // что там после перемещения останется неопределенное значени то мне без
    // разницы
    // if in construct function throws exception it will forward further
  }
  //*******************************************
  //*****************Erasers*******************
  void left_pop_chunk() noexcept {
    alloc_traits::destroy(all_tp_, chunk_head_);
    // if chunk was empty it will be UB
    --chunk_size_;
    if (chunk_size_ != 0) {
      chunk_head_ -= l_changing_delta_;
    }
  }
  void right_pop_chunk() noexcept {
    alloc_traits::destroy(all_tp_, chunk_tail_);
    // if chunk was empty it will be UB
    --chunk_size_;
    if (chunk_size_ != 0) {
      chunk_tail_ -= r_changing_delta_;
    }
  }
  //*******************************************
};
//===============================================

//=================ITERATOR==================
template <typename TempT, typename Alloc>
template <bool IsConst>
struct Deque<TempT, Alloc>::common_iterator {
  //***************Public usings****************
  using iterator_category = std::random_access_iterator_tag;
  using value_type = TempT;
  using pointer = TempT*;
  using reference = TempT&;
  using difference_type = int64_t;  // ptrdiff_t;
  //********************************************
 private:
  //**************Private usings****************
  using conditional_ptr = std::conditional_t<IsConst, const TempT*, TempT*>;
  using conditional_ref = std::conditional_t<IsConst, const TempT&, TempT&>;
  //********************************************
  //****************Fields**********************
  conditional_ptr ptr_ = nullptr;  // pointer of current node
  chunk_pointer cur_chunk_ptr_ = nullptr;
  // pointer of current chunk
  //********************************************

  //********************************************
  //***********Private Functions****************
  void change_chunk(const chunk_pointer& new_chunk) noexcept {
    cur_chunk_ptr_ = new_chunk;
  }
  //********************************************
 public:
  //****************Constructors****************
  common_iterator() noexcept = default;
  common_iterator(const pointer& ptr, const chunk_pointer& curnk_ptr) noexcept
      : ptr_(ptr),
        cur_chunk_ptr_(curnk_ptr) {}  // constructor from two pointers
  template <bool IsConstTmp>
  common_iterator(const common_iterator<IsConstTmp>& iter) noexcept
      : ptr_(iter.operator->()), cur_chunk_ptr_(iter.get_chunk_pointer()) {
    static_assert(std::is_same_v<decltype(const_cast<TempT&>(*ptr_)),
                                 decltype(const_cast<TempT&>(*iter))>);
    // make CE if value types of iterators are not same
  }  // copy costructor
  ~common_iterator() {}
  template <bool IsConstTmp>
  common_iterator<IsConst>& operator=(
      const common_iterator<IsConstTmp>& iter) & noexcept {
    ptr_ = get_pointer(iter);
    cur_chunk_ptr_ = get_chunk_pointer(iter);
    return *this;
  }
  //********************************************
  //****************Memory operators************
  operator conditional_ptr() const noexcept { return &(*(*this)); }
  // operator c-style cast of iterator
  conditional_ref operator*() const noexcept { return *ptr_; }
  conditional_ptr operator->() const noexcept { return ptr_; }
  chunk_pointer get_chunk_pointer() const noexcept { return cur_chunk_ptr_; }
  // template <typename IterT1>
  // IterT& operator->*(IterT1 IterT::*another_ptr) const noexcept {
  //   return (*ptr_).*another_ptr;
  // }
  // commented to the best times
  conditional_ref operator[](const int64_t& idx) const noexcept {
    return *(*this + idx);
  }
  //********************************************
  //************Aithmetic operators*************
  common_iterator<IsConst>& operator+=(const difference_type& delta) noexcept {
    const difference_type kDistance =
        static_cast<difference_type>(ptr_ -
                                     cur_chunk_ptr_->get_left_chunk_confine()) +
        delta;
    if (kDistance >= 0 and
        kDistance < static_cast<difference_type>(set_chunk_rank())) {
      ptr_ += delta;
    } else {
      const difference_type kChunkDistance =
          (kDistance > 0)
              ? kDistance / static_cast<difference_type>(set_chunk_rank())
              : -((-kDistance - 1) /
                  static_cast<difference_type>(set_chunk_rank())) -
                    1;
      change_chunk(cur_chunk_ptr_ + kChunkDistance);
      ptr_ = cur_chunk_ptr_->get_left_chunk_confine() + kDistance -
             kChunkDistance * static_cast<difference_type>(set_chunk_rank());
    }
    return *this;
  }
  common_iterator<IsConst>& operator-=(const difference_type& delta) noexcept {
    return *this += -delta;
  }
  common_iterator<IsConst> operator+(const difference_type& delta) noexcept {
    common_iterator<IsConst> tmp = *this;
    tmp += delta;
    return tmp;
  }
  common_iterator<IsConst> operator-(const difference_type& delta) noexcept {
    common_iterator<IsConst> tmp = *this;
    tmp -= delta;
    return tmp;
  }
  common_iterator<IsConst>& operator++() noexcept { return *this += 1; }
  common_iterator<IsConst>& operator--() noexcept { return *this -= 1; }
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
                   static_cast<difference_type>(cur_chunk_ptr_ != nullptr)) +
           (ptr_ - cur_chunk_ptr_->get_left_chunk_confine()) +
           (iter.cur_chunk_ptr_->get_right_chunk_confine() - iter.ptr_ +
            1);  // ?
  }
  //********************************************
  //**************Compare operators*************
  template <bool IsConstTmp>
  friend bool operator<(const common_iterator<IsConst>& it1,
                        const common_iterator<IsConstTmp>& it2) noexcept {
    return (it1.cur_chunk_ptr_ == it2.cur_chunk_ptr_)
               ? it1.ptr_ < it2.ptr_
               : it1.cur_chunk_ptr_ < it2.cur_chunk_ptr_;
    // тут надо проверять находимся ли мы в одном чанке или нет
  }
  //********************************************
};

//************Aithmetic operators************
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
//*******************************************
//*************Compare operators*************
template <typename TempT, bool IsConst, bool IsConstTmp,
          typename Alloc = std::allocator<TempT>>
bool operator>(
    const typename Deque<TempT, Alloc>::template common_iterator<IsConst>& it1,
    const typename Deque<TempT, Alloc>::template common_iterator<IsConstTmp>&
        it2) noexcept {
  return it2 < it1;
}
template <typename TempT, bool IsConst, bool IsConstTmp,
          typename Alloc = std::allocator<TempT>>
bool operator>=(
    const typename Deque<TempT, Alloc>::template common_iterator<IsConst>& it1,
    const typename Deque<TempT, Alloc>::template common_iterator<IsConstTmp>&
        it2) noexcept {
  return !(it1 < it2);
}
template <typename TempT, bool IsConst, bool IsConstTmp,
          typename Alloc = std::allocator<TempT>>
bool operator<=(
    const typename Deque<TempT, Alloc>::template common_iterator<IsConst>& it1,
    const typename Deque<TempT, Alloc>::template common_iterator<IsConstTmp>&
        it2) noexcept {
  return !(it1 > it2);
}
template <typename TempT, bool IsConst, bool IsConstTmp,
          typename Alloc = std::allocator<TempT>>
bool operator==(
    const typename Deque<TempT, Alloc>::template common_iterator<IsConst>& it1,
    const typename Deque<TempT, Alloc>::template common_iterator<IsConstTmp>&
        it2) noexcept {
  return !(it1 < it2 || it1 > it2);
}
template <typename TempT, bool IsConst, bool IsConstTmp,
          typename Alloc = std::allocator<TempT>>
bool operator!=(
    const typename Deque<TempT, Alloc>::template common_iterator<IsConst>& it1,
    const typename Deque<TempT, Alloc>::template common_iterator<IsConstTmp>&
        it2) noexcept {
  return !(it1 == it2);
}
//*******************************************
//===========================================

//=================DEQUE BODY====================
template <typename TempT, typename Alloc>
struct Deque<TempT, Alloc>::DequeBody {
 private:
  //*****************Fields******************
  const uint64_t kDefaultDequeLenth = 4;
  // --------------------------------
  // reason why default 4
  // # - using space, * - free space
  // ******
  // ****##
  // ##****
  // ******
  // --------------------------------
  // size_t using_len = 0;
  size_t id_start_ = 0;
  size_t id_finish_ = 0;
  allocator_type db_all_;
  std::vector<Chunk> vec_of_chunks_;
  //******************************************
  //*************Private Functions************
  //```````````Functions in Constructor```````
  void db_allocation_attempt(const size_t& count) {
    try {
      vec_of_chunks_.resize(count);
    } catch (...) {
      throw;
    }
  }
  void db_construction_attempt(const size_t& full_size) {
    size_t idx = 0;
    try {
      for (; idx < full_size / 2; ++idx) {
        vec_of_chunks_[idx] = std::move(Chunk(true));
      }
      for (; idx < full_size; ++idx) {
        vec_of_chunks_[idx] = std::move(Chunk(false));
      }
    } catch (...) {
      for (size_t i = 0; i < idx; ++i) {
        vec_of_chunks_[i].~Chunk();
      }
      throw;
    }
  }
  void db_filling_attempt_l(const TempT& value, const size_t& len) {
    size_t idx = 0;
    try {
      for (; idx < len; ++idx) {
        db_push_front(value);
      }
    } catch (...) {
      for (size_t i = 0; i < idx; ++i) {
        db_pop_front();
      }
      throw;
    }
  }
  void db_filling_attempt_r(const TempT& value, const size_t& len) {
    size_t idx = 0;
    try {
      for (; idx < len; ++idx) {
        db_push_back(value);
      }
    } catch (...) {
      for (size_t i = 0; i < idx; ++i) {
        db_pop_back();
      }
      throw;
    }
  }
  void db_filling_attempt(const TempT& value, const size_t& full_size) {
    const size_t kHalfOfCount = full_size / 2;
    try {
      db_filling_attempt_l(value, kHalfOfCount);
      db_filling_attempt_r(value, full_size - kHalfOfCount);
    } catch (...) {
      for (size_t i = 0; i < full_size; ++i) {
        vec_of_chunks_[i].~Chunk();
      }
      id_finish_ = id_start_ = 0;
      throw;
    }
  }
  //``````````````````````````````````````````
  //```````Functions for pushing units````````
  void db_reallocation_attempt_l(const size_t& new_size) {
    const size_t kOldSize = vec_of_chunks_.size();
    try {
      vec_of_chunks_.resize(new_size);
    } catch (...) {
      throw;
    }
    try {
      for (size_t i = kOldSize; i < vec_of_chunks_.size(); ++i) {
        vec_of_chunks_[i] = std::move(Chunk(true));
      }
    } catch (...) {
      vec_of_chunks_.resize(kOldSize);
      throw;
    }
    const size_t kDelta = new_size - kOldSize;
    for (size_t i = kOldSize - 1; i < new_size; --i) {
      std::swap(vec_of_chunks_[kDelta + i], vec_of_chunks_[i]);
    }
    id_finish_ += kDelta;
    id_start_ += kDelta;
  }
  void db_reallocation_attempt_r(const size_t& new_size) {
    const size_t kOldSize = vec_of_chunks_.size();
    try {
      vec_of_chunks_.resize(new_size);
    } catch (...) {
      throw;
    }
    try {
      for (size_t i = kOldSize; i < vec_of_chunks_.size(); ++i) {
        vec_of_chunks_[i] = std::move(Chunk(false));
      }
    } catch (...) {
      vec_of_chunks_.resize(kOldSize);
      throw;
    }
  }
  //``````````````````````````````````````````
  //******************************************

 public:
  //*************Constructors****************
  DequeBody() = default;

  DequeBody(const size_t& count, const TempT& value = TempT(),
            const Alloc& alloc = std::allocator<TempT>())
      : db_all_(alloc) {
    const size_t kSize = count / set_chunk_rank() + 1;
    const size_t kFullSize = std::max(kSize + 2, kDefaultDequeLenth);
    db_allocation_attempt(kFullSize);
    db_construction_attempt(kFullSize);
    id_start_ = kFullSize / 2 - 1;
    id_finish_ = kFullSize / 2;
    if (count == 1) {
      id_finish_ = id_start_ = 2;
    }
    db_filling_attempt(value, count);
    // using_len = full_size - 2;
  }

  DequeBody(const DequeBody& db_copy)
      : vec_of_chunks_(db_copy.vec_of_chunks_),
        id_start_(db_copy.id_start_),
        id_finish_(db_copy.id_finish_) {}

  DequeBody& operator=(DequeBody&& db_copy) & noexcept {
    std::swap(vec_of_chunks_, db_copy.vec_of_chunks_);
    // db_copy.~DequeBody();
    // db_copy = DequeBody();
    id_finish_ = db_copy.id_finish_;
    id_start_ = db_copy.id_start_;
    db_copy.id_finish_ = db_copy.id_start_ = 0;
    return *this;
  }

  DequeBody& operator=(const DequeBody& db_copy) & {
    // if (db_copy.vec_of_chunks_.empty()) {
    //   return *this;
    // }
    return *this = std::move(DequeBody(db_copy));
  }

  DequeBody(DequeBody&& db_copy)
      : vec_of_chunks_(std::move(db_copy.vec_of_chunks_)),
        id_start_(std::move(db_copy.id_start_)),
        id_finish_(std::move(db_copy.id_finish_)) {
    db_copy = std::move(DequeBody());
  }

  ~DequeBody() noexcept {
    for (size_t i = 0; i < vec_of_chunks_.size(); ++i) {
      vec_of_chunks_[i].~Chunk();
    }
    id_finish_ = id_start_ = 0;  // using_len = 0;
  }
  //******************************************
  //****************Getters*******************
  Chunk& get_head() noexcept { return vec_of_chunks_[id_start_]; }
  const Chunk& get_head() const noexcept { return vec_of_chunks_[id_start_]; }
  Chunk& get_tail() noexcept { return vec_of_chunks_[id_finish_]; }
  const Chunk& get_tail() const noexcept { return vec_of_chunks_[id_finish_]; }
  Chunk& operator[](const size_t& index) noexcept {
    return vec_of_chunks_[id_start_ + index];
  }
  const Chunk& operator[](const size_t& index) const noexcept {
    return vec_of_chunks_[id_start_ + index];
  }
  bool db_body_costructed() const noexcept { return !vec_of_chunks_.empty(); }
  //******************************************
  //***************Unit Changes***************
  void db_push_front(const TempT& value) {
    if (vec_of_chunks_[id_start_].chunk_full()) {
      if (id_start_ == 0) {
        db_reallocation_attempt_l(vec_of_chunks_.size() * 2);
      }
      --id_start_;
    }
    try {
      vec_of_chunks_[id_start_].left_set_chunk(value);
    } catch (...) {
      if (vec_of_chunks_[id_start_].chunk_empty()) {
        ++id_start_;
        // не буду деалоцировать обратно при выкидывании исключения не вижу
        // смысла тк рано или поздно снова придется реалоуировать память
        throw;
      }
    }
  }
  void db_push_front(TempT&& value) {
    if (vec_of_chunks_[id_start_].chunk_full()) {
      if (id_start_ == 0) {
        db_reallocation_attempt_l(vec_of_chunks_.size() * 2);
      }
      --id_start_;
    }
    try {
      vec_of_chunks_[id_start_].left_set_chunk(std::move_if_noexcept(value));
    } catch (...) {
      if (vec_of_chunks_[id_start_].chunk_empty()) {
        ++id_start_;
        // не буду деалоцировать обратно при выкидывании исключения не вижу
        // смысла тк рано или поздно снова придется реалоуировать память
        throw;
      }
    }
  }
  void db_push_back(const TempT& value) {
    if (vec_of_chunks_[id_finish_].chunk_full()) {
      if (id_finish_ == vec_of_chunks_.size() - 1) {
        db_reallocation_attempt_r(vec_of_chunks_.size() * 2);
      }
      ++id_finish_;
    }
    try {
      vec_of_chunks_[id_finish_].right_set_chunk(value);
    } catch (...) {
      if (vec_of_chunks_[id_finish_].chunk_empty()) {
        --id_finish_;
        // не буду деалоцировать обратно при выкидывании исключения не вижу
        // смысла тк рано или поздно снова придется реалоуировать память
        throw;
      }
    }
  }
  void db_push_back(TempT&& value) {
    if (vec_of_chunks_[id_finish_].chunk_full()) {
      if (id_finish_ == vec_of_chunks_.size() - 1) {
        db_reallocation_attempt_r(vec_of_chunks_.size() * 2);
      }
      ++id_finish_;
    }
    try {
      vec_of_chunks_[id_finish_].right_set_chunk(std::move_if_noexcept(value));
    } catch (...) {
      if (vec_of_chunks_[id_finish_].chunk_empty()) {
        --id_finish_;
        // не буду деалоцировать обратно при выкидывании исключения не вижу
        // смысла тк рано или поздно снова придется реалоуировать память
        throw;
      }
    }
  }
  void db_pop_front() noexcept {
    vec_of_chunks_[id_start_].left_pop_chunk();
    if (vec_of_chunks_[id_start_].chunk_empty()) {
      ++id_start_;
    }
  }
  void db_pop_back() noexcept {
    vec_of_chunks_[id_finish_].right_pop_chunk();
    if (vec_of_chunks_[id_finish_].chunk_empty()) {
      --id_finish_;
    }
  }
  //******************************************
};
//===============================================

//====================DEQUE======================
//*****************Constructors******************
template <typename TempT, typename Alloc>
Deque<TempT, Alloc>::Deque() = default;

template <typename TempT, typename Alloc>
Deque<TempT, Alloc>::Deque(const size_t& count, const Alloc& alloc)
    : body_(count, TempT(), alloc) {}

template <typename TempT, typename Alloc>
Deque<TempT, Alloc>::Deque(const size_t& count, const TempT& value,
                           const Alloc& alloc)
    : body_(count, value, alloc) {}

template <typename TempT, typename Alloc>
Deque<TempT, Alloc>::Deque(const Deque<TempT, Alloc>& deq) : body_(deq.body_) {}

template <typename TempT, typename Alloc>
Deque<TempT, Alloc>::Deque(Deque<TempT, Alloc>&& deq)
    : body_(std::move(deq.body_)) {}

template <typename TempT, typename Alloc>
Deque<TempT, Alloc>& Deque<TempT, Alloc>::operator=(
    const Deque<TempT, Alloc>& deq) & {
  body_ = deq.body_;
  return *this;
}

template <typename TempT, typename Alloc>
Deque<TempT, Alloc>& Deque<TempT, Alloc>::operator=(
    Deque<TempT, Alloc>&& deq) & noexcept {
  body_ = std::move(deq.body_);
  deq.body_ = std::move(Deque<TempT, Alloc>().body_);
  return *this;
}

template <typename TempT, typename Alloc>
Deque<TempT, Alloc>::~Deque() noexcept {
  //  body_.~DequeBody();
  // тк вектор сам се почистит
}
//***********************************************

//***************Accessing a element*************
template <typename TempT, typename Alloc>
TempT& Deque<TempT, Alloc>::operator[](const size_t& index) noexcept {
  size_t new_index = index;
  if (body_.get_head().get_chunk_size() > index) {
    return body_.get_head()[index];
  }
  new_index -= body_.get_head().get_chunk_size();
  const size_t kIndexOfChunk = new_index / set_chunk_rank() + 1;
  const size_t kIndexOfUnit = new_index % set_chunk_rank();
  return body_[kIndexOfChunk][kIndexOfUnit];
}
template <typename TempT, typename Alloc>
const TempT& Deque<TempT, Alloc>::operator[](
    const size_t& index) const noexcept {
  size_t new_index = index;
  if (body_.get_head().get_chunk_size() > index) {
    return body_.get_head()[index];
  }
  new_index -= body_.get_head().get_chunk_size();
  const size_t kIndexOfChunk = new_index / set_chunk_rank() + 1;
  const size_t kIndexOfUnit = new_index % set_chunk_rank();
  return body_[kIndexOfChunk][kIndexOfUnit];
}
template <typename TempT, typename Alloc>
TempT& Deque<TempT, Alloc>::at(const size_t& index) {
  if (index >= size()) {
    throw std::out_of_range("you want to access a non-existent element");
  }
  return operator[](index);
}
template <typename TempT, typename Alloc>
const TempT& Deque<TempT, Alloc>::at(const size_t& index) const {
  if (index >= size()) {
    throw std::out_of_range("you want to access a non-existent element");
  }
  return operator[](index);
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

//***************SizeInformation*****************
template <typename TempT, typename Alloc>
size_t Deque<TempT, Alloc>::size() const noexcept {
  const_iterator finish = end();
  const_iterator start = begin();
  size_t res = (finish <= start) ? 0 : static_cast<size_t>(finish - start);
  return res;
}
template <typename TempT, typename Alloc>
bool Deque<TempT, Alloc>::empty() const noexcept {
  return size() == 0;
}
//***********************************************

//*************Modification methods**************
template <typename TempT, typename Alloc>
void Deque<TempT, Alloc>::pop_back() noexcept {
  body_.db_pop_back();
}
template <typename TempT, typename Alloc>
void Deque<TempT, Alloc>::pop_front() noexcept {
  body_.db_pop_front();
}
template <typename TempT, typename Alloc>
void Deque<TempT, Alloc>::push_back(const TempT& elem) {
  if (body_.db_body_costructed()) {
    body_.db_push_back(elem);
  } else {
    *this = std::move(Deque(1, elem));
  }
}
template <typename TempT, typename Alloc>
void Deque<TempT, Alloc>::push_front(const TempT& elem) {
  if (body_.db_body_costructed()) {
    body_.db_push_front(elem);
  } else {
    *this = std::move(Deque(1, elem));
  }
}
template <typename TempT, typename Alloc>
void Deque<TempT, Alloc>::push_back(TempT&& elem) {
  if (body_.db_body_costructed()) {
    body_.db_push_back(std::move_if_noexcept(elem));
  } else {
    *this = std::move(Deque(1, std::move_if_noexcept(elem)));
  }
}
template <typename TempT, typename Alloc>
void Deque<TempT, Alloc>::push_front(TempT&& elem) {
  if (body_.db_body_costructed()) {
    body_.db_push_front(std::move_if_noexcept(elem));
  } else {
    *this = std::move(Deque(1, std::move_if_noexcept(elem)));
  }
}
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::iterator Deque<TempT, Alloc>::insert(
    Deque<TempT, Alloc>::const_iterator pos, const TempT& value) {
  push_back(value);
  // тут не нужен try catch тк внутри пушбека уже все сделано
  const size_t kIndex = pos - begin();
  if (kIndex + 1 == size()) {
    return end() - 1;
  }
  for (size_t i = size() - 1; i > kIndex; ++i) {
    std::swap(operator[](i), operator[](i - 1));
  }
  return begin() + kIndex;
}
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::iterator Deque<TempT, Alloc>::insert(
    Deque<TempT, Alloc>::const_iterator pos, TempT&& value) {
  push_back(std::move_if_noexcept(value));
  // тут не нужен try catch тк внутри пушбека уже все сделано
  const size_t kIndex = pos - begin();
  if (kIndex + 1 == size()) {
    return end() - 1;
  }
  for (size_t i = size() - 1; i > kIndex; --i) {
    std::swap(operator[](i), operator[](i - 1));
  }
  return begin() + kIndex;
}
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::iterator Deque<TempT, Alloc>::erase(
    Deque<TempT, Alloc>::const_iterator pos) {
  const size_t kIndex = pos - begin();
  if (kIndex + 1 == size()) {
    pop_back();
    return end();
  }
  for (size_t i = kIndex + 1; i < size(); ++i) {
    std::swap(operator[](i), operator[](i - 1));
  }
  pop_back();
  return begin() + kIndex + 1;
}
//***********************************************

//***************Iterator methods****************
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::iterator Deque<TempT, Alloc>::begin() noexcept {
  return typename Deque<TempT, Alloc>::iterator(
      &(body_.get_head().get_chunk_head()), &(body_.get_head()));
}
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::const_iterator Deque<TempT, Alloc>::begin()
    const noexcept {
  return typename Deque<TempT, Alloc>::const_iterator(
      const_cast<pointer>(&(body_.get_head().get_chunk_head())),
      const_cast<chunk_pointer>(&(body_.get_head())));
}
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::const_iterator Deque<TempT, Alloc>::cbegin()
    const noexcept {
  return typename Deque<TempT, Alloc>::const_iterator(
      const_cast<pointer>(&(body_.get_head().get_chunk_head())),
      const_cast<chunk_pointer>(&(body_.get_head())));
}
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::iterator Deque<TempT, Alloc>::end() noexcept {
  return Deque<TempT, Alloc>::iterator(&body_.get_tail().get_chunk_tail(),
                                       &body_.get_tail()) +
         1;
}
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::const_iterator Deque<TempT, Alloc>::end()
    const noexcept {
  return typename Deque<TempT, Alloc>::const_iterator(
             const_cast<pointer>(&(body_.get_tail().get_chunk_tail())),
             const_cast<chunk_pointer>(&(body_.get_tail()))) +
         1;
}
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::const_iterator Deque<TempT, Alloc>::cend()
    const noexcept {
  return typename Deque<TempT, Alloc>::const_iterator(
             const_cast<pointer>(&(body_.get_tail().get_chunk_tail())),
             const_cast<chunk_pointer>(&(body_.get_tail()))) +
         1;
}
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::reverse_iterator
Deque<TempT, Alloc>::rbegin() noexcept {
  typename Deque<TempT, Alloc>::iterator iter = end() - 1;
  return typename Deque<TempT, Alloc>::reverse_iterator(
      std::make_reverse_iterator(iter));
}
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::const_reverse_iterator
Deque<TempT, Alloc>::rbegin() const noexcept {
  typename Deque<TempT, Alloc>::const_iterator iter = end() - 1;
  return typename Deque<TempT, Alloc>::const_reverse_iterator(
      std::make_reverse_iterator(iter));
}
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::const_reverse_iterator
Deque<TempT, Alloc>::crbegin() const noexcept {
  return typename Deque<TempT, Alloc>::const_reverse_iterator(
      std::make_reverse_iterator(cend() - 1));
}
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::reverse_iterator
Deque<TempT, Alloc>::rend() noexcept {
  return typename Deque<TempT, Alloc>::reverse_iterator(
      std::make_reverse_iterator(begin() - 1));
}
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::const_reverse_iterator Deque<TempT, Alloc>::rend()
    const noexcept {
  return typename Deque<TempT, Alloc>::const_reverse_iterator(
      std::make_reverse_iterator(begin() - 1));
}
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::const_reverse_iterator
Deque<TempT, Alloc>::crend() const noexcept {
  return typename Deque<TempT, Alloc>::const_reverse_iterator(
      std::make_reverse_iterator(cbegin() - 1));
}
//***********************************************

//===============================================
