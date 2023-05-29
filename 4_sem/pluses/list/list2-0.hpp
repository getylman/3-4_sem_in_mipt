#pragma once

#include <cstddef>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>

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
  //===============Iterator=================
  template <bool IsConst>
  class common_iterator;
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
      allocator_type>::template rebind_alloc<value_type>;
  using type_alloc_traits = std::allocator_traits<type_alloc_type>;
  using node_alloc_type =
      typename type_alloc_traits::template rebind_alloc<Node>;
  using node_alloc_traits = std::allocator_traits<node_alloc_type>;
  //========================================

 public:
  //=================Usings=================
  using iterator = common_iterator<false>;
  using const_iterator = common_iterator<true>;
  using reverse_iterator = std::reverse_iterator<common_iterator<false>>;
  using const_reverse_iterator = std::reverse_iterator<common_iterator<true>>;
  //========================================
  //=============Alloc Getters==============
  Alloc get_allocator() noexcept;
  Alloc get_allocator() const noexcept;
  //========================================
  //==============Constructors==============
  List();
  List(const size_t& count, const TempT& value, const Alloc& alloc = Alloc());
  explicit List(const size_t& count, const Alloc& alloc = Alloc());
  List(const List& other);
  List(List&& other);
  List(std::initializer_list<TempT> init, const Alloc& alloc = Alloc());
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
  //===========Iterator Methods=============
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
  //========================================
  friend struct Node;
  friend struct ListBody;
};

//=================Node=====================
template <typename TempT, typename Alloc>
struct List<TempT, Alloc>::Node {
  //************Private Getters*************
  const Node* get_next_neighbour() const noexcept { return next_node_; }
  const Node* get_prev_neighbour() const noexcept { return prev_node_; }
  const TempT& get_body() const noexcept { return *node_body_; }
  Node* get_next_neighbour() noexcept { return next_node_; }
  Node* get_prev_neighbour() noexcept { return prev_node_; }
  TempT& get_body() noexcept { return *node_body_; }
  //****************************************
 private:
  //**********Allocator functions***********
  const type_alloc_type& node_get_allocator() const noexcept;
  type_alloc_type& node_get_allocator() noexcept { return type_alloc_; }
  void node_copy_neighbours(const Node& other) noexcept;
  void node_copy_neighbours(Node&& other) noexcept;
  //****************************************
 public:
  //**************Constructors**************
  Node() = default;
  //****************Setters*****************
  void node_set_next_neighbour(Node* node) noexcept;
  void node_set_prev_neighbour(Node* node) noexcept;
  //****************************************
 private:
  Node* prev_node_ = nullptr;
  Node* next_node_ = nullptr;
  pointer node_body_ = nullptr;
  allocator_type type_alloc_;
  friend struct List<TempT, Alloc>::ListBody;
};
//==========================================
//=============Node definition==============
template <typename TempT, typename Alloc>
const typename List<TempT, Alloc>::type_alloc_type&
List<TempT, Alloc>::Node::node_get_allocator() const noexcept {
  return type_alloc_;
}

template <typename TempT, typename Alloc>
void List<TempT, Alloc>::Node::node_copy_neighbours(
    const Node& other) noexcept {
  next_node_ = other.get_next_neighbour();
  prev_node_ = other.get_prev_neighbour();
}

template <typename TempT, typename Alloc>
void List<TempT, Alloc>::Node::node_copy_neighbours(Node&& other) noexcept {
  next_node_ = other.get_next_neighbour();
  prev_node_ = other.get_prev_neighbour();
}

template <typename TempT, typename Alloc>
void List<TempT, Alloc>::Node::node_set_next_neighbour(Node* node) noexcept {
  if (node->get_prev_neighbour() != nullptr) {
    if (node->prev_node_->next_node_ != nullptr) {
      node->prev_node_->next_node_ = nullptr;
      // untie the old next node from previous node of the node
    }
  }
  if (get_next_neighbour() != nullptr) {
    if (next_node_->prev_node_ != nullptr) {
      next_node_->prev_node_ = nullptr;
      // untie the old previous node from next node of this
    }
  }
  next_node_ = node;
  node->prev_node_ = &(*this);
}

