// CE
#include <iostream>

class MyTreap {
 public: 
  int64_t key;
  int64_t prior;
  MyTreap* left;
  MyTreap* right;

  MyTreap(int64_t key, int64_t prior) : key(key), prior(prior), left(nullptr), right(nullptr) {}
};

void Split(MyTreap* t, int64_t x, MyTreap*& new_left, MyTreap*& new_right) {
  if (t == nullptr) {
    new_left = new_right = nullptr;
  } else if (t->key > x) {
    Split(t->left, x, new_left, t->left);
    new_right = t;
  } else {
    Split(t->right, x, t->right, new_right);
    new_left = t;
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
}

int64_t LowerBound(MyTreap* t, const int64_t& i) {
  MyTreap* left_t;
  MyTreap* right_t;
  Split(t, i, left_t, right_t);
  int64_t ans;
  if (right_t != nullptr) {
    MyTreap* curr = right_t;
    while (curr != nullptr) {
      if (curr->left == nullptr) {
        break;
      } else {
        curr = curr->left;
      }
    }
    ans = curr->key;
  } else {
    ans = -1;
  }
  Merge(t, left_t, right_t);
  return ans;
}

bool IsExists(MyTreap*& t, int64_t x) {
  if (t == nullptr) {
    return false;
  }
  if (t->key != x) {
    return IsExists(t->key > x ? t->left : t->right, x);
  }
  return true;
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
  MyTreap* t;
  size_t num_of_calls;
  std::cin >> num_of_calls;
  int64_t ans;
  char previous = '+';
  const int64_t kInf = 1e9;
  char x;
  int64_t a;
  for (size_t i = 0; i < num_of_calls; i++) {
    std::cin >> x >> a;
    if (x == '?') {
      ans = IsExists(t, a) ? a : LowerBound(t, a);
      std::cout << ans << "\n";
    } else {
      a = previous == '+' ? a : (a + ans) % kInf;
      if (!IsExists(t, a)) {
        MyTreap* new_el = new MyTreap(a, rand() % kInf);
        Insert(t, new_el);
      }
    }
    previous = x;
  }
  Suicide(t);
}