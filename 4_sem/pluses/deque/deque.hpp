/**
 * @file deque.hpp
 * @author getylman
 * @date 02.03.2023
 */
#pragma once
#include <iterator>
#include <memory>
#include <type_traits>

template <typename TempT, typename Alloc = std::allocator<TempT>>
class Deque {
 protected:
  using allocator_type = Alloc;
  using Alloc_traits = std::allocator_traits<allocator_type>;
  using Pntr = Alloc_traits::pointer;
  using Pntr_Const = Alloc_traits::const_pointer;
  using value_type = TempT;
  using pointer = Alloc_traits::pointer;
  using const_pointer = Alloc_traits::const_pointer;
  using reference = Alloc_traits::reference;
  using const_reference = Alloc_traits::const_reference;
  using difference_type = ptrdiff_t;

 private:
  //====================Chunk=====================
  class Chunk {
   private:

   public:
    const uint64_t chunk_rank_;
    uint64_t size_;
  };
  //==============================================
 public:
  //=================Constructors=================
  Deque();  // check https://habr.com/ru/post/505632/
  Deque(const size_t& count);
  Deque(const size_t& count, const TempT& value);
  Deque(const Deque<T, Alloc>& deq);
  ~Deque();
  Deque<TempT, Alloc>& operator=(cosnt Deque<TempT, Alloc>& deq);
  //==============================================
  //=============Accessing an element=============
  TempT& operator[](const size_t& index);
  const TempT& operator[](const size_t& index) const;
  TempT& at(const size_t& index);
  const TempT& at(const size_t& index) const;
  TempT& front();
  const TempT& front() const;
  TempT& back();
  const TempT& back() const;
  //==============================================
  //==============SizeInformation=================
  size_t size() const {
    return total_size_;
  }
  bool empty() const {
    return size() == 0;
  }
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
  template <bool IsConst>
  struct common_iterator {
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
    conditional_ptr ptr;
    // ещё надо хранить указатель к каком именно чанке находимся
    //********************************************
   public:
    //****************Memory operators************
    conditional_ref operator*() const noexcept {
      return *ptr;
    }
    conditional_ptr operator->() const noexcept {
      return ptr;
    }
    template <typename TempT1>
    TempT1& operator->*(TempT1 TempT::*another_ptr) const noexcept {
      return (*ptr).*another_ptr;
    }
    conditional_ref operator[](const int64_t& idx) const noexcept {
      return *(ptr + idx);
    }
    //********************************************
    //************Aithmetic operators*************
    common_iterator<IsConst>& operator+=(const int64_t& delta) noexcept;
    common_iterator<IsConst>& operator-=(const int64_t& delta) noexcept;
    common_iterator<IsConst> operator+(const int64_t& delta) noexcept;
    common_iterator<IsConst> operator-(const int64_t& delta) noexcept;
    common_iterator<IsConst>& operator++() noexcept;
    common_iterator<IsConst>& operator--() noexcept;
    common_iterator<IsConst>& operator++(int) noexcept;
    common_iterator<IsConst>& operator--(int) noexcept;
    //********************************************
    //**************Compare operators*************
    friend bool operator<(
        const typename Deque<TempT, Alloc>::common_iterator<IsConst>& iter_left,
        const typename Deque<TempT, Alloc>::common_iterator<IsConst>&
            iter_right) noexcept {
      return iter_left.ptr < iter_right.ptr;
      // тут надо проверять находимся ли мы в одном чанке или нет
    }
    //********************************************
  };
  //**************Iterator usings*****************
  using iterator = common_iterator<false>;
  using const_iterator = common_iterator<true>;
  template <bool IsConst>
  using common_reverse_iterator =
      std::reverse_iterator<common_iterator<IsConst>>;
  using reverse_iterator = common_reverse_iterator<true>;
  using const_reverse_iterator = common_reverse_iterator<false>;
  //**********************************************
  //==============================================
 private:
  const uint64_t kChunkSize = 0;  // size of chunks
  uint64_t total_size_ = 0;       // size of in full container
  uint64_t fc_size_ = 0;          // first chunk size
  uint64_t lc_size_ = 0;          // last chunk size
};

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//           NO MEMBER DEFINITION
//=================ITERATOR==================
//***********Arithmetic operators************
template <typename TempT, typename Alloc = std::allocator<TempT>, bool IsConst>
typename Deque<TempT, Alloc>::common_iterator<IsConst>::difference_type
operator-(const typename Deque<TempT, Alloc>::common_iterator<IsConst>& it1,
          const typename Deque<TempT, Alloc>::common_iterator<IsConst>& it2);
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
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//               DECLARATION

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^