template <typename TempT, typename Alloc>
void List<TempT, Alloc>::Node::node_set_prev_neighbour(Node* node) noexcept {
  if (node->get_next_neighbour() != nullptr) {
    if (node->next_node_->prev_node_ != nullptr) {
      node->next_node_->prev_node_ = nullptr;
      // untie the old previous node from next node of the node
    }
  }
  if (get_prev_neighbour() != nullptr) {
    if (prev_node_->next_node_ != nullptr) {
      prev_node_->next_node_ = nullptr;
      // untie the old next node from previous node of this
    }
  }
  prev_node_ = node;
  node->next_node_ = &(*this);
}
//==========================================

//================Iterator==================
template <typename TempT, typename Alloc>
template <bool IsConst>
class List<TempT, Alloc>::common_iterator
    : public std::iterator<std::bidirectional_iterator_tag,
                           std::conditional_t<IsConst, const TempT, TempT>> {
  //***************Fields*******************
  Node* cur_node_;
  //****************************************
  //**************Private usings****************
  using conditional_ptr = std::conditional_t<IsConst, const TempT*, TempT*>;
  using conditional_ref = std::conditional_t<IsConst, const TempT&, TempT&>;
  //********************************************
 public:
  //************Constructors****************
  common_iterator() noexcept : cur_node_() {}
  explicit common_iterator(const Node* other_node) noexcept
      : cur_node_(const_cast<Node*>(other_node)) {}
  template <bool IsConstTmp>
  common_iterator(const common_iterator<IsConstTmp>& other) noexcept
      : cur_node_(other.cur_node_) {}
  template <bool IsConstTmp>
  common_iterator<IsConst>& operator=(const common_iterator<IsConstTmp>& other);
  //****************************************
  //*********Memory Access Operators********
  conditional_ptr operator->() const noexcept { return &cur_node_->get_body(); }
  conditional_ptr operator->() noexcept { return &cur_node_->get_body(); }
  conditional_ref operator*() const noexcept { return cur_node_->get_body(); }
  conditional_ref operator*() noexcept { return cur_node_->get_body(); }
  //****************************************
  //***********Arithmetic Operators*********
  common_iterator<IsConst>& operator++() noexcept;
  common_iterator<IsConst>& operator--() noexcept;
  common_iterator<IsConst> operator++(int) noexcept;
  common_iterator<IsConst> operator--(int) noexcept;
  //****************************************
  //*********Comparation Operators**********
  template <bool IsConstTmp>
  friend bool operator==(const common_iterator<IsConst>& left,
                         const common_iterator<IsConstTmp>& right) noexcept {
    return left.cur_node_ == right.cur_node_;
  }
  template <bool IsConstTmp>
  friend bool operator!=(const common_iterator<IsConst>& left,
                         const common_iterator<IsConstTmp>& right) noexcept {
    return left.cur_node_ != right.cur_node_;
  }
  //****************************************
  friend struct List<TempT, Alloc>::ListBody;
};

template <typename TempT, typename Alloc>
template <bool IsConst>
template <bool IsConstTmp>
typename List<TempT, Alloc>::template common_iterator<IsConst>&
List<TempT, Alloc>::common_iterator<IsConst>::operator=(
    const common_iterator<IsConstTmp>& other) {
  cur_node_ = other.cur_node_;
  return *this;
}

template <typename TempT, typename Alloc>
template <bool IsConst>
typename List<TempT, Alloc>::template common_iterator<IsConst>&
List<TempT, Alloc>::common_iterator<IsConst>::operator++() noexcept {
  cur_node_ = cur_node_->get_next_neighbour();
  return *this;
}

template <typename TempT, typename Alloc>
template <bool IsConst>
typename List<TempT, Alloc>::template common_iterator<IsConst>&
List<TempT, Alloc>::common_iterator<IsConst>::operator--() noexcept {
  cur_node_ = cur_node_->get_prev_neighbour();
  return *this;
}

template <typename TempT, typename Alloc>
template <bool IsConst>
typename List<TempT, Alloc>::template common_iterator<IsConst>
List<TempT, Alloc>::common_iterator<IsConst>::operator++(int) noexcept {
  common_iterator<IsConst> tmp = *this;
  tmp.cur_node_ = tmp.cur_node_->get_next_neighbour();
  return *this;
}

