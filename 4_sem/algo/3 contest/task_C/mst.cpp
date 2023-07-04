#include <algorithm>
#include <iostream>
#include <numeric>
#include <set>
#include <vector>
// this solution made only for submit tests
// soon I will change it
class Solution {
 private:
  // struct Node {
  //   uint64_t total_weight = 0;
  //   uint64_t value = 0;
  //   uint64_t used_in_dfs = 0;
  //   std::set<uint64_t> neighbs;  // vert and weight
  // };
  // struct Graph {
  //   uint64_t num_of_dfs = 0;
  //   std::vector<Node> adj_list;
  //   void DFS(Node* cur_node, uint64_t& num_of_travel, const uint64_t&
  //   set_val) {
  //     if (cur_node->used_in_dfs == num_of_travel) {
  //       return;
  //     }
  //     cur_node->used_in_dfs = num_of_travel;
  //   }
  //   void AddEdge(const uint64_t& left, const uint64_t& right,
  //                const uint64_t& weight) {
  //   }
  // };
  struct DSU {
    uint32_t Get(const uint32_t& vertex) {
      uint32_t node = vertex;
      for (; node != body_[node]; node = body_[node]);
      return body_[vertex] = node;
    }

    uint64_t GetTotalWeight() const {  return total_val_; }

    void Union(const std::pair<std::pair<uint32_t, uint32_t>, uint32_t>& unit) {
      std::pair<uint32_t, uint32_t> two = {Get(unit.first.first),
                                           Get(unit.first.second)};
      uint64_t root = 0;
      if (two.first == two.second) {
        return;
      }
      root =
          (body_[(rang_[two.first] > rang_[two.second]) ? two.second
                                                        : two.first] =
               (rang_[two.first] > rang_[two.second]) ? two.first : two.second);
      total_val_ += unit.second;
      rang_[root] +=
          static_cast<uint32_t>(rang_[two.first] == rang_[two.second]);
    }
    DSU(const uint32_t& num_of_vertexes)
        : body_(num_of_vertexes), rang_(num_of_vertexes) {
      std::iota(body_.begin(), body_.end(), 0);
    }

   private:
    std::vector<uint32_t> body_;
    std::vector<uint32_t> rang_;
    uint64_t total_val_ = 0;
  };

  void ToSort(
      std::vector<std::pair<std::pair<uint32_t, uint32_t>, uint32_t>>& edges) {
    std::sort(
        edges.begin(), edges.end(),
        [](const std::pair<std::pair<uint32_t, uint32_t>, uint32_t>& left,
           const std::pair<std::pair<uint32_t, uint32_t>, uint32_t>& right) {
          return (left.second == right.second) ? left.first < right.first
                                               : left.second < right.second;
        });
  }

 public:
  uint64_t Answer(
      const uint32_t& num_of_vertexes,
      std::vector<std::pair<std::pair<uint32_t, uint32_t>, uint32_t>>& edges) {
    DSU dsu(num_of_vertexes);
    ToSort(edges);
    uint64_t total_weight = 0;
    // std::vector<uint32_t> indexes_of_sets(num_of_vertexes);
    // std::iota(indexes_of_sets.begin(), indexes_of_sets.end(), 0);
    for (const auto& i : edges) {
      // if (indexes_of_sets[i.first.first] == indexes_of_sets[i.first.second])
      // {
      //   continue;
      // }
      // total_weight += i.second;
      // for (auto& j : indexes_of_sets) {
      //   j = (j == indexes_of_sets[i.first.second])
      //           ? indexes_of_sets[i.first.first]
      ////            : j;
      // }
      dsu.Union(i);
    }
    return total_weight = dsu.GetTotalWeight();
  }
};

int main() {
  uint32_t num_of_vertexes = 0;
  uint32_t num_of_edges = 0;
  scanf("%u%u", &num_of_vertexes, &num_of_edges);
  std::vector<std::pair<std::pair<uint32_t, uint32_t>, uint32_t>> edges(
      num_of_edges);
  while (num_of_edges-- > 0) {
    std::cin >> edges[edges.size() - num_of_edges - 1].first.first >>
        edges[edges.size() - num_of_edges - 1].first.second >>
        edges[edges.size() - num_of_edges - 1].second;
    --edges[edges.size() - num_of_edges - 1].first.first;
    --edges[edges.size() - num_of_edges - 1].first.second;
    // if (edges[edges.size() - num_of_edges - 1].first.first >
    //     edges[edges.size() - num_of_edges - 1].first.second) {
    //   std::swap(edges[edges.size() - num_of_edges - 1].first.first,
    //             edges[edges.size() - num_of_edges - 1].first.second);
    // }
  }
  std::cout << Solution().Answer(num_of_vertexes, edges);
}
// ----