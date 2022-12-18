#include <iostream>

struct Node {
  int key, prior;
  Node *left, *right, *parent;
};

class LineTreap {
 public:
  LineTreap() {
    root_ = nullptr;
    previous_node_ = nullptr;
  }
  void Insert(int key, int prior) {
    Node* node = new Node;
    node->key = key;
    node->left = nullptr;
    node->right = nullptr;
    node->prior = prior;
    node->parent = previous_node_;
    if (root_ != nullptr) {
      if (previous_node_->prior < node->prior) {
        previous_node_->right = node;
      } else {
        Node* tmp = previous_node_;
        while ((tmp != nullptr) && (node->prior < tmp->prior)) {
          tmp = tmp->parent;
        }
        if (tmp == nullptr) {
          node->left = root_;
          root_->parent = node;
          root_ = node;
          root_->parent = nullptr;
        } else {
          node->left = tmp->right;
          if (tmp->right != nullptr) {
            tmp->right->parent = node;
          }
          tmp->right = node;
          node->parent = tmp;
        }
      }
      previous_node_ = node;
    } else {
      root_ = node;
      previous_node_ = root_;
    }
  }
  ~LineTreap() { Suicide(root_); }
  void PrintTreap() { PrintTreap(root_); }

 private:
  Node* root_;
  Node* previous_node_;
  void PrintNode(Node*& node) {
    int par, l_kid, r_kid;
    par = ((node->parent == nullptr) ? 0 : node->parent->key);
    l_kid = ((node->left == nullptr) ? 0 : node->left->key);
    r_kid = ((node->right == nullptr) ? 0 : node->right->key);
    std::cout << par << ' ' << l_kid << ' ' << r_kid << '\n';
  }
  void PrintTreap(Node* node) {
    if (node == nullptr) {
      return;
    }
    PrintTreap(node->left);
    PrintNode(node);
    PrintTreap(node->right);
  }
  void Suicide(Node*& node) {
    if (node == nullptr) {
      return;
    }
    Suicide(node->left);
    Suicide(node->right);
    if (node->left == nullptr && node->right == nullptr) {
      delete node;
      node = nullptr;
      return;
    }
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);
  LineTreap line_treap;
  uint32_t size = 0;
  std::cin >> size;
  int key = 0, prior = 0;
  for (uint32_t i = 0; i < size; ++i) {
    std::cin >> key >> prior;
    line_treap.Insert(i + 1, prior);
  }
  std::cout << "YES\n";
  line_treap.PrintTreap();
}