template <typename TempT, typename Alloc>
template <bool IsConst>
typename List<TempT, Alloc>::template common_iterator<IsConst>
List<TempT, Alloc>::common_iterator<IsConst>::operator--(int) noexcept {
  common_iterator<IsConst> tmp = *this;
  tmp.cur_node_ = tmp.cur_node_->get_prev_neighbour();
  return *this;
}
//==========================================

//================ListBody==================
template <typename TempT, typename Alloc>
struct List<TempT, Alloc>::ListBody {
 private:
  //************Private usings**************
  using type_alloc_type = typename std::allocator_traits<
      allocator_type>::template rebind_alloc<value_type>;
  using type_alloc_traits = std::allocator_traits<type_alloc_type>;
  using node_alloc_type =
      typename type_alloc_traits::template rebind_alloc<Node>;
  using node_alloc_traits = std::allocator_traits<node_alloc_type>;
  using iterator = common_iterator<false>;
  using const_iterator = common_iterator<true>;
  using reverse_iterator = std::reverse_iterator<common_iterator<false>>;
  using const_reverse_iterator = std::reverse_iterator<common_iterator<true>>;
  //****************************************
 public:
  //*********Allocator functions************
  node_alloc_type& lb_get_node_allocator() noexcept { return node_alloc_; }
  const node_alloc_type& lb_get_node_allocator() const noexcept;
  type_alloc_type& lb_get_type_allocator() noexcept { return alloc_type_; }
  const type_alloc_type& lb_get_type_allocator() const noexcept;
  //****************************************
  //***********Member function**************
  typename List<TempT, Alloc>::Node* lb_get_head_node() noexcept;
  const typename List<TempT, Alloc>::Node* lb_get_head_node() const noexcept;
  typename List<TempT, Alloc>::Node* lb_get_tail_node() noexcept;
  const typename List<TempT, Alloc>::Node* lb_get_tail_node() const noexcept;
  typename List<TempT, Alloc>::Node* lb_get_imag_node() noexcept;
  const typename List<TempT, Alloc>::Node* lb_get_imag_node() const noexcept;
  //****************************************
 private:
  //*****************Fields*****************
  node_alloc_type node_alloc_;
  allocator_type alloc_type_;
  size_t size_ = 0;
  Node* im_node_ = nullptr;  // imageinary node
  //****************************************
  //**********Allocator functions***********
  typename List<TempT, Alloc>::Node* lb_node_allocate();
  void lb_node_deallocate(typename List<TempT, Alloc>::Node* node_ptr) noexcept;
  //****************************************
  //************Node functoins**************
  template <typename... Args>
  typename List<TempT, Alloc>::Node* lb_create_node(Args&&... args);
  void lb_creat_imaginary_node();
  void lb_destroy_imaginary_node() noexcept;
  template <typename... Args>
  void lb_insert_right(iterator pos, Args&&... args);
  template <typename... Args>
  void lb_insert_left(iterator pos, Args&&... args);
  void lb_erase(iterator pos) noexcept;
  //****************************************
  //*************Range Functions************
  void lb_range_poper(const size_t& idx) noexcept;
  // if have been throwen exception it clean all and throw furthen
  // the exception
  void lb_range_defaultly_init(const size_t& len);
  // construct list with default value
  void lb_range_fill_init(const size_t& len, const TempT& value);
  // len times push_back into list
  template <typename Iter>
  void lb_range_copy_init(Iter start, Iter finish);
  //****************************************
 public:
  //**************Constructors**************
  ListBody()
      : node_alloc_(node_alloc_type(allocator_type())),
        alloc_type_(allocator_type()) {}
  ListBody(const allocator_type& alr)
      : node_alloc_(node_alloc_type(alr)), alloc_type_(alr) {}
  ListBody(const size_t& len, const allocator_type& alr = allocator_type());
  ListBody(const size_t& len, const value_type& value,
           const allocator_type& alr = allocator_type());
  ListBody(const ListBody& other);
  ListBody(const ListBody& other, const allocator_type& alr);
  ListBody(std::initializer_list<TempT> init_l,
           const allocator_type& alr = allocator_type());
  ~ListBody();
  ListBody& operator=(const ListBody& other) &;
  ListBody& operator=(ListBody&& other) & noexcept;

