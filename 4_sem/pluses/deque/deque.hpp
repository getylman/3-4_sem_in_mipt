/**
 * @file deque.hpp
 * @author getylman
 * @date 17.03.2023
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
  const uint64_t kDefaultDequeLenth = 4;
  // --------------------------------
  // reason why default 4
  // # - using space, * - free space
  // ******
  // ****##
  // ##****
  // ******
  // --------------------------------
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
  //===============Deque Body=====================
  struct DequeBody;
  // struct to keep all Deque fields
  //==============================================

 protected:
  using Chunk_alloc_type =
      typename Alloc_traits::template rebind<Chunk<>>::other;
  // allocator type of chunk
  using Chunk_alloc_traits = std::allocator_traits<Chunk_alloc_type>;
  // allocator trits for chunk
  using Chunk_pointer = Chunk_alloc_traits::pointer;
  // pointer for chunks
  Chunk_alloc_type all_ch_;
  // object of chunk allocator type
 private:
  //============Deque Memory Controllere==========
  struct MemCntAndBodyOfDeque;
  // struct which one can use memory resources
  Type_alloc_type& get_unit_allocator() noexcept;
  const Type_alloc_type& get_unit_allocator() const noexcept;
  Chunk_alloc_type get_chunk_allocator() const noexcept;
  pointer chunk_allocation();
  void chunk_dealocation(pointer ptr) noexcept;
  Chunk_pointer body_allocation(const uint64_t& size);
  void body_dealocation(Chunk_pointer ptr, const uint64_t& size) noexcept;
  void reserve_memory_in_deque(const uint64_t& num_of_units);
  // function for allocation memory for vector of chunks
  // kinda it will reserve enought chunks to keeping our units
  void body_range_construction(Chunk_pointer head, Chunk_pointer tail);
  void body_range_destruction(Chunk_pointer head, Chunk_pointer tail) noexcept;
  //==============================================

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
  //================Deque Fields=================
  // DequeBody body_;  // full body of our container
  MemCntAndBodyOfDeque mc_body_;
  //=============================================
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
  using difference_type = int64_t;  // ptrdiff_t;
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
  Chunk_pointer cur_chunk_ptr_ = nullptr;
  // pointer of current chunk

  //********************************************
 public:
  //****************Constructors****************
  common_iterator() noexcept {
  }
  common_iterator(const pointer& ptr, const Chunk_pointer& curnk_ptr) noexcept
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
  common_iterator<IsConst>& operator=(
      const common_iterator<IsConstTmp>& iter) & noexcept {
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
  Type_alloc_type all_tp_;  // object of our allocator
  //**************Private Functions************
  void allocation_attempt_of_chunk(pointer& ptr) {
    try {
      ptr = Alloc_traits::allocate(all_tp_, ChunkRank);
    } catch (...) {  // if allocation of memory failed it will stop
      throw;         // constructing and throw the exception of this situation
    }
  }  // tring to allocate memeory for chunk
  void coping_chunk_inside(Chunk<>& src, Chunk<>& dst) noexcept {
    // src -> source   dst -> destination
    dst.chunk_head_ = dst.chunk_body_ + (src.chunk_head_ - src.chunk_body_);
    dst.chunk_tail_ = dst.chunk_body_ + (src.chunk_tail_ - src.chunk_body_);
    dst.r_chunk_confine_ =
        dst.chunk_body_ + (src.r_chunk_confine_ - src.chunk_body_);
    dst.l_chunk_confine_ =
        dst.chunk_body_ + (src.l_chunk_confine_ - src.chunk_body_);
    dst.chunk_size_ = src.chunk_size_;
  }  // function for copy chunk fields
  void copy_attempt_of_chunk(Chunk<>& src, Chunk<>& dst) {
    // src -> source   dst -> destination
    uint64_t idx = static_cast<uint64_t>(src.chunk_head_ - src.chunk_body_);
    try {
      for (; idx <= static_cast<uint64_t>(src.chunk_tail_ - src.chunk_body_);
           ++idx) {
        Alloc_traits::construct(all_tp_, dst.chunk_body_ + idx,
                                src.chunk_body_ + idx);
      }
    } catch (...) {
      for (uint64_t i =
               static_cast<uint64_t>(src.chunk_head_ - src.chunk_body_);
           i < idx; ++i) {
        Alloc_traits::destroy(all_tp_, dst.chunk_body_ + i);
      }
      Alloc_traits::deallocate(all_tp_, dst.chunk_body_, ChunkRank);
      throw;
    }
  }  // tring to copy from src to dst chunk
  void construct_unit_attempt_of_chunk(
      pointer& ptr, TempT&& value,
      uint16_t&
          changing_delta) noexcept(std::is_nothrow_constructible_v<TempT>) {
    if (chunk_size_ != 0) {
      ptr += changing_delta;
    }
    try {
      Alloc_traits::construct(all_tp_, ptr, std::forward<TempT>(value));
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
    r_chunk_confine_ = chunk_body_ + ChunkRank - 1;
    chunk_tail_ = chunk_head_ = (is_head) ? r_chunk_confine_ : l_chunk_confine_;
    // I hope that you remember the construction which one you used
  }
  ~Chunk() {
    for (uint64_t i = static_cast<uint64_t>(chunk_head_ - chunk_body_);
         i <= static_cast<uint64_t>(chunk_tail_ - chunk_body_); ++i) {
      Alloc_traits::destroy(all_tp_, chunk_body_ + i);
    }
    Alloc_traits::deallocate(all_tp_, chunk_body_, ChunkRank);
    chunk_body_ = chunk_head_ = chunk_tail_ = r_chunk_confine_ =
        l_chunk_confine_ = nullptr;
    chunk_size_ = 0;
  }
  template <typename AnotherAlloc>
  Chunk(const typename Deque<TempT, AnotherAlloc>::Chunk<>& chnk) {
    if (chnk.chunk_body_ != nullptr) {
      allocation_attempt_of_chunk(chunk_body_);
      copy_attempt_of_chunk(*this, chnk);
      chunk_size_ = chnk.chunk_size_;
      chunk_head_ = chunk_body_ + (chnk.chunk_head_ - chnk.chunk_body_);
      chunk_tail_ = chunk_body_ + (chnk.chunk_tail_ - chnk.chunk_body_);
      r_chunk_confine_ = (l_chunk_confine_ = chunk_body_) + ChunkRank - 1;
    }
  }
  Chunk(Chunk<>&& chnk) noexcept
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
  template <typename AnotherAlloc>
  Chunk<>& operator=(
      const typename Deque<TempT, AnotherAlloc>::Chunk<>& chnk) & {
    if (chunk_body_ != chnk.chunk_body_) {
      Chunk<> tmp_chunk;
      allocation_attempt_of_chunk(tmp_chunk.chunk_body_);
      copy_attempt_of_chunk(tmp_chunk, chnk);
      for (uint64_t i = static_cast<uint64_t>(chunk_head_ - chunk_head_);
           i <= static_cast<uint64_t>(chunk_tail_ - chunk_body_); ++i) {
        Alloc_traits::destroy(all_tp_, chunk_body_ + i);
      }
      Alloc_traits::deallocate(all_tp_, chunk_body_, ChunkRank);
      chunk_body_ = tmp_chunk.chunk_body_;
      coping_chunk_inside(chnk, *this);
    }
    return *this;
  }
  Chunk<>& operator=(Chunk<>&& chnk) & noexcept {
    if (chunk_body_ != chnk.chunk_body_) {
      ~Chunk();
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
  bool chunk_empty() const noexcept {
    return get_chunk_size() == 0;
  }
  bool chunk_full() const noexcept {
    return chunk_size_ == ChunkRank;
  }
  constexpr const uint64_t get_chunk_rank() const noexcept {
    return ChunkRank;
  }
  //*******************************************
  //*****************Setters*******************
  void left_set_chunk(const value_type& value) noexcept(
      std::is_nothrow_constructible_v<TempT>) {
    construct_unit_attempt_of_chunk(chunk_head_, value, l_changing_delta_);
  }
  void right_set_chunk(const value_type& value) noexcept(
      std::is_nothrow_constructible_v<TempT>) {
    construct_unit_attempt_of_chunk(
        chunk_tail_, value,
        r_changing_delta_);  // подумать над move_if_noexxcept и как тут это
                             // заюзать
  }
  void left_set_chunk(value_type&& value) noexcept(
      std::is_nothrow_constructible_v<TempT>) {
    construct_unit_attempt_of_chunk(chunk_head_, value, l_changing_delta_);
    // что там после перемещения останется неопределенное значени то мне без
    // разницы
  }
  void right_set_chunk(value_type&& value) noexcept(
      std::is_nothrow_constructible_v<TempT>) {
    construct_unit_attempt_of_chunk(chunk_tail_, value, r_changing_delta_);
    // что там после перемещения останется неопределенное значени то мне без
    // разницы
  }
  //*******************************************
  //*****************Erasers*******************
  void left_pop_chunk() noexcept {
    Alloc_traits::destroy(all_tp_, chunk_head_);
    // if chunk was empty it will be UB
    --chunk_size_;
    if (chunk_size_ != 0) {
      chunk_head_ -= l_changing_delta_;
    }
  }
  void right_pop_chunk() noexcept {
    Alloc_traits::destroy(all_tp_, chunk_tail_);
    // if chunk was empty it will be UB
    --chunk_size_;
    if (chunk_size_ != 0) {
      chunk_tail_ -= r_changing_delta_;
    }
  }
  //*******************************************
};
//===========================================

//==========Deque Memory Controllere=========
template <typename TempT, typename Alloc>
struct Deque<TempT, Alloc>::MemCntAndBodyOfDeque : public Type_alloc_type,
                                                   public DequeBody {
  MemCntAndBodyOfDeque() noexcept(
      std::is_nothrow_default_constructible_v<Type_alloc_type>)
      : Type_alloc_type() {
  }
  MemCntAndBodyOfDeque(const Type_alloc_type& allc) noexcept
      : Type_alloc_type(allc) {
  }
  MemCntAndBodyOfDeque(Type_alloc_type&& allc) noexcept
      : Type_alloc_type(std::move(allc)) {
  }
  MemCntAndBodyOfDeque(MemCntAndBodyOfDeque&&) = default;
  MemCntAndBodyOfDeque(MemCntAndBodyOfDeque&& mcr, Type_alloc_type&& allc)
      : Type_alloc_type(std::move(allc)), DequeBody(std::move(mcr)) {
  }
};
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::Type_alloc_type&
Deque<TempT, Alloc>::get_unit_allocator() noexcept {
  return mc_body_;
}
template <typename TempT, typename Alloc>
const typename Deque<TempT, Alloc>::Type_alloc_type&
Deque<TempT, Alloc>::get_unit_allocator() const noexcept {
  return mc_body_;
}
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::Chunk_alloc_type
Deque<TempT, Alloc>::get_chunk_allocator() const noexcept {
  return Chunk_alloc_type(get_unit_allocator());
}
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::pointer Deque<TempT, Alloc>::chunk_allocation() {
  return Alloc_traits::allocate(mc_body_, set_chunk_rank());
}
template <typename TempT, typename Alloc>
void Deque<TempT, Alloc>::chunk_dealocation(pointer ptr) noexcept {
  Alloc_traits::dealocate(mc_body_, ptr, set_chunk_rank());
}
template <typename TempT, typename Alloc>
typename Deque<TempT, Alloc>::Chunk_pointer
Deque<TempT, Alloc>::body_allocation(const uint64_t& size) {
  Chunk_alloc_type chunk_allocator = get_chunk_allocator();
  return Chunk_alloc_traits::allocate(chunk_allocator, size);
}
template <typename TempT, typename Alloc>
void Deque<TempT, Alloc>::body_dealocation(Chunk_pointer ptr,
                                           const uint64_t& size) noexcept {
  Chunk_alloc_type chunk_allocator = get_chunk_allocator();
  Chunk_alloc_traits::dealocate(chunk_allocation, ptr, size);
  ptr = nullptr;
}
template <typename TempT, typename Alloc>  // ?
void Deque<TempT, Alloc>::body_range_construction(Chunk_pointer head,
                                                  Chunk_pointer tail) {
  Chunk_pointer cur_ptr = nullptr;
  try {
    for (cur_ptr = head; cur_ptr <= tail; ++cur_ptr) {
      *cur_ptr = chunk_allocation();
    }
  } catch (...) {
    for (Chunk_pointer id_ptr = head; id_ptr < cur_ptr; ++id_ptr) {
      chunk_dealocation(*id_ptr);
    }
    throw;
  }
}
template <typename TempT, typename Alloc>  // ?
void Deque<TempT, Alloc>::body_range_destruction(Chunk_pointer head,
                                                 Chunk_pointer tail) noexcept {
  for (Chunk_pointer cur_ptr = head; cur_ptr <= tail; ++cur_ptr) {
    chunk_dealocation(*cur_ptr);
  }
}
template <typename TempT, typename Alloc>
void Deque<TempT, Alloc>::reserve_memory_in_deque(
    const uint64_t& num_of_units) {
  const uint64_t required_amount_of_chunks =
      (num_of_units + set_chunk_rank() - 1) / set_chunk_rank();
  mc_body_.total_size = num_of_units;
  mc_body_.num_of_chunks = std::max(
      kDefaultDequeLenth, static_cast<uint64_t>(required_amount_of_chunks + 2));
  try {
    mc_body_.body = body_allocation(mc_body_.num_of_chunks);
  } catch (...) {
    throw;
    // did not allocate memory for body of deque but have not any meaning
  }

}

//===========================================

//==============Deque Body===================
template <typename TempT, typename Alloc>
struct Deque<TempT, Alloc>::DequeBody {
  uint64_t total_size = 0;             // size of in full container
  uint64_t num_of_chunks = 0;          // how many chunks have a container
  Chunk_pointer body = nullptr;        // pointer of full deque. the end is just
                                       // body plus size
  Chunk_pointer head_chunk = nullptr;  // pointer of current head
  Chunk_pointer tail_chunk = nullptr;  // pointer of current tail

  DequeBody() noexcept : body(), head_chunk(), tail_chunk() {
  }
  DequeBody(const DequeBody&) = default;
  DequeBody& operator=(const DequeBody&) = default;
  DequeBody(DequeBody&& dqb) noexcept : DequeBody(dqb) {
    dqb = DequeBody();
  }
  void exchange_data(DequeBody& dqb) noexcept {
    std::swap(*this, dqb);
  }
};
//===========================================

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//               DECLARATION
// инсерт и ерейс я буду делать через пуш и фронт полсе в нужное место через
// последовательные свапы перенесу

//==============Private Functions============

// --- template <typename TempT, typename Alloc>
// --- void Deque<TempT, Alloc>::reserve_memory_in_deque(
// ---     const uint64_t& num_of_units) {
// ---   const uint64_t required_amount_of_chunks =
// ---       (num_of_units + set_chunk_rank() - 1) / set_chunk_rank();
// ---   try {
// ---     body_.body_ =
// ---         Chunk_alloc_traits::allocate(all_ch_, required_amount_of_chunks);
// ---   } catch (...) {
// ---     throw;
// ---   }
// ---   // рассмотреть случай когда число чанков 1 и остальные условия
// ---   // в случае когда один чанк надо стянуть всё в середину а
// ---   // в остальных случаях крайние стянуть равномерно
// ---   // каждый чанк вызываем конструктор или заполняем пушами
// ---   body_.num_of_chunks_ = required_amount_of_chunks;          // ?
// ---   total_size_ = num_of_chunks_;                              // ?
// ---   tail_chunk_ = (head_chunk_ = body_) + num_of_chunks_ - 1;  // ?
// ---   // ? -> хз нужна ли эта страка тут или можно закинуть внутрь
// конструктора
// --- }
//===========================================
//================Constructors===============
template <typename TempT, typename Alloc>
Deque<TempT, Alloc>::Deque() = default;

template <typename TempT, typename Alloc>
Deque<TempT, Alloc>::Deque(const size_t& count, const Alloc& alloc) {
}
//===========================================

// добавить функцию для увелечения памяти под чанки при переполнения
// аллоцированной части
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^