#include <iostream>

template<typename T>
struct Node {
  T val;
  Node *left, *right, *parent;
};

template<typename T>
class MySplayTree {
 public:
  MySplayTree(T val) {
    root_ = new Node<T>;
    root_->val = val;
    root_->left = root_->right = root_->parent = nullptr; 
  }
  void Insert(Node<T> *elem) {
    Node<T> *node = root_;
    while (node) {
      if (node->val > elem->val) {
        if (node->left == nullptr) {
          node->left = elem;
          elem->parent = node;
          break;
        }
        node = node->left;
      } else {
        if (node->right == nullptr) {
          node->right = elem;
          elem->parent = node;
          break;
        }
        node = node->right;
      }
    }
    Splay_(elem);
  }
  Node<T>* Find(const T& val) {
    Node<T> *node = root_;
    while (node && node->val == val) {
      if (node->val < val) {
        node = node->right;
      } else {
        node = node->left;
      }
    }
    if (node) {
      Splay_(node);
    } 
    return root_;
  }
  void Erase(Node<T> *elem) {
    Node<T> *node = Find(elem->val);
    MySplayTree<T> other;
    other.root_ = root_->right;
    root_ = root_->left;
    this->Merge_(other);
    delete node;
  }
 private:
  Node<T> *root_;
  void Splay_(Node<T> *node) {
    if (node->parent == nullptr) {
      return;
    }
    while (node->parent && node->parent->parent) {
      if (node->parent->right == node) {
        if (node->parent->parent->right == node->parent) {
          node->parent = LRot_(node->parent);
        } else {
          node = RRot_(node);
          node = LRot_(node);
        }
      } else {

      }
    }
    if (node->parent) {
      if (node->parent->left == node) {
        RRot_(node);
      } else {
        LRot_(node);
      }
    }
  }
  Node<T>* LRot_(Node<T> *elem) {
    Node<T> *node = elem->parent;
    node->right = elem->left;
    elem->left = node;
    return elem;
  }
  Node<T>* RRot_(Node<T> *elem) {
    Node<T> *node = elem->parent;
    node->left = elem->right;
    elem->right = node;
    return elem;
  }
  Node<T>* Merge_(MySplayTree<T>& other) { // переделай на две ноды мердж говно
    Node<T> *node = root_;
    if (root_ == nullptr) {
      return other.root_;
    }
    while (node && node->rihgt) {
      node = node->right;
    }
    Splay_(node);
    root_->right = other.root_;
    other.root_ = nullptr;
    return root_;
  }
  std::pair<Node<T>*, Node<T>*> Split_(Node<T> *elem) {
    Find(elem->val);
    Node<T> left = root_->left;
    root_->left = nullptr;
    return {left, root_};
  }
};

int main() {

}