#pragma once

#include <iterator>
#include <memory>
#include <type_traits>

template <typename TempT, typename Alloc = std::allocator<TempT>>
class List {
  //===============ListBody=================
  struct ListBody;
  //========================================
  //==============List Fields===============
  ListBody list_body_;
  //========================================
 public:
  //===============Iterator=================
  template <bool IsConst>
  class common_iterator;
  //========================================
  //================Usings==================
  using value_type = TempT;
  using allocator_type = Alloc;
  using iterator = common_iterator<false>;
  using const_iterator = common_iterator<true>;
  //========================================
  //==============Constructors==============
  List();
  List(const size_t& count, const TempT& value = TempT(), const Alloc& alloc = Alloc());
  explicit List(const size_t& count, const Alloc& alloc = Alloc());
  List(const List& other);
  List(List&& other);
  List(std::initializer_list<TempT> init, const Alloc& alloc = std::allocator<TempT>());
  ~List();
  List<TempT, Alloc>& operator=(const List<TempT, Alloc>& other) &;
  List<TempT, Alloc>& operator=(List<TempT, Alloc>&& other) & noexcept;
  //========================================
  //========Element access methods==========
  TempT& front() noexcept;
  const TempT& front() const noexcept;
  TempT& back() noexcept;
  const TempT& back() const noexcept;
  //========================================
  //================Capacity================
  size_t size() const noexcept;
  bool empty() const noexcept;
  //========================================
  //================Modifiers===============
  void pop_front() noexcept;
  void pop_back() noexcept;
  void push_back(const TempT& value);
  void push_back(TempT&& value);
  void push_front(const TempT& value);
  void push_front(TempT&& value);
  //========================================
};