/**
 * @file deque.hpp
 * @author getylman
 * @date 27.02.2023
 */
#pragma once
#include <memory>
#include <type_traits>


template <typename TempT, typename Alloc = std::allocator<TempT>>
class Deque {
 public:
  //=================Constructors=================
  Deque(); // check https://habr.com/ru/post/505632/
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
  size_t size() const { return total_size_; }
  bool empty() const { return size() == 0; }
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
   private:
    using conditional_ptr = std::conditional_t<IsConst, const TempT*, TempT*>;
    using conditional_ref = std::conditional_t<IsConst, const TempT&, TempT&>;
    conditional_ptr ptr;
   public:
    //****************Memory operators************
    conditional_ref operator*() { return *ptr; }
    conditional_ptr operator->() { return ptr; }
    template <typename TempT1>
    TempT1& operator->*(TempT1 TempT::*another_ptr) { return (*ptr).*another_ptr; }
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
    //*****************Usings*********************
    using iterator_category = std::random_access_iterator_tag;
    using value_type = 
    //********************************************
  };
  using iterator = common_iterator<false>;
  using const_iterator = common_iterator<true>;
  template <typename Iterator>
  class common_reverse_iterator {
    Iterator iter;
   public:
    common_reverse_iterator<Iterator>& operator++();
    // есть у меня в конспекте
    // common_reverse_iterator<>
    // мб скип тк написано можно использовать стандартный
  };
  //==============================================
 private:
  const uint64_t kChunkSize = 0; // size of chunks
  uint64_t total_size_ = 0; // size of in full container
  uint64_t fc_size_ = 0; // first chunk size
  uint64_t lc_size_ = 0; // last chunk size
};

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//               DECLARATION

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^