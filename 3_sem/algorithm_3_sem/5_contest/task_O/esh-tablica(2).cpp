// 76367158
#include <iostream>
#include <list>
#include <vector>

class HashTable {
 public:
  HashTable() { body_.resize(kBase, std::list<size_t>()); }
  bool Exist(const size_t& elem) {
    const size_t kId = HashFunction(elem);
    return !(IsExist(elem) == body_[kId].end());
  }
  void Insert(const size_t& elem) {
    const size_t kId = HashFunction(elem);
    if (IsExist(elem) == body_[kId].end()) {
      body_[kId].push_back(elem);
    }
  }
  void Erase(const size_t& elem) {
    const size_t kId = HashFunction(elem);
    std::list<size_t>::iterator itr = IsExist(elem);
    if (itr != body_[kId].end()) {
      body_[kId].erase(itr);
    }
  }

 private:
  const size_t kBase = 100000;
  std::vector<std::list<size_t>> body_;
  size_t HashFunction(const size_t& elem) const {
    return ((elem % kBase) + (elem / kBase)) % kBase;
  }
  std::list<size_t>::iterator IsExist(const size_t& elem) {
    const size_t kId = HashFunction(elem);
    std::list<size_t>::iterator itr = body_[kId].begin();
    for (; itr != body_[kId].end(); ++itr) {
      if (*itr == elem) {
        break;
      }
    }
    return itr;
  }
};

int main() {
  size_t num_of_calls = 0;
  std::cin >> num_of_calls;
  HashTable htl;
  char com;
  size_t elem = 0;
  for (size_t i = 0; i < num_of_calls; ++i) {
    std::cin >> com >> elem;
    if (com == '?') {
      std::cout << ((htl.Exist(elem)) ? "YES\n" : "NO\n");
    } else if (com == '-') {
      htl.Erase(elem);
    } else {
      htl.Insert(elem);
    }
  }
}
