/**
 * @file deque.hpp
 * @author getylman
 * @date 27.02.2023
 */
#pragma once
#include <memory>
#include <type_traits>


template <typename T, typename Alloc = std::allocator<T>>
class Deque {
 public:
  //=================Constructors=================
  Deque();
  Deque(const size_t& count);
  Deque(const size_t& count, const T& value);
  Deque(const Deque<T, Alloc>& deq);
  ~Deque();
  Deque<T, Alloc>& operator=(cosnt Deque<T, Alloc>& deq);
  //==============================================
  //=============Accessing an element=============
  T& operator[](const size_t& index);
  const T& operator[](const size_t& index) const;
  T& at(const size_t& index);
  const T& at(const size_t& index) const;
  T& front();
  const T& front() const;
  T& back();
  const T& back() const;
  //==============================================
  //==============SizeInformation=================
  size_t size() const { return total_size_; }
  bool empty() const { return size() == 0; }
  //==============================================
  //===========Modification methods===============
  void push_back(const T& elem);
  void push_back(T&& elem);
  void pop_back();
  void push_front(const T& elem);
  void push_front(T&& elem);
  void pop_front();
  //==============================================
  //==================ITERATOR====================
  template <bool IsConst>
  struct common_iterator {
   private:
    using conditional_ptr = std::conditional_t<IsConst, const T*, T*>;
    using conditional_ref = std::conditional_t<IsConst, const T&, T&>;
    conditional_ptr ptr;
   public:
    //****************Memory operators************
    conditional_ref operator*() { return *ptr; }
    conditional_ptr operator->() { return ptr; }
    template <typename T1>
    T1& operator->*(T1 T::*another_ptr) { return (*ptr).*another_ptr; }
    //********************************************
    //************Aithmetic operators*************
    common_iterator<IsConst>& operator+=(const int64_t& delta);
    common_iterator<IsConst>& operator-=(const int64_t& delta);
    common_iterator<IsConst> operator+(const int64_t& delta);
    common_iterator<IsConst> operator-(const int64_t& delta);
    common_iterator<IsConst>& operator++();
    common_iterator<IsConst>& operator--();
    common_iterator<IsConst>& operator++(int);
    common_iterator<IsConst>& operator--(int);
    //********************************************
    //**************Compare operators*************
    // bool operator<()
    //********************************************
  };
  using iterator = common_iterator<false>;
  using const_iterator = common_iterator<true>;
  //==============================================
 private:
  const uint64_t kChunkSize = 0; // size of chunks
  uint64_t total_size_ = 0; // size of in full container
  uint64_t fc_size_ = 0; // first chunk size
  uint64_t lc_size_ = 0; // last chunk size
};
//using Deque = std::deque<T>;



//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//               DECLARATION

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^