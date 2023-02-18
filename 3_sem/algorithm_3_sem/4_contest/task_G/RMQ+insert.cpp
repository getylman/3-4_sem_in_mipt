#include <limits.h>

#include <iostream>

struct Node {
  int64_t key;
  int prior;
  int size;
  int64_t min;
  Node* left;
  Node* right;
  Node(int64_t val) {
    key = min = val;
    prior = rand();
    size = 1;
    left = right = nullptr;
  }
  ~Node() {
    delete left;
    delete right;
  }
};
typedef Node* TreeOfTask;

int GetSize(TreeOfTask tree) {
  if (tree == nullptr) {
    return 0;
  }
  return tree->size;
}

int64_t GetMin(TreeOfTask tree) {
  if (tree != nullptr) {
    return tree->min;
  }
  return INT_MAX;
}

TreeOfTask ToUpDate(TreeOfTask& tree) {
  if (tree != nullptr) {
    tree->size = GetSize(tree->left) + GetSize(tree->right) + 1;
    tree->min = std::min(tree->key, std::min(GetMin(tree->left), GetMin(tree->right)));
  }
  return tree;
}

TreeOfTask ToMerge(TreeOfTask l_node, TreeOfTask r_node) {
  if (l_node == nullptr) {
    return r_node;
  }
  if (r_node == nullptr) {
    return l_node;
  }
  if (l_node->prior > r_node->prior) {
    l_node->right = ToMerge(l_node->right, r_node);
    l_node = ToUpDate(l_node);
    return l_node;
  }

  r_node->left = ToMerge(l_node, r_node->left);
  r_node = ToUpDate(r_node);
  return r_node;
}

void ToSplit(TreeOfTask tree, int x, TreeOfTask& lson, TreeOfTask& rson, int num = 0) {
  if (tree == nullptr) {
    lson = nullptr;
    rson = nullptr;
    return;
  }
  int cur = GetSize(tree->left) + num;
  if (cur <= x) {
    ++cur;
    ToSplit(tree->right, x, tree->right, rson, cur);
    lson = tree;
  } else {
    ToSplit(tree->left, x, lson, tree->left, num);
    rson = tree;
  }
  tree = ToUpDate(tree);
}

void Insert(TreeOfTask& t, int64_t val, int id) {
  TreeOfTask l_node, r_node;
  ToSplit(t, id, l_node, r_node);
  TreeOfTask node = new Node(val);
  t = ToMerge(ToMerge(l_node, node), r_node);
}

int64_t GetAnsw(TreeOfTask& my_tr, size_t lt, size_t rt) {
  TreeOfTask l1_node, r1_node, l2_node, r2_node;
  ToSplit(my_tr, rt - 1, l1_node, r1_node);
  ToSplit(l1_node, lt - 2, l2_node, r2_node);
  r2_node = ToUpDate(r2_node);
  int64_t mn = GetMin(r2_node);
  my_tr = ToMerge(ToMerge(l2_node, r2_node), r1_node);
  return mn;
}

int main() {
  size_t size = 0;
  std::cin >> size;
  TreeOfTask my_tr = nullptr;
  char com;
  size_t lt = 0;
  size_t rt = 0;
  int j = 0;
  int64_t x = 0;
  for (size_t i = 0; i < size; ++i) {
    std::cin >> com;
    if (com == '?') {
      std::cin >> lt >> rt;
      int64_t mn = GetAnsw(my_tr, lt, rt);
      std::cout << mn << '\n';
    } else {
      std::cin >> j >> x;
      Insert(my_tr, x, j - 1);
    }
  }
  delete my_tr;
  return 0;
}