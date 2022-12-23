#include <iostream>

class BST {
  struct Node {
    int data;
    Node* left;
    Node* right;
    int height;
  };
  Node* root;
  void MakeEmpty(Node* t) {
    if (t == nullptr) {
      return;
    }
    MakeEmpty(t->left);
    MakeEmpty(t->right);
    delete t;
  }
  Node* insert(int x, Node* t) {
    if (t == nullptr) {
      t = new Node;
      t->data = x;
      t->height = 0;
      t->left = t->right = nullptr;
    } else if (x < t->data) {
      t->left = insert(x, t->left);
      if (height(t->left) - height(t->right) == 2) {
        if (x < t->left->data)
          t = SingleRightRotate(t);
        else
          t = doubleRightRotate(t);
      }
    } else if (x > t->data) {
      t->right = insert(x, t->right);
      if (height(t->right) - height(t->left) == 2) {
        if (x > t->right->data)
          t = singleLeftRotate(t);
        else
          t = doubleLeftRotate(t);
      }
    }

    t->height = std::max(height(t->left), height(t->right)) + 1;
    return t;
  }

  Node* SingleRightRotate(Node*& t) {
    Node* u = t->left;
    t->left = u->right;
    u->right = t;
    t->height = std::max(height(t->left), height(t->right)) + 1;
    u->height = std::max(height(u->left), t->height) + 1;
    return u;
  }

  Node* singleLeftRotate(Node*& t) {
    Node* u = t->right;
    t->right = u->left;
    u->left = t;
    t->height = std::max(height(t->left), height(t->right)) + 1;
    u->height = std::max(height(t->right), t->height) + 1;
    return u;
  }

  Node* doubleLeftRotate(Node*& t) {
    t->right = SingleRightRotate(t->right);
    return singleLeftRotate(t);
  }

  Node* doubleRightRotate(Node*& t) {
    t->left = singleLeftRotate(t->left);
    return SingleRightRotate(t);
  }

  Node* findMin(Node* t) {
    if (t == nullptr) {
      return nullptr;
    } else if (t->left == nullptr) {
      return t;
    } else {
      return findMin(t->left);
    }
  }

  Node* findMax(Node* t) {
    if (t == nullptr) {
      return nullptr;
    } else if (t->right == nullptr) {
      return t;
    } else {
      return findMax(t->right);
    }
  }

  Node* remove(int x, Node* t) {
    Node* temp;

    // Element not found
    if (t == nullptr) {
      return nullptr;
    }
    // Searching for element
    else if (x < t->data) {
      t->left = remove(x, t->left);
    }
    else if (x > t->data) {
      t->right = remove(x, t->right);
    }
    // Element found
    // With 2 children
    else if (t->left && t->right) {
      temp = findMin(t->right);
      t->data = temp->data;
      t->right = remove(t->data, t->right);
    }
    // With one or zero child
    else {
      temp = t;
      if (t->left == nullptr)
        t = t->right;
      else if (t->right == nullptr)
        t = t->left;
      delete temp;
    }
    if (t == nullptr) {
      return t;
    }
    t->height = std::max(height(t->left), height(t->right)) + 1;

    // If Node is unbalanced
    // If left Node is deleted, right case
    if (height(t->left) - height(t->right) == 2) {
      // right right case
      if (height(t->left->left) - height(t->left->right) == 1) {
        return singleLeftRotate(t);
      } else {
        return doubleLeftRotate(t);
      }
    }
    // If right Node is deleted, left case
    else if (height(t->right) - height(t->left) == 2) {
      // left left case
      if (height(t->right->right) - height(t->right->left) == 1) {
        return SingleRightRotate(t);
      } else {
        return doubleRightRotate(t);
      }
    }
    return t;
  }

  int height(Node* t) { return (t == nullptr ? -1 : t->height); }

  int getBalance(Node* t) {
    if (t == nullptr) {
      return 0;
    } else {
      return height(t->left) - height(t->right);
    }
  }

  void inorder(Node* t) {
    if (t == nullptr) {
      return;
    }
    inorder(t->left);
    std::cout << t->data << " ";
    inorder(t->right);
  }

  Node* LowerBound(Node* t, int x) const {
    if (t == nullptr) {
      return nullptr;
    }
    if (t->data >= x) {
      if (t->data == x || t->left == nullptr) {
        return t;
      }
      return LowerBound(t->left, x);
    }
    return LowerBound(t->right, x);
  }
 public:
  BST() {
    root = nullptr;
  }

  void insert(int x) {
    root = insert(x, root);
  }

  void remove(int x) {
    root = remove(x, root);
  }

  void display() {
    inorder(root);
    std::cout << std::endl;
  }

  int PrintLB(const int& x) const {
    return (LowerBound(root, x) == nullptr) ? -1 : LowerBound(root, x)->data;
  }
};


int main() {
  BST tree;
  size_t num_of_calls = 0;
  std::cin >> num_of_calls;
  char com;
  int num;
  for (size_t i = 0; i < num_of_calls; ++i) {
    std::cin >> com >> num;
    if (com == '+') {
      tree.insert(num);
    } else {
      std::cout << tree.PrintLB(num);
      std::cout << std::endl;
    }
  }
  tree.display();
}