  //****************************************
  //***************Comparation**************
  friend bool operator==(const ListBody& left, const ListBody& right) {
    if (left.lb_size() != right.lb_size()) {
      return false;
    }
    const_iterator it1(left.lb_get_head_node());
    const_iterator it2(right.lb_get_head_node());
    const_iterator fin1(left.lb_get_tail_node()->get_next_neighbour());
    const_iterator fin2(right.lb_get_tail_node()->get_next_neighbour());
    while (it1 != fin1 && it2 != fin2 && *it1 == *it2) {
      ++it1;
      ++it2;
    }
    return it1 == fin1 && it2 == fin2;
  }
  friend bool operator!=(const ListBody& left, const ListBody& right) {
    return !(left == right);
  }
  //****************************************
  //*****************Capacity***************
  size_t lb_size() const noexcept { return size_; }
  bool lb_empty() const noexcept { return size_ == 0; }
  //****************Modifiers***************
  void lb_push_back(const TempT& value);
  void lb_push_back(TempT&& value);
  template <typename... Args>
  void lb_emplace_back(Args&&... args);
  void lb_push_front(const TempT& value);
  void lb_push_front(TempT&& value);
  template <typename... Args>
  void lb_emplace_front(Args&&... args);
  void lb_pop_back() noexcept { lb_erase(iterator(lb_get_tail_node())); }
  void lb_pop_front() noexcept { lb_erase(iterator(lb_get_head_node())); }
  void lb_clear() noexcept;
  //****************************************
};
//==========================================

//^^^^^^^^^^^^^^^^^^List^^^^^^^^^^^^^^^^^^^^
//===============Constructors===============
template <typename TempT, typename Alloc>
List<TempT, Alloc>::List() : list_body_() {}

template <typename TempT, typename Alloc>
List<TempT, Alloc>::List(const size_t& count, const TempT& value,
                         const Alloc& alloc)
    : list_body_(count, value, alloc) {}

template <typename TempT, typename Alloc>
List<TempT, Alloc>::List(const size_t& count, const Alloc& alloc)
    : list_body_(count, alloc) {}

template <typename TempT, typename Alloc>
List<TempT, Alloc>::List(const List& other) : list_body_(other.list_body_) {}

template <typename TempT, typename Alloc>
List<TempT, Alloc>::List(std::initializer_list<TempT> init, const Alloc& alloc)
    : list_body_(init, alloc) {}

template <typename TempT, typename Alloc>
List<TempT, Alloc>::~List() = default;

template <typename TempT, typename Alloc>
List<TempT, Alloc>& List<TempT, Alloc>::operator=(
    const List<TempT, Alloc>& other) & {
  list_body_ = other.list_body_;
  return *this;
}
//==========================================
//=================Capacity=================
template <typename TempT, typename Alloc>
size_t List<TempT, Alloc>::size() const noexcept {
  return list_body_.lb_size();
}

template <typename TempT, typename Alloc>
bool List<TempT, Alloc>::empty() const noexcept {
  return list_body_.lb_empty();
}

//==========================================
//=================Modifiers================
template <typename TempT, typename Alloc>
void List<TempT, Alloc>::pop_back() noexcept {
  list_body_.lb_pop_back();
}

template <typename TempT, typename Alloc>
void List<TempT, Alloc>::pop_front() noexcept {
  list_body_.lb_pop_front();
}

template <typename TempT, typename Alloc>
void List<TempT, Alloc>::push_back(const TempT& value) {
  list_body_.lb_push_back(value);
}

template <typename TempT, typename Alloc>
void List<TempT, Alloc>::push_back(TempT&& value) {
  list_body_.lb_push_back(std::move_if_noexcept(value));
}

template <typename TempT, typename Alloc>
void List<TempT, Alloc>::push_front(const TempT& value) {
  list_body_.lb_push_front(value);
}

template <typename TempT, typename Alloc>
void List<TempT, Alloc>::push_front(TempT&& value) {
  list_body_.lb_push_front(std::move_if_noexcept(value));
}
//==========================================
//=============Iterator Methods=============
template <typename TempT, typename Alloc>
typename List<TempT, Alloc>::iterator List<TempT, Alloc>::begin() noexcept {
  return typename List<TempT, Alloc>::iterator(list_body_.lb_get_head_node());
}

