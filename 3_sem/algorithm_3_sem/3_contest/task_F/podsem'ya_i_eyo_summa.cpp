#include <iostream>

class MyTreap {
 public:
  int64_t key;
  int64_t prior;
  int64_t sum;
  MyTreap* left;
  MyTreap* right;

  MyTreap(int64_t key, int64_t prior)
      : key(key), prior(prior), sum(0), left(nullptr), right(nullptr) {}
};

int64_t Sum(const MyTreap* t) { return t != nullptr ? t->sum : 0; }

void UpdSum(MyTreap*& t) {
  if (t != nullptr) {
    t->sum = t->key + Sum(t->left) + Sum(t->right);
  }
}

void Merge(MyTreap*& t, MyTreap* left_t, MyTreap* right_t) {
  if (left_t == nullptr || right_t == nullptr) {
    t = left_t != nullptr ? left_t : right_t;
  } else if (left_t->prior > right_t->prior) {
    Merge(left_t->right, left_t->right, right_t);
    t = left_t;
  } else {
    Merge(right_t->left, left_t, right_t->left);
    t = right_t;
  }
  UpdSum(t);
}

void Split(MyTreap* t, const int64_t& x, MyTreap*& new_left,
           MyTreap*& new_right) {
  if (t == nullptr) {
    new_left = new_right = nullptr;
  } else if (t->key > x) {
    Split(t->left, x, new_left, t->left);
    new_right = t;
  } else {
    Split(t->right, x, t->right, new_right);
    new_left = t;
  }
  UpdSum(t);
}

void Insert(MyTreap*& t, MyTreap* new_el) {
  if (t == nullptr) {
    t = new_el;
  } else if (new_el->prior > t->prior) {
    Split(t, new_el->key, new_el->left, new_el->right);
    t = new_el;
  } else {
    Insert(new_el->key < t->key ? t->left : t->right, new_el);
  }
  UpdSum(t);
}

int64_t GetSum(MyTreap* t, int64_t l, int64_t r) {
  MyTreap* left_t = nullptr;
  MyTreap* mid_t = nullptr;
  MyTreap* right_t = nullptr;
  Split(t, l - 1, left_t, t);
  Split(t, r, mid_t, right_t);
  int64_t ans = Sum(mid_t);
  Merge(t, left_t, mid_t);
  Merge(t, t, right_t);
  return ans;
}

bool IsExists(MyTreap* t, int64_t x) {
  if (t != nullptr) {
    if (t->key == x) {
      return true;
    }
    return IsExists(x < t->key ? t->left : t->right, x);
  }
  return false;
}

void Suicide(MyTreap*& t) {
  if (t == nullptr) {
    return;
  }
  Suicide(t->left);
  Suicide(t->right);
  delete t;
}

int main() {
  MyTreap* t = nullptr;
  const int64_t kInf = 1e9;
  size_t n;
  std::cin >> n;
  int64_t ans;
  char previous = '+';
  char x;
  int64_t a;
  int64_t l;
  int64_t r;
  for (size_t i = 0; i < n; i++) {
    std::cin >> x;
    if (x == '+') {
      std::cin >> a;
      a = previous == '+' ? a : (a + ans) % kInf;
      if (!IsExists(t, a)) {
        MyTreap* new_el = new MyTreap(a, rand() % kInf);
        Insert(t, new_el);
      }
    } else {
      std::cin >> l >> r;
      ans = GetSum(t, l, r);
      std::cout << ans << "\n";
    }
    previous = x;
  }
  Suicide(t);
}