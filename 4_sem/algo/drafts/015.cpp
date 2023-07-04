#include <iostream>
#include <vector>

class Snm {
 public:
  size_t Get(size_t vertex) {
    if (vertex == parent[vertex - 1]) return vertex;
    return parent[vertex - 1] = Get(parent[vertex - 1]);
  }

  void Union(size_t first, size_t second, size_t power) {
    size_t head_1 = Get(first);
    size_t head_2 = Get(second);
    size_t root;
    if (head_2 != head_1) {
      if (rang[head_1 - 1] > rang[head_2 - 1]) {
        parent[head_2 - 1] = head_1;
        root = head_1;
        friends[head_1 - 1] += friends[head_2 - 1] + power;
      } else {
        parent[head_1 - 1] = head_2;
        root = head_2;
        friends[head_2 - 1] += friends[head_1 - 1] + power;
      }
      if (rang[head_1 - 1] == rang[head_2 - 1]) {
        ++rang[root - 1];
      }
    } else {
      friends[head_1 - 1] += power;
    }
  }

  size_t

  Friends(size_t current) {
    return friends[Get(current) - 1];
  }

  explicit Snm(size_t size) {
    parent.resize(size);
    rang.resize(size, 0);
    friends.resize(size, 0);
    for (size_t i = 0; i < parent.size(); ++i) {
      parent[i] = 1 + i;
    }
  }

 private:
  std::vector<size_t> parent;
  std::vector<size_t> rang;
  std::vector<size_t> friends;
};

int main() {
  // for faster stream work
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  size_t query, m;
  std::cin >> m >> query;
  Snm snm(m);
  for (size_t i = 0; i < query; ++i) {
    size_t command;
    std::cin >> command;
    if (command == 1) {
      size_t first, second, val;
      std::cin >> first >> second >> val;
      snm.Union(first, second, val);
    } else {
      size_t current;
      std::cin >> current;
      std::cout << snm.Friends(snm.Get(current)) << '\n';
    }
  }
}