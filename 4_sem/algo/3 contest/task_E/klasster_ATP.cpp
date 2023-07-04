#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

class Solution {
 private:
  struct Node {
    uint16_t value = 0;
    std::vector<std::pair<uint16_t, uint16_t>> neighbs;
    bool used = false;
    void AddNeighb(const std::pair<uint16_t, uint16_t>& vert_and_weight) {
      neighbs.push_back(vert_and_weight);
    }
  };
  struct Graph {
    std::vector<Node> adj_list;
    // std::vector<std::pair<std::pair<uint16_t, uint16_t>, uint32_t>> edges_;

    Graph(const uint16_t& num_of_vertexes) : adj_list(num_of_vertexes + 1) {
      for (uint16_t i = 0; i <= num_of_vertexes; ++i) {
        adj_list[i].value = i;
        adj_list[i].used = false;
      }
    }
    void AddEdge(
        const std::pair<std::pair<uint16_t, uint16_t>, uint16_t>& edge) {
      adj_list[edge.first.first].AddNeighb({edge.first.second, edge.second});
      adj_list[edge.first.second].AddNeighb({edge.first.first, edge.second});
    }
    void MakeAllFalse() {
      for (auto& i : adj_list) {
        i.used = false;
      }
    }
  };

  struct DSU {
    uint16_t Get(const uint16_t& vertex) {
      uint16_t node = vertex;
      for (; node != body_[node]; node = body_[node]);
      return body_[vertex] = node;
    }

    uint64_t GetTotalWeight() const { return total_val_; }

    void Union(const std::pair<std::pair<uint16_t, uint16_t>, uint16_t>& unit,
               Graph& graph) {
      std::pair<uint16_t, uint16_t> two = {Get(unit.first.first),
                                           Get(unit.first.second)};
      uint16_t root = 0;
      if (two.first == two.second) {
        return;
      }
      graph.AddEdge(unit);
      root =
          (body_[(rang_[two.first] > rang_[two.second]) ? two.second
                                                        : two.first] =
               (rang_[two.first] > rang_[two.second]) ? two.first : two.second);
      total_val_ += unit.second;
      rang_[root] +=
          static_cast<uint16_t>(rang_[two.first] == rang_[two.second]);
    }
    DSU(const uint16_t& num_of_vertexes)
        : body_(num_of_vertexes + 1), rang_(num_of_vertexes + 1) {
      std::iota(body_.begin(), body_.end(), 0);
    }

   private:
    std::vector<uint32_t> body_;
    std::vector<uint32_t> rang_;
    uint64_t total_val_ = 0;
  };

  void ToSort(
      std::vector<std::pair<std::pair<uint16_t, uint16_t>, uint16_t>>& edges) {
    std::sort(
        edges.begin(), edges.end(),
        [](const std::pair<std::pair<uint16_t, uint16_t>, uint16_t>& left,
           const std::pair<std::pair<uint16_t, uint16_t>, uint16_t>& right) {
          return (left.second == right.second) ? left.first > right.first
                                               : left.second > right.second;
        });
  }

  // fields
  Graph graph_;
  DSU dsu_;
  const size_t kBigNum = SIZE_MAX;
  //

  void DFS(std::pair<Node*, Node*> node_pair,
           size_t& res) {
    graph_.adj_list[node_pair.first->value].used = true;
    if (node_pair.first->value == node_pair.second->value) {
      return;
    }
    size_t res1 = 0;
    for (auto& i : graph_.adj_list[node_pair.first->value].neighbs) {
      if (graph_.adj_list[i.first].used) {
        continue;
      }
      DFS({&graph_.adj_list[i.first], node_pair.second}, 
          res1 = std::min(res, static_cast<size_t>(i.second)));
      if (res1 != 0) {
        res = res1;
        return;
      }
    }
    res = 0;
  }

 public:
  Solution(
      const uint16_t& num_of_vertexes,
      std::vector<std::pair<std::pair<uint16_t, uint16_t>, uint16_t>>& edges)
      : graph_(num_of_vertexes), dsu_(num_of_vertexes) {
    ToSort(edges);
    for (const auto& i : edges) {
      dsu_.Union(i, graph_);
    }
  }
  uint64_t GetAnswer(const std::pair<uint16_t, uint16_t>& vertex_pair) {
    graph_.MakeAllFalse();
    size_t res = kBigNum;
    DFS({&graph_.adj_list[vertex_pair.first],
         &graph_.adj_list[vertex_pair.second]}, res);
    return res;
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  uint16_t num_of_vertexes = 0;
  uint32_t num_of_edges = 0;
  uint32_t num_of_calls = 0;
  std::cin >> num_of_vertexes >> num_of_edges >> num_of_calls;
  std::vector<std::pair<std::pair<uint16_t, uint16_t>, uint16_t>> edges(
      num_of_edges);
  while (num_of_edges-- > 0) {
    std::cin >> edges[edges.size() - num_of_edges - 1].first.first >>
        edges[edges.size() - num_of_edges - 1].first.second >>
        edges[edges.size() - num_of_edges - 1].second;
  }
  Solution solution(num_of_vertexes, edges);
  std::pair<uint16_t, uint16_t> vertex_pair;
  while (num_of_calls-- > 0) {
    std::cin >> vertex_pair.first >> vertex_pair.second;
    std::cout << solution.GetAnswer(vertex_pair) << '\n';
  }
}