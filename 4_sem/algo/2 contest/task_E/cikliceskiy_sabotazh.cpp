#include <iostream>
#include <vector>
// 2e
class Solution {
  // const int64_t kInf = 0xeffffff0; 
  struct Node {
    uint16_t value = 0;
    uint16_t parent = (uint16_t)-1;
    int dist = 0;
    std::vector<std::pair<uint16_t, int>> neighbours;
  };
  std::vector<Node> adj_list_;
  std::vector<int64_t> dists_;
  void BuildAdjList(
      const size_t& vertexes,
      std::vector<std::pair<std::pair<uint16_t, uint16_t>, int>>& edges) {
    adj_list_.resize(vertexes + 1);
    for (size_t i = 1; i <= vertexes; ++i) {
      adj_list_[i].value = i;
    }
    for (const auto& i : edges) {
      adj_list_[i.first.first].neighbours.push_back({i.first.second, i.second});
    }
    dists_.resize(vertexes + 1, 0);
  }

 public:
  void Answer(
      const size_t& vertexes,
      std::vector<std::pair<std::pair<uint16_t, uint16_t>, int>>& edges) {
    BuildAdjList(vertexes, edges);
    uint16_t cur_in_loop_finded_situation = 0;
    std::vector<uint16_t> loop;
    uint16_t tmp = 0;
    std::pair<uint16_t, uint16_t> num = {vertexes, vertexes};
    while (num.first-- > 0) {
      tmp = uint16_t(-1);
      for (size_t j = 1; j <= vertexes; ++j) {
        for (const auto& k : adj_list_[j].neighbours) {
          if (dists_[k.first] > dists_[j] + k.second) {
            dists_[k.first] = dists_[j] + k.second;
            adj_list_[k.first].parent = j;
            tmp = k.first;
          }
        }
      }
      if (num.first != 0) {
        continue;
      }
      if (tmp == uint16_t(-1)) {
        std::cout << "NO";
        return;
      }
      std::cout << "YES\n";
      while (num.second-- > 0) {
        tmp = adj_list_[tmp].parent;
      }
      cur_in_loop_finded_situation = tmp;
      while (true) {
        loop.push_back(cur_in_loop_finded_situation);
        if (cur_in_loop_finded_situation == tmp and loop.size() > 1) {
          break;
        }
        cur_in_loop_finded_situation =
            adj_list_[cur_in_loop_finded_situation].parent;
      }
      std::cout << loop.size() << '\n';
      for (size_t j = loop.size() - 1; j < loop.size(); --j) {
        std::cout << loop[j] << ' ';
      }
    }
  }
};

int main() {
  size_t vertexes = 0;
  std::cin >> vertexes;
  int edge = 0;
  const int kNoEdge = 100000;
  std::vector<std::pair<std::pair<uint16_t, uint16_t>, int>> edges;
  for (size_t i = 0; i < vertexes; ++i) {
    for (size_t j = 0; j < vertexes; ++j) {
      std::cin >> edge;
      if (edge < 0 and i == j) {
        std::cout << "YES\n2\n" << i + 1 << ' ' << i + 1;
        return 0;
      }
      if (edge == kNoEdge or i == j) {
        continue;
      }
      edges.push_back({{i + 1, j + 1}, edge});
    }
  }
  Solution().Answer(vertexes, edges);
}