#include <iostream>

struct Node {
  Node *l, *r, *p;
  int d;
};

struct SplayTree {
  Node *root;
};

SplayTree *CreateTree() {
  SplayTree *tree = new SplayTree;
  tree->root = nullptr;
  return tree;
}

Node *Create(int d, Node *l = nullptr, Node *r = nullptr, Node *p = nullptr) {
  Node *ret = new Node;
  ret->p = p;
  ret->l = l;
  ret->r = r;
  ret->d = d;
  return ret;
}

void Delete(Node *n) {
  if (n != nullptr) {
    Delete(n->l);
    Delete(n->r);
    delete n;
  }
}

void DeleteTree(SplayTree *tree) {
  Delete(tree->root);
  delete tree;
}

inline void SetParent(Node *child, Node *parent) {
  if (child != nullptr) {
    child->p = parent;
  }
}

inline void ChildrenFix(Node *node) {
  SetParent(node->l, node);
  SetParent(node->r, node);
}

void RLRotate(Node *p, Node *c) {
  // Grandparent fix
  Node *g_p = p->p;
  if (g_p != nullptr) {  //  Если у p есть родитель
    if (g_p->l == p) {   //  Если родитель это левая дочь бабушка то изменяем дочь бабушки на внучку
      g_p->l = c;
    } else {             //  Иначе правая дочь бабушки теперь внучка.
      g_p->r = c;
    }
  }
  if (p->l == c) {       //  Если дочь это левая-дочь матери то левая дочь матери теперь правые потомки дочки
    p->l = c->r;
    c->r = p;
  } else {
    p->r = c->l;
    c->l = p;
  }
  ChildrenFix(c);
  ChildrenFix(p);
  c->p = g_p;
}

/// Вытягивание вершины вверх. Возвращает вершину дерева
Node *Splay(Node *n) {
  if (n->p == nullptr) {
    return n;
  }
  Node *p = n->p;
  Node *g_p = p->p;
  if (g_p == nullptr) {
    RLRotate(p, n);
    return n;
  } else {
    bool zigzag = (g_p->l == p) == (p->l == n);
    if (zigzag) {
      RLRotate(g_p, p);
      RLRotate(p, n);
    } else {
      RLRotate(p, n);
      RLRotate(g_p, n);
    }
    return Splay(n);
  }
}

/// Вытягивание вершины по ключу. Возвращает вершину.
Node *Find(Node *n, int d) {
  if (n == nullptr) {
    return nullptr; // Пустое дерево
  }
  if (n->d == d) {
    return Splay(n);
  }
  if (d < n->d && n->l != nullptr) {
    return Find(n->l, d);
  }
  if (d > n->d && n->r != nullptr) {
    return Find(n->r, d);
  }
  return Splay(n);
}

/// Режем дерево на 2. В левом все элементы меньше d, в правом больше
std::pair<Node*, Node*> Split(Node *root, int d) {
  if (root == nullptr) {
    return {nullptr, nullptr}; // Пустое дерево
  }
  root = Find(root, d); // root ИЗМЕНИЛСЯ!
  if (root->d == d) {
    SetParent(root->l, nullptr);
    SetParent(root->r, nullptr);
    delete root;
    return {root->l, root->r};
  }
  if (root->d < d) {
    Node *r = root->r;
    root->r = nullptr;
    SetParent(r, nullptr);
    return {root, root->r};
  }
  // root->d > d
  Node *l = root->l;
  root->l = nullptr;
  SetParent(l, nullptr);
  return {root->l, root};
}
//  PreorderWalk
void FancyWalk(Node *root) {
  if (root != nullptr) {
    std::cout << root->d << " : {";
    FancyWalk(root->l);
    std::cout << ", ";
    FancyWalk(root->r);
    std::cout << "}";
  } else {
    std::cout << "*";
  }
}

Node *Insert(Node *root, int d) {
  std::pair<Node*, Node*> p = Split(root, d);
  FancyWalk(p.first);
  std::cout << std::endl;
  FancyWalk(p.second);
  std::cout << std::endl;
  Node *last_root = root;
  root = Create(d);
  root->l = p.first;
  root->r = p.second;
  if (p.second == nullptr && last_root->d > root->d) {
    std::cout << last_root->d << " " << root->d;
    SetParent(last_root, root);
    root->r = last_root;
  }
  if (p.first == nullptr && last_root->d < root->d) {
    std::cout << last_root->d << " " << root->d;
    SetParent(last_root, root);
    root->l == last_root;
  }
  ChildrenFix(root);
  return root;
}

void Add(SplayTree *tree, int d) {
  if (tree->root == nullptr) {
    tree->root = Create(d);
    return;
  }
  tree->root = Insert(tree->root, d);
}

/// Для корректной работы, ключи дерева l должны быть меньше ключей дерева r
Node *Merge(Node *r, Node* l) {
  if (r == nullptr) {
    return l;
  }
  if (l == nullptr) {
    return r;
  }
  r = Find(r, l->d);
  r->l = l;
  l->p = r;
  return r;
}

Node *Remove(Node *root, int d) {
  root = Find(root, d);
  Node *r = root->r;
  Node *l = root->l;
  delete root;
  SetParent(r, nullptr);
  SetParent(l, nullptr);
  return Merge(l, r);
}

void MinWithParametr(SplayTree *tree, int i) {
  std::cout << Find(tree->root, i)->d;
}

int main() {
  int k;
  std::cin >> k;
  bool after_q = false;
  int y;
  SplayTree *tree = CreateTree();
  for (int j = 0; j < k; j++) {
    char c;
    std::cin >> c;
    if (c == '+') {
      int i;
      std::cin >> i;
      Add(tree, i);
    }
    if (c == '?') {
      int i;
      std::cin >> i;
      MinWithParametr(tree, i);
    }
    FancyWalk(tree->root);
  }
  std::cout << std::endl;
  DeleteTree(tree);
  return 0;
}