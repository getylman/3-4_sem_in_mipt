#pragma once

#include <iterator>
#include <memory>
#include <type_traits>

template <typename TempT, typename Alloc = std::allocator<TempT>>
class List {
 public:
  //=================Usings=================
  using value_type = TempT;
  using allocator_type = Alloc;
  using pointer = TempT*;
  using const_pointer = const TempT*;
  using reference = TempT&;
  using const_reference = const TempT&;
  using difference_type = ptrdiff_t;
  //========================================

 private:
  //=================Node===================
  struct Node;
  //========================================
  //===============ListBody=================
  struct ListBody;
  //========================================
  //==============List Fields===============
  ListBody list_body_;
  //========================================
  //============Private usings==============
  using type_alloc_type = typename std::allocator_traits<
      allocator_type>::template rebind<value_type>::other;
  using type_alloc_traits = std::allocator_traits<type_alloc_type>;
  using node_alloc_type =
      typename type_alloc_traits::template rebind<Node>::other;
  using node_alloc_traits = std::allocator_traits<node_alloc_type>;
  //========================================

 public:
  //===============Iterator=================
  template <bool IsConst>
  class common_iterator;
  //========================================
  //=================Usings=================
  using iterator = common_iterator<false>;
  using const_iterator = common_iterator<true>;
  //========================================
  //==============Constructors==============
  List();
  List(const size_t& count, const TempT& value, const Alloc& alloc = Alloc());
  explicit List(const size_t& count, const Alloc& alloc = Alloc());
  List(const List& other);
  List(List&& other);
  List(std::initializer_list<TempT> init,
       const Alloc& alloc = std::allocator<TempT>());
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

//=================Node=====================
template <typename TempT, typename Alloc>
struct List<TempT, Alloc>::Node {
 private:
  //************Private Getters*************
  Node* get_next_neighbour() const noexcept { return next_node_; }
  Node* get_prev_neighbour() const noexcept { return prev_node_; }
  TempT& get_body() const noexcept { return *node_body_; }
  //****************************************
  //********Private Memory Functions********
  void node_construct_attempt(const TempT& value) {
    try {
      type_alloc_traits::construct(type_alloc_, node_body_, value);
    } catch (...) {
      type_alloc_traits::deallocate(type_alloc_, 1);
      throw;
    }
  }
  void node_construct_attempt(TempT&& value) {
    try {
      type_alloc_traits::construct(type_alloc_, node_body_,
                                   std::move_if_noexcept(value));
    } catch (...) {
      type_alloc_traits::deallocate(type_alloc_, 1);
      throw;
    }
  }
  void node_suicide() noexcept {
    if (node_body_ != nullptr) {
      type_alloc_traits::destroy(type_alloc_, node_body_);
      type_alloc_traits::deallocate(type_alloc_, node_body_, 1);
    }
  }
  void node_copy_neighbours(const Node& other) noexcept {
    next_node_ = other.get_next_neighbour();
    prev_node_ = other.get_prev_neighbour();
  }
  void node_copy_neighbours(Node&& other) noexcept {
    next_node_ = other.get_next_neighbour();
    prev_node_ = other.get_prev_neighbour();
  }
  //****************************************
 public:
  //**************Constructors**************
  Node() = default;
  Node(const TempT& value)
      : node_body_(type_alloc_traits::allocate(type_alloc_, 1)) {
    // if throws exception in allocation it will throwen further
    node_construct_attempt(value);
  }
  Node(TempT&& value)
      : node_body_(type_alloc_traits::allocate(type_alloc_, 1)) {
    // if throws exception in allocation it will throwen further
    node_construct_attempt(std::move_if_noexcept(value));
  }
  ~Node() { node_suicide(); }
  Node(const Node& other)
      : node_body_(type_alloc_traits::allocate(type_alloc_, 1)) {
    node_construct_attempt(other.get_body());
    node_copy_neighbours(other);
  }
  Node(Node&& other) : node_body_(type_alloc_traits::allocate(type_alloc_, 1)) {
    node_construct_attempt(std::move(other.get_body()));
    node_copy_neighbours(other);
  }
  Node& operator=(Node&& other) & noexcept {
    if (*this == other) {
      node_suicide();
      node_body_ = &other.get_body();
      node_copy_neighbours(std::move(other));
      other.next_node_ = other.prev_node_ = other.node_body_ = nullptr;
    }
    return *this;
  }
  Node& operator=(const Node& other) & {
    if (other != *this) {
      *node_body_ = other.get_body();
      node_copy_neighbours(other);
    }
    return *this;
  }
  //****************************************
  //**************Comparation***************
  friend bool operator==(const Node& left, const Node& right) noexcept {
    return left.get_body() == right.get_body();
  }
  friend bool operator!=(const Node& left, const Node& right) noexcept {
    return left.get_body() != right.get_body();
  }
  //****************************************
  //****************Setters*****************
  void node_set_next_neighbour(Node* node) noexcept {
    if (node->prev_node_->next_node_ != nullptr) {
      node->prev_node_->next_node_ = nullptr;
      // untie the old next node from previous node of the node
    }
    if (next_node_->prev_node_ != nullptr) {
      next_node_->prev_node_ = nullptr;
      // untie the old previous node from next node of this
    }
    next_node_ = node;
    node->prev_node_ = &(*this);
  }
  void node_set_prev_neighbour(Node* node) noexcept {
    if (node->next_node_->prev_node_ != nullptr) {
      node->next_node_->prev_node_ = nullptr;
      // untie the old previous node from next node of the node
    }
    if (prev_node_->next_node_ != nullptr) {
      prev_node_->next_node_ = nullptr;
      // untie the old next node from previous node of this
    }
    prev_node_ = node;
    node->next_node_ = &(*this);
  }
  //****************************************
 private:
  Node* prev_node_ = nullptr;
  Node* next_node_ = nullptr;
  pointer node_body_ = nullptr;
  allocator_type type_alloc_;
};
//==========================================