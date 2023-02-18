#include <iostream>
#include <vector>

template<typename T>
class MyBTree {
 public:
  explicit MyBTree(const size_t& t_rank) : t_rank(t_rank) {}
  int Find(const T& elem) const {
    Node* node = root_;
    while (true) {
      for (size_t i = 0; i < node->var_arr.size(); ++i) {
        if (node->var_arr[i] == elem) {
          return true;//надо переписать файнд в ловер баунд я сам заебался пойду катать остальные задачи
        }
        if (node->var_arr[i] > elem) {
          if (node->link_arr[i] == nullptr) {
            return false;
          }
          node = node->link_arr[i];
          break;
        }
      }
      node = node->link_arr.back();
    }
  }
  void Insert(const T& elem) { //хуйня занаво 
    if (root_ == nullptr) {
      root_ = new Node;
      root_->var_arr.push_back(elem);
      root_->link_arr.push_back(nullptr);
      root_->link_arr.push_back(nullptr);
      root_->parent = nullptr;
      return;
    }
    
    Node* node = GoDown(root_, elem, nullptr, 0);
    Node* tmp_node = nullptr;
    for (; node != nullptr; node = node->parent) {
      if (node->var_arr.size() == 0) {
        node->var_arr.push_back(elem);
        node->link_arr.push_back(nullptr);
        node->link_arr.push_back(nullptr);
        break;
      }
      if (node->var_arr.size() < t_rank - 1) {
        if (elem > node->var_arr.back()) {
          node->var_arr.push_back(elem);
          node->link_arr.push_back(nullptr);
        }
        for (size_t i = 0; i < node->var_arr.size(); ++i) {
          if (node->var_arr[i] > elem) {
            node->var_arr.insert(node->var_arr.begin() + i, elem);
            node->link_arr.insert(node->link_arr.begin() + i, nullptr);
            break;
          }
        }
        break;
      }
      if (node->var_arr.size() == t_rank - 1 && node->parent != nullptr && node->parent->var_arr.size() < t_rank) {
        size_t node_pos = 0;
        for (size_t i = 0; i < node->parent->var_arr.size(); ++i) {
          if (node->parent->link_arr[i] == node) {
            node_pos = i;
            break;
          }
        }
        if (node_pos + 1 <= t_rank - 1) {
          tmp_node = node->parent->link_arr[node_pos + 1]; //tmp_node - right brother
          if (tmp_node != nullptr) {
            if (tmp_node->var_arr.size() == t_rank - 1) {
              Node* new_node = new Node;
              size_t par1, par2;
              std::vector<T> pos_arr;
              std::vector<Node*> pos_link;
              pos_arr.reserve(2 * t_rank);
              pos_link.reserve(2 * t_rank + 3);
              for (size_t i = 0; i < node->var_arr.size(); ++i) {
                pos_arr.push_back(node->var_arr[i]);
                pos_link.push_back(node->link_arr[i]);
              }
              pos_link.push_back(node->link_arr.back());
              pos_arr.push_back(elem);
              pos_link.push_back(node->parent->link_arr[node_pos]);
              pos_arr.push_back(node->parent->var_arr[node_pos]);
              for (size_t i = 0; i < tmp_node->var_arr.size(); ++i) {
                pos_arr.push_back(tmp_node->var_arr[i]);
                pos_link.push_back(tmp_node->link_arr[i]);
              }
              pos_link.push_back(tmp_node->link_arr.back());
              node->var_arr.resize((2 * t_rank - 2) / 3);
              node->link_arr.resize((2 * t_rank - 2) / 3);
              node->link_arr.push_back(nullptr);
              par1 = pos_arr[(2 * t_rank - 2) / 3];
              for (size_t i = ((2 * t_rank - 2) / 3) + 1; i < (4 * t_rank) / 3; ++i) {
                tmp_node->var_arr[i - (((2 * t_rank - 2) / 3) + 1)] = pos_arr[i];
                tmp_node->link_arr[i - (((2 * t_rank - 2) / 3) + 1)] = pos_link[i];
              }
              tmp_node->var_arr.resize((2 * t_rank - 2) / 3);
              tmp_node->link_arr.resize((2 * t_rank - 2) / 3);
              tmp_node->link_arr.push_back(nullptr);
              par2 = pos_arr[(4 * t_rank) / 3];
              for (size_t i = ((4 * t_rank) / 3 + 1); i < 2 * t_rank; ++i) {
                new_node->var_arr.push_back(pos_arr[i]);
                new_node->link_arr.push_back(pos_link[i]);
              }
              pos_link.push_back(nullptr);
              if (node_pos == 0) {
                node_pos = 1;
              }
              node->parent->var_arr[node_pos - 1] = par1;
              node->parent->var_arr[node_pos] = par2;
              node->parent->link_arr[node_pos -1] = node;
              node->parent->link_arr[node_pos] = tmp_node;
              node->parent->link_arr[node_pos + 1] = new_node;
            }
          } else {
            node_pos = ((node_pos == 0) ? 1 : node_pos);
            T put = node->parent->var_arr[node_pos - 1];
            tmp_node->var_arr.insert(node->var_arr.begin(), put);
            node->parent->var_arr[node_pos - 1] = node->var_arr.back();
          }
        }
      }
    }
    
  }
  ~MyBTree() {
    Suicide(root_);
  }
 private:
  size_t t_rank;
  struct Node {
    std::vector<T> var_arr;
    std::vector<Node*> link_arr;
    Node* parent = nullptr;
    bool is_leaf = true;
    Node() {
      var_arr.reserve(2 * t_rank);
      link_arr.reserve(2 * t_rank);
    }
  };
  Node* root_ = nullptr;

