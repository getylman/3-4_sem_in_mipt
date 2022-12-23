#include <iostream>

template<typename T>
struct Node {
  T value;
  int height;
  Node* left_kid;
  Node* right_kid;
};

template<typename T>
class MySearchTree {
 public:
  MySearchTree() {
    head_ = nullptr;
    head_->height = 0;
  }
  void Insert(T elem, Node<T>* node) {
    int h = node->height;
    if (node == nullptr) {
      node = new Node<T>;
      node->left_kid = nullptr;
      node->right_kid = nullptr;
      node->value = elem;
      node->height = h + 1;
      break;
    }
  }
 private:
  Node<T>* head_;  
};


int main() {

}