template <typename TempT, typename Alloc>
typename List<TempT, Alloc>::const_iterator List<TempT, Alloc>::begin()
    const noexcept {
  return typename List<TempT, Alloc>::const_iterator(
      list_body_.lb_get_head_node());
}

template <typename TempT, typename Alloc>
typename List<TempT, Alloc>::const_iterator List<TempT, Alloc>::cbegin()
    const noexcept {
  return typename List<TempT, Alloc>::const_iterator(
      list_body_.lb_get_head_node());
}

template <typename TempT, typename Alloc>
typename List<TempT, Alloc>::iterator List<TempT, Alloc>::end() noexcept {
  return typename List<TempT, Alloc>::iterator(
      list_body_.lb_get_tail_node()->get_next_neighbour());
}

template <typename TempT, typename Alloc>
typename List<TempT, Alloc>::const_iterator List<TempT, Alloc>::end()
    const noexcept {
  return typename List<TempT, Alloc>::const_iterator(
      list_body_.lb_get_tail_node()->get_next_neighbour());
}

template <typename TempT, typename Alloc>
typename List<TempT, Alloc>::const_iterator List<TempT, Alloc>::cend()
    const noexcept {
  return typename List<TempT, Alloc>::const_iterator(
      list_body_.lb_get_tail_node()->get_next_neighbour());
}

template <typename TempT, typename Alloc>
typename List<TempT, Alloc>::reverse_iterator
List<TempT, Alloc>::rbegin() noexcept {
  return typename List<TempT, Alloc>::reverse_iterator(end());
}

template <typename TempT, typename Alloc>
typename List<TempT, Alloc>::const_reverse_iterator List<TempT, Alloc>::rbegin()
    const noexcept {
  return typename List<TempT, Alloc>::const_reverse_iterator(end());
}

template <typename TempT, typename Alloc>
typename List<TempT, Alloc>::const_reverse_iterator
List<TempT, Alloc>::crbegin() const noexcept {
  return typename List<TempT, Alloc>::const_reverse_iterator(end());
}

template <typename TempT, typename Alloc>
typename List<TempT, Alloc>::reverse_iterator
List<TempT, Alloc>::rend() noexcept {
  return typename List<TempT, Alloc>::reverse_iterator(begin());
}

template <typename TempT, typename Alloc>
typename List<TempT, Alloc>::const_reverse_iterator List<TempT, Alloc>::rend()
    const noexcept {
  return typename List<TempT, Alloc>::const_reverse_iterator(begin());
}

template <typename TempT, typename Alloc>
typename List<TempT, Alloc>::const_reverse_iterator List<TempT, Alloc>::crend()
    const noexcept {
  return typename List<TempT, Alloc>::const_reverse_iterator(begin());
}
//==========================================
//=============Alloc Getters================
template <typename TempT, typename Alloc>
Alloc List<TempT, Alloc>::get_allocator() noexcept {
  return list_body_.lb_get_type_allocator();
}

template <typename TempT, typename Alloc>
Alloc List<TempT, Alloc>::get_allocator() const noexcept {
  return list_body_.lb_get_type_allocator();
}
//==========================================
template <typename TempT, typename Alloc>
const typename List<TempT, Alloc>::node_alloc_type&
List<TempT, Alloc>::ListBody::lb_get_node_allocator() const noexcept {
  return node_alloc_;
}

template <typename TempT, typename Alloc>
const typename List<TempT, Alloc>::type_alloc_type&
List<TempT, Alloc>::ListBody::lb_get_type_allocator() const noexcept {
  return alloc_type_;
}

template <typename TempT, typename Alloc>
typename List<TempT, Alloc>::Node*
List<TempT, Alloc>::ListBody::lb_get_head_node() noexcept {
  return im_node_->get_next_neighbour();
}

template <typename TempT, typename Alloc>
const typename List<TempT, Alloc>::Node*
List<TempT, Alloc>::ListBody::lb_get_head_node() const noexcept {
  return im_node_->get_next_neighbour();
}

