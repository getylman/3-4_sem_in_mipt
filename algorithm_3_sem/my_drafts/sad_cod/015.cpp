#include <iostream>
#include <cstdint>
#include <utility>
#include <string>
#include <cstdlib>
#include <experimental/random>

using Key = int;
using Priority = int;

struct Node {
  Key x_ = 0;
  Priority y_ = 0;
  int size_ = 0;
  Node *left_ = nullptr;
  Node *right_ = nullptr;
  Node *parent_ = nullptr;
};

void Delete(Node *n) {
  if (n != nullptr) {
    Delete(n->left_);
    Delete(n->right_);
    delete n;
  }
}

int Size(Node *node) {
  return node ? node->size_ : 0;
}

void FixNode(Node *node) {
  if (node == nullptr) {
    return;
  }
  node->size_ = Size(node->left_) + Size(node->right_) + 1;
}

Node *Find(Node *node, int x) {
  if (node != nullptr) {
    if (node->x_ == x) {
      return node;
    }
    if (node->x_ > x) {
      return Find(node->left_, x);
    }
    return Find(node->right_, x);
  }
  return nullptr;
}

std::pair<Node *, Node *> Split(Node *t, int x) {
  if (t == nullptr) {
    return {nullptr, nullptr};
  }
  if (t->x_ < x) {
    std::pair<Node *, Node *> lr = Split(t->right_, x);
    t->right_ = lr.first;
    if (lr.first != nullptr) {
      lr.first->parent_ = t;
    }
    if (lr.second != nullptr) {
      lr.second->parent_ = nullptr;
    }
    FixNode(t);
    return {t, lr.second};
  }
  std::pair<Node *, Node *> lr = Split(t->left_, x);
  t->left_ = lr.second;
  if (lr.second != nullptr) {
    lr.second->parent_ = t;
  }
  if (lr.first != nullptr) {
    lr.first->parent_ = nullptr;
  }
  FixNode(t);
  return {lr.first, t};
}

Node *Merge(Node *t1, Node *t2) {
  if (t1 == nullptr) {
    return t2;
  }
  if (t2 == nullptr) {
    return t1;
  }
  if (t1->y_ < t2->y_) {
    t1->right_ = Merge(t1->right_, t2);
    t1->right_->parent_ = t1;
    FixNode(t1);
    return t1;
  }
  t2->left_ = Merge(t1, t2->left_);
  t2->left_->parent_ = t2;
  FixNode(t2);
  return t2;
}

struct Treap {
  Node *root_ = nullptr;

  bool IsExist(int x) const {
    return Find(root_, x);
  }

  void Insert(int x) {
    if (IsExist(x)) {
      return;
    }
    std::pair<Node *, Node *> tt = Split(root_, x);
    Node *lessx = tt.first;
    Node *morex = tt.second;
    root_ = Merge(Merge(lessx, new Node({x, std::rand() % 10000000, 1})), morex);
  }

  void Erase(int x) {
    std::pair<Node *, Node *> tl = Split(root_, x);
    std::pair<Node *, Node *> tm = Split(tl.second, x + 1);
    delete tm.first;
    root_ = Merge(tl.first, tm.second);
  }

  const Node *Next(int x) const {
    Node *res = nullptr;
    Node *cur = root_;
    while (cur != nullptr) {
      if (cur->x_ > x) {
        if (res == nullptr || res->x_ > cur->x_) {
          res = cur;
        }
        cur = cur->left_;
      } else {
        cur = cur->right_;
      }
    }
    return res;
  }

  const Node *Prev(int x) const {
    Node *res = nullptr;
    Node *cur = root_;
    while (cur != nullptr) {
      if (cur->x_ < x) {
        if (res == nullptr || res->x_ < cur->x_) {
          res = cur;
        }
        cur = cur->right_;
      } else {
        cur = cur->left_;
      }
    }
    return res;
  }

  const Node *Kth(int k) const {
    Node *cur = root_;
    while (cur != nullptr) {
      if (Size(cur->left_) < k) {
        k -= Size(cur->left_) + 1;
        cur = cur->right_;
      } else if (Size(cur->left_) > k) {
        cur = cur->left_;
      } else {
        return cur;
      }
    }
    return nullptr;
  }
  ~Treap() {
    Delete(root_);
  }
};

void InOrderWalk(const Node *n) {
  if (n != nullptr) {
    InOrderWalk(n->left_);
    std::cout << n->x_ << std::endl;
    InOrderWalk(n->right_);
  }
}

int main() {
  auto t = Treap();
  std::string cmnd;
  while (std::cin >> cmnd) {
    int x;
    // InOrderWalk(t->root_);
    if (cmnd == "insert") {
      std::cin >> x;
      t.Insert(x);
    }
    if (cmnd == "delete") {
      std::cin >> x;
      t.Erase(x);
    }
    if (cmnd == "exists") {
      std::cin >> x;
      if (t.IsExist(x)) {
        std::cout << "true" << '\n';
      } else {
        std::cout << "false" << '\n';
      }
    }
    if (cmnd == "next") {
      std::cin >> x;
      auto next = t.Next(x);
      if (next) {
        std::cout << next->x_ << '\n';
      } else {
        std::cout << "none\n";
      }
    }
    if (cmnd == "prev") {
      std::cin >> x;
      auto prev = t.Prev(x);
      if (prev) {
        std::cout << prev->x_ << '\n';
      } else {
        std::cout << "none\n";
      }
    }
    if (cmnd == "kth") {
      int k;
      std::cin >> k;
      auto kth = t.Kth(k);
      if (kth) {
        std::cout << kth->x_ << '\n';
      } else {
        std::cout << "none\n";
      }
    }
  }
  return 0;
}