#include <algorithm>
#include <climits>
#include <cstdlib>
#include <iostream>

struct Item {
  long long key;
  int prior;
  int size;
  long long min;
  Item* left;
  Item* right;
  explicit Item(long long key)
      : key(key), prior(rand()), size(1), min(key), left(nullptr), right(nullptr) {}
  ~Item() {
    delete left;
    delete right;
  }
};
typedef Item* PItem;

int GetSize(PItem t) {
  if (t == nullptr) {
    return 0;
  }
  return t->size;
}

long long GetMin(PItem t) {
  if (t != nullptr) {
    return t->min;
  }
  return INT_MAX;
}

PItem Update(PItem& t) {
  if (t == nullptr) {
    return t;
  }
  t->size = GetSize(t->left) + GetSize(t->right) + 1;
  t->min = std::min(std::min(GetMin(t->left), GetMin(t->right)), t->key);
  return t;
}

PItem Merge(PItem l, PItem r) {
  if (l == nullptr) {
    return r;
  }
  if (r == nullptr) {
    return l;
  }
  if (l->prior > r->prior) {
    l->right = Merge(l->right, r);
    l = Update(l);
    return l;
  }

  r->left = Merge(l, r->left);
  r = Update(r);
  return r;
}

void Split(PItem t, int x, PItem& l, PItem& r, int num = 0) {
  if (t == nullptr) {
    l = nullptr;
    r = nullptr;
    return;
  }
  int cur = GetSize(t->left) + num;
  if (cur <= x) {
    cur++;
    Split(t->right, x, t->right, r, cur);
    l = t;
  } else {
    Split(t->left, x, l, t->left, num);
    r = t;
  }
  t = Update(t);
}

void Insert(PItem& t, long long val, int pos) {
  PItem l, r;
  Split(t, pos, l, r);
  PItem v = new Item(val);
  t = Merge(Merge(l, v), r);
}

long long GetAnsw(PItem& tree, int l, int r) {
  PItem l1, r1, l2, r2;
  Split(tree, r - 1, l1, r1);
  Split(l1, l - 2, l2, r2);
  r2 = Update(r2);
  long long min = GetMin(r2);
  tree = Merge(Merge(l2, r2), r1);
  return min;
}

int main() {
  int n = 0;
  std::cin >> n;
  PItem tree = nullptr;
  char command;
  for (int i = 0; i < n; i++) {
    std::cin >> command;
    if (command == '?') {
      int l = 0;
      int r = 0;
      std::cin >> l >> r;
      long long min = GetAnsw(tree, l, r);
      std::cout << min << std::endl;
    }
    if (command == '+') {
      int j = 0;
      long long x = 0;
      std::cin >> j >> x;
      Insert(tree, x, j - 1);
    }
  }
  delete tree;
  return 0;
}