template <typename TempT, typename Alloc>
typename List<TempT, Alloc>::Node*
List<TempT, Alloc>::ListBody::lb_get_tail_node() noexcept {
  return im_node_->get_prev_neighbour();
}

template <typename TempT, typename Alloc>
const typename List<TempT, Alloc>::Node*
List<TempT, Alloc>::ListBody::lb_get_tail_node() const noexcept {
  return im_node_->get_prev_neighbour();
}

template <typename TempT, typename Alloc>
typename List<TempT, Alloc>::Node*
List<TempT, Alloc>::ListBody::lb_get_imag_node() noexcept {
  return im_node_;
}

template <typename TempT, typename Alloc>
const typename List<TempT, Alloc>::Node*
List<TempT, Alloc>::ListBody::lb_get_imag_node() const noexcept {
  return im_node_;
}

template <typename TempT, typename Alloc>
typename List<TempT, Alloc>::Node*
List<TempT, Alloc>::ListBody::lb_node_allocate() {
  return node_alloc_traits::allocate(node_alloc_, 1);
  // if it throw exception it will throw further
}

template <typename TempT, typename Alloc>
void List<TempT, Alloc>::ListBody::lb_node_deallocate(
    typename List<TempT, Alloc>::Node* node_ptr) noexcept {
  node_alloc_traits::deallocate(node_alloc_, node_ptr, 1);
}

template <typename TempT, typename Alloc>
template <typename... Args>
typename List<TempT, Alloc>::Node* List<TempT, Alloc>::ListBody::lb_create_node(
    Args&&... args) {
  Node* ptr = lb_node_allocate();
  node_alloc_type& all_node = lb_get_node_allocator();
  try {
    ptr->node_body_ = type_alloc_traits::allocate(lb_get_type_allocator(), 1);
    type_alloc_traits::construct(lb_get_type_allocator(), ptr->node_body_,
                                 std::forward<Args>(args)...);
  } catch (...) {
    /// TODO: допилить мысль
    type_alloc_traits::deallocate(lb_get_type_allocator(), ptr->node_body_, 1);
    lb_node_deallocate(ptr);
    throw;
  }
  return ptr;
}

template <typename TempT, typename Alloc>
void List<TempT, Alloc>::ListBody::lb_creat_imaginary_node() {
  im_node_ = lb_node_allocate();
  im_node_->next_node_ = im_node_;
  im_node_->prev_node_ = im_node_;
}  // function to creat imaginary node

template <typename TempT, typename Alloc>
void List<TempT, Alloc>::ListBody::lb_destroy_imaginary_node() noexcept {
  if (im_node_ == nullptr) {
    return;
  }
  lb_node_deallocate(im_node_);
}

template <typename TempT, typename Alloc>
template <typename... Args>
void List<TempT, Alloc>::ListBody::lb_insert_right(iterator pos,
                                                   Args&&... args) {
  Node* tmp_node = lb_create_node(std::forward<Args>(args)...);
  // if throws exception in construction it will throwen further
  Node* pos_cur_node = pos.cur_node_;
  Node* pos_prev_node = pos.cur_node_->get_prev_neighbour();
  pos_prev_node->next_node_ = tmp_node;
  tmp_node->prev_node_ = pos_prev_node;
  pos_cur_node->prev_node_ = tmp_node;
  tmp_node->next_node_ = pos_cur_node;
  ++size_;
}  // this is usual insert

template <typename TempT, typename Alloc>
template <typename... Args>
void List<TempT, Alloc>::ListBody::lb_insert_left(iterator pos,
                                                  Args&&... args) {
  Node* tmp_node = lb_create_node(std::forward<Args>(args)...);
  // if throws exception in construction it will throwen further
  Node* pos_cur_node = pos.cur_node_;
  Node* pos_next_node = pos.cur_node_->get_next_neighbour();
  pos_next_node->prev_node_ = tmp_node;
  tmp_node->next_node_ = pos_next_node;
  pos_cur_node->next_node_ = tmp_node;
  tmp_node->prev_node_ = pos_cur_node;
  ++size_;
}

