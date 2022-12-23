#include <iostream>

struct Node {
  int x = 0;
  int y = 0;
  int size = 0;
  Node* left = nullptr;
  Node* right = nullptr;
  Node* parent = nullptr;
};

void Delete(Node* node) {
  if (node != nullptr) {
    Delete(node->left);
    Delete(node->right);
    delete node;
  }
}

int Size(Node* node) { return (node != nullptr) ? node->size : 0; }

void FixNode(Node* node) {
  if (node == nullptr) {
    return;
  }
  node->size = Size(node->left) + Size(node->right) + 1;
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
    FixNode(t);
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
  FixNode(t);
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
    FixNode(t1);
    return t1;
  }
  t2->left = Merge(t1, t2->left);
  t2->left->parent = t2;
  FixNode(t2);
  return t2;
}

struct Treap {
  Node* root = nullptr;

  bool IsExist(int x) const { return (Find(root, x) != nullptr); }

  void Insert(const int& x) {
    if (IsExist(x)) {
      return;
    }
    std::pair<Node*, Node*> tt = Split(root, x);
    Node* lessx = tt.first;
    Node* morex = tt.second;
    root = Merge(Merge(lessx, new Node({x, std::rand() % 10000000, 1})), morex);
  }

  void Erase(int x) {
    std::pair<Node*, Node*> tl = Split(root, x);
    std::pair<Node*, Node*> tm = Split(tl.second, x + 1);
    delete tm.first;
    root = Merge(tl.first, tm.second);
  }

  const Node* Next(int x) const {
    Node* res = nullptr;
    Node* cur = root;
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
    Node* cur = root;
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
    Node* cur = root;
    while (cur != nullptr) {
      if (Size(cur->left) < k) {
        k -= Size(cur->left) + 1;
        cur = cur->right;
      } else if (Size(cur->left) > k) {
        cur = cur->left;
      } else {
        return cur;
      }
    }
    return nullptr;
  }
  ~Treap() { Delete(root); }
};

void InOrderWalk(const Node* n) {
  if (n != nullptr) {
    InOrderWalk(n->left);
    std::cout << n->x << std::endl;
    InOrderWalk(n->right);
  }
}

int main() {
  auto t = Treap();
  std::string cmnd;
  while (std::cin >> cmnd) {
    int x;
    // InOrderWalk(t->root);
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
      if (next != nullptr) {
        std::cout << next->x << '\n';
      } else {
        std::cout << "none\n";
      }
    }
    if (cmnd == "prev") {
      std::cin >> x;
      auto prev = t.Prev(x);
      if (prev != nullptr) {
        std::cout << prev->x << '\n';
      } else {
        std::cout << "none\n";
      }
    }
    if (cmnd == "kth") {
      int k;
      std::cin >> k;
      auto kth = t.Kth(k);
      if (kth != nullptr) {
        std::cout << kth->x << '\n';
      } else {
        std::cout << "none\n";
      }
    }
  }
  return 0;
}
