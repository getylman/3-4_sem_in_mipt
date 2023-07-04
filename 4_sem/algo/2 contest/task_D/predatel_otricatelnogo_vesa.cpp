#include <iostream>
#include <vector>
// 2d
class Solution {
 public:
  static void Answer(
      const size_t& vertexes,
      std::vector<std::pair<std::pair<uint16_t, uint16_t>, int16_t>>& edges) {
    ssize_t* dist = new ssize_t[vertexes + 1];
    const ssize_t kInf = 0xffffffff;
    const ssize_t kK30k = 30000;
    for (size_t i = 1; i <= vertexes; ++i) {
      dist[i] = kInf;
    }
    dist[1] = 0;
    int num = vertexes;
    while (num-- > 0) {
      for (const auto& i : edges) {
        dist[i.first.second] =
            (dist[i.first.first] + i.second < dist[i.first.second])
                ? dist[i.first.first] + (ssize_t)i.second
                : dist[i.first.second];
      }
    }
    num = 0;
    while (0ul + ++num <= vertexes) {
      std::cout << ((dist[num] >= kK30k) ? kK30k : dist[num]) << ' ';
    }
    delete[] dist;
  }
};

int main() {
  size_t vertexes = 0;
  size_t num_of_edges = 0;
  std::cin >> vertexes >> num_of_edges;
  std::vector<std::pair<std::pair<uint16_t, uint16_t>, int16_t>> edges(
      num_of_edges);
  for (size_t i = 0; i < num_of_edges; ++i) {
    std::cin >> edges[i].first.first >> edges[i].first.second >>
        edges[i].second;
  }
  Solution::Answer(vertexes, edges);
} 