#include <iostream>

class SplayTree {
 public:
  struct Node {
    std::pair<std::string, std::string> val;
    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr;
  };
  SplayTree() { root = nullptr; }
  void Insert(const std::pair<std::string, std::string>& elem) {
    if (root == nullptr) {std::cout << "voshel1\n";
      root = new Node;
      root->val = elem;
      root->left = root->right = root->parent = nullptr;
      return;//std::cout << "voshel 1\n";
    }
    Node* cur = root;std::cout << "voshel 2\n";std::cout << "\t" << elem.first << ' ' << elem.second << std::endl;
    Node* node = new Node;
    node->val = elem;
    node->left = node->right = node->parent = nullptr;
    while (cur != nullptr) {
      if (cur->val.first < elem.first) {
        if (cur->right == nullptr) {
          node->parent = cur;
          std::cout << "Left son " << cur->left << " \tRight son " << cur->right << std::endl;
          cur->right = node;std::cout << cur->right->val.first << ' ' << cur->right->val.second << "+++\n";
          std::cout << "Left son " << cur->left << " \tRight son " << cur->right << std::endl;
          cur = node;
          break;
        }
        cur = cur->right;
      } else {
        if (cur->left == nullptr) {
          node->parent = cur;
          std::cout << "Left son " << cur->left << " \tRight son " << cur->right << std::endl;
          cur->left = node;std::cout << cur->left->val.first << ' ' << cur->left->val.second << "+++\n";
          std::cout << "Left son " << cur->left << " \tRight son " << cur->right << std::endl;
          cur = node;
          break;
        }
        cur = cur->left;
      }
    }PrintTree();
    Splay(cur);PrintTree();
  }
  Node* Find(const std::string& elem) {
    Node* cur = root;
    while ((cur != nullptr) && cur->val.first == elem) {
      if (cur->val.first < elem) {
        cur = cur->right;
      } else {
        cur = cur->left;
      }
    }
    if (cur != nullptr) {
      Splay(cur);
    }
    return root;
  }
  std::string FindVar(const std::string& elem) {
    Find(elem);
    return root->val.second;
  }
  //void Erase(const T& elem);
  void PrintTree() {
    std::cout << "___\n";
    InOrderPrint(root);
    std::cout << "---\n";
  }
  ~SplayTree() { Suicide(root); }
 private:
  Node* root = nullptr;
  Node* RRot(Node* node) {
    Node* cur = node->parent;
    cur->left = node->right;
    //cur->left->parent = node;
    node->right = cur;
    //cur->parent = node;
    return node;
  }
  Node* LRot(Node* node) {std::cout << "***\n";//std::cout << "cur : ";PrintNode(cur);std::cout << "node->parent : ";PrintNode(node->parent);
    Node* cur = node->parent;std::cout << "cur : ";PrintNode(cur);std::cout << "node->parent : ";PrintNode(node->parent);std::cout << "cur->right : ";PrintNode(cur->right);std::cout << "node->left : ";PrintNode(node->left);
    Node* tmp = node->left;
    node->left = cur;
    cur->right = tmp;// проблема в том что во время вращения делает оборт бляяя
    std::cout << "cur->right : ";PrintNode(cur->right);std::cout << "node->left : ";PrintNode(node->left);std::cout << "node->left : ";PrintNode(node->left);std::cout << "cur : ";PrintNode(cur);//std::cout << "***\n";
    
    std::cout << "node->left : ";PrintNode(node->left);std::cout << "cur : ";PrintNode(cur);std::cout << "***\n";
    return node;
  }
  void Splay(Node* node) {
    if (node->parent == nullptr) {
      return;
    }
    while (node->parent != nullptr && node->parent->parent != nullptr) {
      if (node->parent->right == node) {
        if (node->parent->parent->right == node->parent) {
          node->parent = LRot(node->parent);
          node = LRot(node);
        } else {
          node = LRot(node);
          node = RRot(node);
        }
      } else {
        if (node->parent->parent->right == node->parent) {
          node = RRot(node);
          node = LRot(node);
        } else {
          node->parent = RRot(node->parent);
          node = RRot(node);
        }
      }
    }
    if (node->parent != nullptr) {
      if (node->parent->right == node) {
        LRot(node);
      } else {
        RRot(node);
      }
    }
  }
  //Node* Merge(const T& lt, const T& rt);
  //std::pair<Node*, Node*> Split(const T& elem) {
  //  Find(elem);
  //  Node* left = root->left;
  //  root->left = nullptr;
  //  return {left, root};
  //}
  void Suicide(Node*& node) {
    if (node == nullptr) {
      return;
    }
    Suicide(node->left);
    Suicide(node->right);
    if (node->left == nullptr && node->right == nullptr) {
      delete node;
      node->parent = nullptr;
      node = nullptr;
    }
  }
  void InOrderPrint(Node* node) {
    if (node == nullptr) {
      return;
    }
    InOrderPrint(node->left);
    std::cout << node->val.first << ' ' << node->val.second << std::endl;
    InOrderPrint(node->right);
  }
  void PrintNode(Node* node) {
    std::string par, l_kid, r_kid;
    if (node == nullptr) {
      std::cout << "noup noup noup nullptr\n";
      return;
    }
    par = ((node->parent == nullptr) ? "nuop" : node->parent->val.first);
    l_kid = ((node->left == nullptr) ? "noup" : node->left->val.first);
    r_kid = ((node->right == nullptr) ? "noup" : node->right->val.first);
    std::cout << par << ' ' << l_kid << ' ' << r_kid << '\n';
  }
};


int main() {
  SplayTree my_tree;
  size_t num_of_calls;
  std::cin >> num_of_calls;
  std::string ft, sd;
  for (size_t i = 0; i < num_of_calls; ++i) {
    std::cin >> ft >> sd;std::cout << "Inserted " << ft << ' ' << sd << "\n";
    my_tree.Insert({ft, sd});std::cout << "Inserted " << sd << ' ' << ft << "\n";
    my_tree.Insert({sd, ft});
  }
  my_tree.PrintTree();/*
  std::cin >> num_of_calls;
  for (size_t i = 0; i < num_of_calls; ++i) {
    std::cin >> ft;
    std::cout << my_tree.FindVar(ft) << std::endl;
  }*/
}