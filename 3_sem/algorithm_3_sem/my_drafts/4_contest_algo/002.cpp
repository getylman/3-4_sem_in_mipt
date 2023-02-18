#include <iostream>
struct Node {
  int x = 0;
  int y = 0;
  size_t size = 0;
  Node* left = nullptr;
  Node* right = nullptr;
  Node* parent = nullptr;
  Node(int x, int y, size_t size) : x(x), y(y), size(size) {}
};

class Treap {
 public:
  Treap() : root_(nullptr) {}
  bool IsExist(int x) const {
    Node* node = root_;
    while (true) {
      if (node == nullptr) {
        return false;
      }
      if (node->x == x) {
        return true;
      }
      if (node->x > x) {
        if (node->left == nullptr) {
          return false;
        }
        node = node->left;
      } else {
        if (node->right == nullptr) {
          return false;
        }
        node = node->right;
      }
    }
  }
  void Insert(const int& x) {
    if (IsExist(x)) {
      return;
    }
    std::pair<Node*, Node*> tt = Split(root_, x);
    Node* lessx = tt.first;
    Node* morex = tt.second;
    root_ = Merge(Merge(lessx, new Node(x, rand() % kMod, 1)), morex);
  }
  void Erase(int x) {
    std::pair<Node*, Node*> tl = Split(root_, x);
    std::pair<Node*, Node*> tm = Split(tl.second, x + 1);
    delete tm.first;
    root_ = Merge(tl.first, tm.second);
  }
  const Node* Next(int x) const {
    Node* res = nullptr;
    Node* cur = root_;
    while (cur != nullptr) {
      if (cur->x > x) {
        if (res == nullptr || res->x > cur->x) {
          res = cur;
        }
        cur = cur->left;
      } else {
        cur = cur->right;
      }
    }
    return res;
  }
  const Node* Prev(int x) const {
    Node* res = nullptr;
    Node* cur = root_;
    while (cur != nullptr) {
      if (cur->x < x) {
        if (res == nullptr || res->x < cur->x) {
          res = cur;
        }
        cur = cur->right;
      } else {
        cur = cur->left;
      }
    }
    return res;
  }
  const Node* Kth(int k) const {
    Node* cur = root_;
    size_t sz = (cur->left != nullptr) ? cur->left->size : 0;
    while (cur != nullptr) {
      if (sz < k) {
        k -= sz + 1;
        cur = cur->right;
      } else if (sz > k) {
        cur = cur->left;
      } else {
        return cur;
      }
    }
    return nullptr;
  }
  ~Treap() { Delete(root_); }
 private:
  const size_t kMod = 10000000;
  Node* root_;
  void Delete(Node* node) {
    if (node != nullptr) {
      Delete(node->left);
      Delete(node->right);
      delete node;
    }
  }
  Node* Find(Node* node, int x) {
    if (node != nullptr) {
      if (node->x == x) {
        return node;
      }
      if (node->x > x) {
        return Find(node->left, x);
      }
      return Find(node->right, x);
    }
    return nullptr;
  }

  std::pair<Node*, Node*> Split(Node* t, int x) {
    if (t == nullptr) {
      return {nullptr, nullptr};
    }
    if (t->x < x) {
      std::pair<Node*, Node*> lr = Split(t->right, x);
      t->right = lr.first;
      if (lr.first != nullptr) {
        lr.first->parent = t;
      }
      if (lr.second != nullptr) {
        lr.second->parent = nullptr;
      }
      if (t != nullptr) {
        size_t sz = 1;
        sz += (t->left != nullptr) ? t->left->size : 0;
        sz += (t->right != nullptr) ? t->right->size : 0;
        t->size = sz;
      }
      return {t, lr.second};
    }
    std::pair<Node*, Node*> lr = Split(t->left, x);
    t->left = lr.second;
    if (lr.second != nullptr) {
      lr.second->parent = t;
    }
    if (lr.first != nullptr) {
      lr.first->parent = nullptr;
    }
    if (t != nullptr) {
      size_t sz = 1;
      sz += (t->left != nullptr) ? t->left->size : 0;
      sz += (t->right != nullptr) ? t->right->size : 0;
      t->size = sz;
    }
    return {lr.first, t};
  }

  Node* Merge(Node* t1, Node* t2) {
    if (t1 == nullptr) {
      return t2;
    }
    if (t2 == nullptr) {
      return t1;
    }
    if (t1->y < t2->y) {
      t1->right = Merge(t1->right, t2);
      t1->right->parent = t1;
      if (t1 != nullptr) {
        size_t sz = 1;
        sz += (t1->left != nullptr) ? t1->left->size : 0;
        sz += (t1->right != nullptr) ? t1->right->size : 0;
        t1->size = sz;
      }
      return t1;
    }
    t2->left = Merge(t1, t2->left);
    t2->left->parent = t2;
    if (t2 != nullptr) {
      size_t sz = 1;
      sz += (t2->left != nullptr) ? t2->left->size : 0;
      sz += (t2->right != nullptr) ? t2->right->size : 0;
      t2->size = sz;
    }
    return t2;
  }
};

int main() {
  Treap t;
  std::string cmnd;
  int x;
  while (std::cin >> cmnd) {
    std::cin >> x;
    if (cmnd == "insert") {
      t.Insert(x);
    } else if (cmnd == "delete") {
      t.Erase(x);
    } else if (cmnd == "exists") {
      if (t.IsExist(x)) {
        std::cout << "true\n";
      } else {
        std::cout << "false\n";
      }
    } else if (cmnd == "next") {
      const Node* k_next = t.Next(x);
      if (k_next != nullptr) {
        std::cout << k_next->x << '\n';
      } else {
        std::cout << "none\n";
      }
    } else if (cmnd == "prev") {
      const Node* k_prev = t.Prev(x);
      if (k_prev != nullptr) {
        std::cout << k_prev->x << '\n';
      } else {
        std::cout << "none\n";
      }
    } else if (cmnd == "kth") {
      const Node* k_kth = t.Kth(x);
      if (k_kth != nullptr) {
        std::cout << k_kth->x << '\n';
      } else {
        std::cout << "none\n";
      }
    }
  }
}