template <typename TempT, typename Alloc>
void List<TempT, Alloc>::ListBody::lb_erase(iterator pos) noexcept {
  --size_;
  Node* pos_prev_node = pos.cur_node_->get_prev_neighbour();
  Node* pos_next_node = pos.cur_node_->get_next_neighbour();
  Node* pos_cur_node = pos.cur_node_;
  pos_prev_node->node_set_next_neighbour(pos_next_node);
  pos_next_node->node_set_prev_neighbour(pos_prev_node);
  if (pos_cur_node != nullptr) {
    type_alloc_traits::destroy(lb_get_type_allocator(),
                               pos_cur_node->node_body_);
    type_alloc_traits::deallocate(lb_get_type_allocator(),
                                  pos_cur_node->node_body_, 1);
    lb_node_deallocate(pos_cur_node);
  }
}

template <typename TempT, typename Alloc>
void List<TempT, Alloc>::ListBody::lb_range_poper(const size_t& idx) noexcept {
  for (size_t i = 0; i < idx; ++i) {
    lb_pop_back();
  }
}

template <typename TempT, typename Alloc>
void List<TempT, Alloc>::ListBody::lb_range_defaultly_init(const size_t& len) {
  size_t idx = 0;
  try {
    for (; idx < len; ++idx) {
      lb_emplace_back();
    }
  } catch (...) {
    lb_range_poper(idx);
    throw;
  }
}

template <typename TempT, typename Alloc>
void List<TempT, Alloc>::ListBody::lb_range_fill_init(const size_t& len,
                                                      const TempT& value) {
  size_t idx = 0;
  try {
    for (; idx < len; ++idx) {
      lb_push_back(value);
    }
  } catch (...) {
    lb_range_poper(idx);
    throw;
  }
}

template <typename TempT, typename Alloc>
template <typename Iter>
void List<TempT, Alloc>::ListBody::lb_range_copy_init(Iter start, Iter finish) {
  size_t idx = 0;
  try {
    for (; start != finish; ++start) {
      lb_emplace_back(*start);
      ++idx;
    }
  } catch (...) {
    lb_range_poper(idx);
    throw;
  }
}

template <typename TempT, typename Alloc>
List<TempT, Alloc>::ListBody::ListBody(const size_t& len,
                                       const allocator_type& alr)
    : node_alloc_(node_alloc_type(alr)), alloc_type_(alr) {
  lb_creat_imaginary_node();
  try {
    lb_range_defaultly_init(len);
  } catch (...) {
    lb_destroy_imaginary_node();
    throw;
  }
}

template <typename TempT, typename Alloc>
List<TempT, Alloc>::ListBody::ListBody(const size_t& len,
                                       const value_type& value,
                                       const allocator_type& alr)
    : node_alloc_(node_alloc_type(alr)), alloc_type_(alr) {
  lb_creat_imaginary_node();
  try {
    lb_range_fill_init(len, value);
  } catch (...) {
    lb_destroy_imaginary_node();
    throw;
  }
}

template <typename TempT, typename Alloc>
List<TempT, Alloc>::ListBody::ListBody(const ListBody& other)
    : node_alloc_(node_alloc_traits::select_on_container_copy_construction(
          other.lb_get_node_allocator())),
      alloc_type_(type_alloc_traits::select_on_container_copy_construction(
          other.alloc_type_)) {
  lb_creat_imaginary_node();
  try {
    lb_range_copy_init(
        iterator(other.lb_get_head_node()),
        iterator(other.lb_get_tail_node()->get_next_neighbour()));
  } catch (...) {
    lb_destroy_imaginary_node();
    throw;
  }
}

template <typename TempT, typename Alloc>
List<TempT, Alloc>::ListBody::ListBody(const ListBody& other,
                                       const allocator_type& alr)
    : node_alloc_(node_alloc_type(alr)), alloc_type_(alr) {
  lb_creat_imaginary_node();
  try {
    lb_range_copy_init(
        iterator(other.lb_get_head_node()),
        iterator(other.lb_get_tail_node()->get_next_neighbour()));
  } catch (...) {
    lb_destroy_imaginary_node();
    throw;
  }
}

template <typename TempT, typename Alloc>
List<TempT, Alloc>::ListBody::ListBody(std::initializer_list<TempT> init_l,
                                       const allocator_type& alr)
    : node_alloc_(node_alloc_type(alr)), alloc_type_(alr) {
  lb_creat_imaginary_node();
  try {
    lb_range_copy_init(init_l.begin(), init_l.end());
  } catch (...) {
    lb_destroy_imaginary_node();
    throw;
  }
}

