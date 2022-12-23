#include <iostream>

template<typename T1, typename T2>
class Node {
 public:
  T1 key;
  T2 val;
  Node* left;
  Node* right;
};

template<typename T1, typename T2>
class MyTreap {
 public:
  MyTreap(T1 key, T2 val) {
    root_ = new Node<T1, T2>;
    root_->key = key;
    root_->val = val;
    root_->left = nullptr;
    root_->right = nullptr;
  }
  MyTreap() {
    root_ = new Node<T1, T2>;
    root_->left = nullptr;
    root_->right = nullptr;
  }
  void Insert(Node<T1, T2>* elem) {
    std::pair<Node<T1, T2>*, Node<T1, T2>*> tmp = Split_(elem);
    Merge_(Merge_(tmp.first, elem), tmp.second); 
  }
  void Remove(Node<T1, T2>* elem) {
    Node<T1, T2>* position;
    if (Find_(elem, position)) {
      Merge_(elem->left, elem->right);
      delete position;
    }
  }
  ~MyTreap() {
    delete root_;
  }
 private:
  Node<T1, T2>* root_ = nullptr;

  Node<T1, T2>* Merge_(Node<T1, T2>* node1, Node<T1, T2>* node2) {
    if (node1 == nullptr) {
      return node2;
    }
    if (node2 == nullptr) {
      return node1;
    }
    if (node1->val <= node2->val) {
      Node<T1, T2>* node = Merge_(node1, node2->left);
      node2->left = node;
      return node2;
    } else {
      Node<T1, T2>* node = Merge_(node1->right, node2);
      node1->right = node;
      return node1;
    }
  }
  std::pair<Node<T1, T2>*, Node<T1, T2>*> Split_(Node<T1, T2>* node) {
    if (root_ == nullptr) {
      return {nullptr, nullptr};
    }
    if (root_->key > node->key) {
      std::pair<Node<T1, T2>*, Node<T1, T2>*> pair_of_treaps = Split_(root_->left);
      root_->left = pair_of_treaps.second;
      return {pair_of_treaps.first, root_};
    } else {
      std::pair<Node<T1, T2>*, Node<T1, T2>*> pair_of_treaps = Split_(root_->right);
      root_->right = pair_of_treaps.first;
      return {root_, pair_of_treaps.second};
    }
  }
  bool Find_(T1 key, Node<T1, T2>* &position) {
    if (root_->key == key) {
      position = root_;
      return true;
    }
    if (root_->key > key) {
      if (root_->left != nullptr) {
        return root_->left.Find_(key, position);
      }
      position = nullptr;
      return false;
    } else {
      if (root_->right != nullptr) {
        return root_->right.Find_(key, position);
      }
      position = nullptr;
      return false;
    }
  }
};

int main() {

}