  Node* GoDown(Node*& node, const T& elem, Node*& pr_node, size_t id) {
    if (node != nullptr) {
      if (node->is_leaf) {
        return node;
      } else {
        for (size_t i = 0; i < node->var_arr.size(); ++i) {
          if (node->var_arr[i] > elem) {
            node = GoDown(node->link_arr[i], elem, node, i);
          }
        }
        node = GoDown(node->link_arr.back(), elem, node, node->var_arr.size());
      }
    } else {
      Node* new_node = new Node;
      node = new_node;
      pr_node->link_arr[id] = node;
      node->parent = pr_node;
      return node;
    }
  }
  void Suicide(Node*& node) {
    if (node == nullptr) {
      return;
    }
    for (size_t i = 0; i < node->link_arr.size(); ++i) {
      Suicide(node->link_arr[i]);
    }
    bool children_died = true;
    for (size_t i = 0; i < node->link_arr.size(); ++i) {
      if (node->link_arr[i] != nullptr) {
        children_died = false;
      }
    }
    if (children_died) {
      delete node;
      node = nullptr;
      return;
    }
  }
};

int main() {
  MyBTree<size_t> my_btree(4);
  size_t coms;
  std::cin >> coms;
  char com;
  size_t num;
  for (int i = 0; i < coms; ++i) {
    std::cin >> com >> num;
    if (com == '+') {
      my_btree.Insert(num);
    } else {
      my_btree.Find(num);
    }
  }
}

/* я не закончил реализацию этого листа и на серидине подзабил на него
template<typename T>
class MyList {
 public:
  explicit MyList() : size_(0), head_(nullptr), tail_(nullptr) {}
  void PushFront(T val) {
    ListNode* node = new ListNode;
    node->value = val;
    if (Empty()) {
      head_ = node;
      head_->nx_node = nullptr;
      head_->pr_node = nullptr;
      tail_ = head_;
    } else {
      node->nx_node = head_;
      head_->pr_node = node;
      head_ = node;
      head_->pr_node = nullptr;
    }
    ++size_;
  }
  void PushBack(T val) {
    ListNode* node = new ListNode;
    node->value = val;
    if (Empty()) {
      head_ = node;
      head_->nx_node = nullptr;
      head_->pr_node = nullptr;
      tail_ = head_;
    } else {
      node->pr_node = tail_;
      tail_->nx_node = node;
      tail_ = node;
      tail_->nx_node = nullptr;
    }
    ++size_;
  }
  void Remove(T val) {
    ListNode*& node = Find(val);
    if (node != nullptr) {
      ListNode *&pr_tmp, *&nx_tmp;
      pr_tmp = node->pr_node;
      nx_tmp = node->nx_node;
      if (nx_tmp != nullptr) {
        nx_tmp->pr_node = pr_tmp; //во избежание если правый сосед пуст
      }
      if (pr_tmp != nullptr) {
        pr_tmp->nx_node = nx_tmp; //во избежании если левый сосед пут
      }
      delete node;
      node = nullptr;
      --size_;
    }
  }
  std::pair<ListNode*, ListNode*> Split(T val) {
    ListNode*& node = Find(val);
    if (node == nullptr) {
      return {nullptr, nullptr};
    }
    ListNode*& nx_tmp = node->nx_node;
    if (nx_tmp != nullptr) {
      nx_tmp->pr_node = nullptr;
      node->nx_node = nullptr;
    }
    return {head_, nx_tmp};
  }
  friend ListNode* Merge(MyList*& my_list) {
    return nullptr;
  }
  size_t Size() { return size_; }
  bool Empty() { return size_ == 0;}
  ListNode* Find(T val) {
    if (tail_->value < val || head_->value > val) {
      return nullptr; //такого элемента просто не было
    }
    ListNode* node = head_;
    while (node != nullptr && node->value < val) {
      if (node->value == val) {
        return node;
      }
      node = node->nx_node;
    }
    return nullptr;
  }
 private:
  struct ListNode {
    T value;
    ListNode *pr_node, *nx_node;
  };
  ListNode *head_, *tail_;
  size_t size_;
};

template<typename T>
struct Node {
  
};

int main() {
  
}*/