template <typename TempT, typename Alloc>
List<TempT, Alloc>::ListBody::~ListBody() {
  lb_clear();
  lb_destroy_imaginary_node();
}

template <typename TempT, typename Alloc>
typename List<TempT, Alloc>::ListBody& List<TempT, Alloc>::ListBody::operator=(
    const ListBody& other) & {
  const bool kRes =
      node_alloc_traits::propagate_on_container_copy_assignment::value;
  if (kRes) {
    node_alloc_ = other.node_alloc_;
    alloc_type_ = other.alloc_type_;
  }
  if (other.lb_empty()) {
    lb_clear();
    return *this;
  }
  iterator iter2(other.lb_get_head_node());
  if (!lb_empty()) {
    size_t min_of_two = std::min(other.lb_size(), lb_size());
    iterator iter1(lb_get_head_node());
    for (size_t i = 0; i < min_of_two; ++i, ++iter1, ++iter2) {
      *iter1 = *iter2;
    }
    while (lb_size() > other.lb_size()) {
      lb_pop_back();
    }
  }
  try {
    while (lb_size() < other.lb_size()) {
      lb_push_back(*iter2);
      ++iter2;
    }
  } catch (...) {
    lb_clear();
    throw;
  }
  return *this;
}

template <typename TempT, typename Alloc>
typename List<TempT, Alloc>::ListBody& List<TempT, Alloc>::ListBody::operator=(
    ListBody&& other) & noexcept {
  if (lb_get_imag_node() != nullptr) {
    lb_clear();
    lb_destroy_imaginary_node();
    // suicide
  }
  im_node_ = std::move(other.lb_get_imag_node());
  size_ = other.lb_size();
  node_alloc_ = std::move(other.lb_get_node_allocator());
  alloc_type_ = std::move(other.alloc_type_);
  // move all fields
  other.size_ = 0;
  im_node_ = nullptr;
  // push default value into other
  return *this;
}

template <typename TempT, typename Alloc>
void List<TempT, Alloc>::ListBody::lb_push_back(const TempT& value) {
  if (lb_get_imag_node() == nullptr) {
    lb_creat_imaginary_node();
  }
  lb_insert_right(iterator(lb_get_imag_node()), value);
}

template <typename TempT, typename Alloc>
void List<TempT, Alloc>::ListBody::lb_push_back(TempT&& value) {
  if (lb_get_imag_node() == nullptr) {
    lb_creat_imaginary_node();
  }
  lb_insert_right(iterator(lb_get_imag_node()), std::move_if_noexcept(value));
}

template <typename TempT, typename Alloc>
template <typename... Args>
void List<TempT, Alloc>::ListBody::lb_emplace_back(Args&&... args) {
  if (lb_get_imag_node() == nullptr) {
    lb_creat_imaginary_node();
  }
  lb_insert_right(iterator(lb_get_imag_node()), std::forward<Args>(args)...);
}

template <typename TempT, typename Alloc>
void List<TempT, Alloc>::ListBody::lb_push_front(const TempT& value) {
  if (lb_get_imag_node() == nullptr) {
    lb_creat_imaginary_node();
  }
  lb_insert_left(iterator(lb_get_imag_node()), value);
}

template <typename TempT, typename Alloc>
void List<TempT, Alloc>::ListBody::lb_push_front(TempT&& value) {
  if (lb_get_imag_node() == nullptr) {
    lb_creat_imaginary_node();
  }
  lb_insert_left(iterator(lb_get_imag_node()), std::move_if_noexcept(value));
}

template <typename TempT, typename Alloc>
template <typename... Args>
void List<TempT, Alloc>::ListBody::lb_emplace_front(Args&&... args) {
  if (lb_get_imag_node() == nullptr) {
    lb_creat_imaginary_node();
  }
  lb_insert_left(iterator(lb_get_imag_node()), std::forward<Args>(args)...);
}

template <typename TempT, typename Alloc>
void List<TempT, Alloc>::ListBody::lb_clear() noexcept {
  while (!lb_empty()) {
    lb_pop_back();
  }
}
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
