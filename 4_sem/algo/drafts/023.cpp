#include <cstddef>
#include <cstdint>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>

class Solution {
  struct Graph {
    Graph(const size_t& num_of_edges, const size_t& num_of_vertexes)
        : edge_cap_(num_of_vertexes, std::vector<int64_t>(num_of_vertexes)),
          edge_thd_(num_of_vertexes, std::vector<int64_t>(num_of_vertexes)) {}
    void PlusOneEdge(const std::pair<size_t, size_t>& edge,
                     const int64_t& cap) {
      edge_cap_[edge.first][edge.second] = cap;
    }
    // int64_t& get_thd(const size_t& idx) { return edge_list[idx].thd; }
    // int64_t& get_cap(const size_t& idx) { return edge_list[idx].cap; }
    // size_t& get_start(const size_t& idx) { return edge_list[idx].edge.first;
    // } size_t& get_finish(const size_t& idx) { return
    // edge_list[idx].edge.second; }
    int64_t& get_thd(const size_t& raw, const size_t& col) {
      return edge_thd_[raw][col];
    }
    int64_t& get_cap(const size_t& raw, const size_t& col) {
      return edge_cap_[raw][col];
    }

   private:
    std::vector<std::vector<int64_t>> edge_cap_;
    std::vector<std::vector<int64_t>> edge_thd_;
  };

  Graph graph_body_;
  const size_t kNumOfVertexes = 0;
  const size_t kNumOfEdges = 0;
  const size_t kSource = 0;
  const size_t kOutfall = 0;
  std::vector<int64_t> arr1;  // dist
  std::vector<int64_t> arr2;  // ptr
  std::vector<std::pair<size_t, size_t>> edges_;

  static void SetVal(std::vector<int64_t>& arr, const int64_t& val) {
    std::fill(arr.begin(), arr.end(), val);
  }

  void BFS(bool& res) {
    SetVal(arr1, INT64_MAX);
    arr1[kSource] = 0;
    std::queue<size_t> qee;
    qee.emplace(kSource);
    while (!qee.empty()) {
      size_t cur_wertex = qee.front();
      qee.pop();
      for (size_t i = 0; i < kNumOfVertexes; ++i) {
        if (arr1[i] == INT64_MAX and graph_body_.get_cap(cur_wertex, i) <
                                         graph_body_.get_thd(cur_wertex, i)) {
          qee.emplace(i);
          arr1[i] = arr2[cur_wertex] + 1;
        }
      }
    }
    res = arr1[kOutfall] != INT64_MAX;
    return;
  }

  void DFS(int64_t& res, size_t cur_node, int64_t cur_thd) {
    if (cur_thd <= 0 or cur_node == kOutfall) {
      res = (cur_thd <= 0) ? 0 : cur_thd;
      return;
    }
    for (size_t i = arr2[cur_node]; i < kNumOfVertexes; ++i) {
      if (arr1[i] != arr1[cur_node] + 1) {
        continue;
      }
      int64_t res1 = 0;
      int64_t min_of_two =
          std::min(cur_thd, graph_body_.get_cap(cur_node, i) -
                                graph_body_.get_thd(cur_node, i));
      DFS(res1, cur_node, min_of_two);
      if (res1 != 0) {
        graph_body_.get_thd(cur_node, i) += res1;
        graph_body_.get_thd(i, cur_node) -= res1;
        res = res1;
        return;
      }
    }
    res = 0;
  }

 public:
  Solution(const size_t& num_of_vertexes, const size_t& num_of_edges,
           const size_t& source, const size_t& outfall)
      : graph_body_(num_of_edges, num_of_vertexes),
        kNumOfVertexes(num_of_vertexes),
        kNumOfEdges(num_of_edges),
        kSource(source),
        kOutfall(outfall),
        arr1(kNumOfVertexes),
        arr2(kNumOfVertexes) {}

  void SetNewEdge(const std::pair<size_t, size_t>& edge, const int64_t& cap) {
    graph_body_.PlusOneEdge(edge, cap);
    edges_.push_back(edge);
  }

  void Ans() {
    bool res_bfs = false;
    int64_t res_dfs = 0;
    int64_t max_thd = 0;
    while (true) {
      BFS(res_bfs);
      if (!res_bfs) {
        break;
      }
      DFS(res_dfs, kSource, INT64_MAX);
      while (res_dfs != 0) {
        max_thd += res_dfs;
        DFS(res_dfs, kSource, INT64_MAX);
      }
      std::cout << max_thd << '\n';
    }
    for (const auto& i : edges_) {
      std::cout << graph_body_.get_thd(i.first, i.second) <<'\n';
    }
  }
};

int main() {
  std::ios::sync_with_stdio(0);
  std::cin.tie(0);
  std::cout.tie(0);
  size_t num_of_vertexes = 0;
  size_t num_of_edges = 0;
  std::cin >> num_of_vertexes >> num_of_edges;
  size_t start = 0;
  size_t finish = 0;
  size_t weight = 0;
  Solution sol(num_of_vertexes, num_of_edges, 0, num_of_vertexes - 1);
  while (num_of_edges-- > 0) {
    std::cin >> start >> finish >> weight;
    sol.SetNewEdge({start - 1, finish - 1}, weight);
  }
  sol.Ans();
}