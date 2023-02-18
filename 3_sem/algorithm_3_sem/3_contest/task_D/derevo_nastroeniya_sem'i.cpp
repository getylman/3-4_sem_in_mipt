#include <chrono>
#include <iostream>
#include <random>
struct Node {
  int x_comp = 0;
  int y_comp = 0;
  size_t size = 0;
  Node* left = nullptr;
  Node* right = nullptr;
  Node* parent = nullptr;
  Node(int x_comp, int y_comp, size_t size)
      : x_comp(x_comp), y_comp(y_comp), size(size) {}
};

class Treap {
 public:
  Treap() : root_(nullptr) {}
  bool IsExist(int x_comp) const {
    Node* node = root_;
    while (node != nullptr) {
      if (node->x_comp == x_comp) {
        return true;
      }
      if (node->x_comp > x_comp) {
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
    return false;
  }
  void Insert(const int& x_comp) {
    if (IsExist(x_comp)) {
      return;
    }
    std::pair<Node*, Node*> new_treaps = Split(root_, x_comp);
    Node* lessx_comp = new_treaps.first;
    Node* morex_comp = new_treaps.second;
    unsigned seed1 =
        std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen1(seed1);
    root_ = Merge(Merge(lessx_comp, new Node(x_comp, gen1() % kMod, 1)),
                  morex_comp);
  }
  void Erase(int x_comp) {
    std::pair<Node*, Node*> splited_subtrees = Split(root_, x_comp);
    std::pair<Node*, Node*> tmp_node =
        Split(splited_subtrees.second, x_comp + 1);
    delete tmp_node.first;
    root_ = Merge(splited_subtrees.first, tmp_node.second);
  }
  const Node* Next(int x_comp) const {
    Node* res = nullptr;
    Node* cur = root_;
    while (cur != nullptr) {
      if (cur->x_comp > x_comp) {
        if (res == nullptr || res->x_comp > cur->x_comp) {
          res = cur;
        }
        cur = cur->left;
      } else {
        cur = cur->right;
      }
    }
    return res;
  }
  const Node* Prev(int x_comp) const {
    Node* res = nullptr;
    Node* cur = root_;
    while (cur != nullptr) {
      if (cur->x_comp < x_comp) {
        if (res == nullptr || res->x_comp < cur->x_comp) {
          res = cur;
        }
        cur = cur->right;
      } else {
        cur = cur->left;
      }
    }
    return res;
  }
  const Node* Kth(int elem) const {
    Node* cur = root_;
    size_t size = (cur->left != nullptr) ? cur->left->size : 0;
    while (cur != nullptr) {
      if ((int)size < elem) {
        elem -= size + 1;
        cur = cur->right;
      } else if ((int)size > elem) {
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
  Node* Find(Node* node, int x_comp) {
    if (node != nullptr) {
      if (node->x_comp == x_comp) {
        return node;
      }
      if (node->x_comp > x_comp) {
        return Find(node->left, x_comp);
      }
      return Find(node->right, x_comp);
    }
    return nullptr;
  }

  std::pair<Node*, Node*> Split(Node* node, int x_comp) {
    if (node == nullptr) {
      return {nullptr, nullptr};
    }
    if (node->x_comp < x_comp) {
      std::pair<Node*, Node*> splited_nodes = Split(node->right, x_comp);
      node->right = splited_nodes.first;
      if (splited_nodes.first != nullptr) {
        splited_nodes.first->parent = node;
      }
      if (splited_nodes.second != nullptr) {
        splited_nodes.second->parent = nullptr;
      }
      if (node != nullptr) {
        size_t size = 1;
        size += (node->left != nullptr) ? node->left->size : 0;
        size += (node->right != nullptr) ? node->right->size : 0;
        node->size = size;
      }
      return {node, splited_nodes.second};
    }
    std::pair<Node*, Node*> splited_nodes = Split(node->left, x_comp);
    node->left = splited_nodes.second;
    if (splited_nodes.second != nullptr) {
      splited_nodes.second->parent = node;
    }
    if (splited_nodes.first != nullptr) {
      splited_nodes.first->parent = nullptr;
    }
    if (node != nullptr) {
      size_t size = 1;
      size += (node->left != nullptr) ? node->left->size : 0;
      size += (node->right != nullptr) ? node->right->size : 0;
      node->size = size;
    }
    return {splited_nodes.first, node};
  }

  Node* Merge(Node* node_lt, Node* node_rt) {
    if (node_lt == nullptr) {
      return node_rt;
    }
    if (node_rt == nullptr) {
      return node_lt;
    }
    if (node_lt->y_comp < node_rt->y_comp) {
      node_lt->right = Merge(node_lt->right, node_rt);
      node_lt->right->parent = node_lt;
      if (node_lt != nullptr) {
        size_t size = 1;
        size += (node_lt->left != nullptr) ? node_lt->left->size : 0;
        size += (node_lt->right != nullptr) ? node_lt->right->size : 0;
        node_lt->size = size;
      }
      return node_lt;
    }
    node_rt->left = Merge(node_lt, node_rt->left);
    node_rt->left->parent = node_rt;
    if (node_rt != nullptr) {
      size_t size = 1;
      size += (node_rt->left != nullptr) ? node_rt->left->size : 0;
      size += (node_rt->right != nullptr) ? node_rt->right->size : 0;
      node_rt->size = size;
    }
    return node_rt;
  }
};

int main() {
  Treap tree;
  std::string cmnd;
  int x_comp;
  while (std::cin >> cmnd) {
    std::cin >> x_comp;
    if (cmnd == "insert") {
      tree.Insert(x_comp);
    } else if (cmnd == "delete") {
      tree.Erase(x_comp);
    } else if (cmnd == "ex_compists") {
      if (tree.IsExist(x_comp)) {
        std::cout << "true\n";
      } else {
        std::cout << "false\n";
      }
    } else if (cmnd == "nex_compt") {
      const Node* k_nex_compt = tree.Next(x_comp);
      if (k_nex_compt != nullptr) {
        std::cout << k_nex_compt->x_comp << '\n';
      } else {
        std::cout << "none\n";
      }
    } else if (cmnd == "prev") {
      const Node* k_prev = tree.Prev(x_comp);
      if (k_prev != nullptr) {
        std::cout << k_prev->x_comp << '\n';
      } else {
        std::cout << "none\n";
      }
    } else if (cmnd == "kth") {
      const Node* k_kth = tree.Kth(x_comp);
      if (k_kth != nullptr) {
        std::cout << k_kth->x_comp << '\n';
      } else {
        std::cout << "none\n";
      }
    }
  }
}


// work variant

/*
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

*/