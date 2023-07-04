#include <cstddef>
#include <cstdint>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>

class Solution {
  struct Graph {
    struct Edge {
      std::pair<size_t, size_t> edge;
      int64_t cap = 0;
      int64_t thd = 0;
      Edge(const std::pair<size_t, size_t>& par, const int64_t& cpy,
           const int64_t& thr)
          : edge(par), cap(cpy), thd(thr) {}
    };
    Graph() : edge_list() {}
    std::pair<size_t, size_t> PlusOneEdge(const std::pair<size_t, size_t>& edge,
                                          const int64_t& cap) {
      edge_list.emplace_back(edge, cap, 0);
      edge_list.emplace_back(std::pair<size_t, size_t>(edge.second, edge.first),
                             0, 0);
      return {edge_list.size() - 2, edge_list.size() - 1};
    }
    int64_t& get_thd(const size_t& idx) { return edge_list[idx].thd; }
    int64_t& get_cap(const size_t& idx) { return edge_list[idx].cap; }
    size_t& get_start(const size_t& idx) { return edge_list[idx].edge.first; }
    size_t& get_finish(const size_t& idx) { return edge_list[idx].edge.second; }

   private:
    std::vector<Edge> edge_list;
  };
  Graph graph_body_;
  const size_t kNumOfVertexes = 0;
  const size_t kNumOfEdges = 0;
  std::vector<int> arr1;
  std::vector<int> arr2;
  const size_t kSource = 0;
  const size_t kOutfall = 0;
  std::vector<std::vector<size_t>> arr4;

 public:
  Solution(const size_t& num_of_vertexes, const size_t& num_of_edges,
           const size_t& source, const size_t& outfall)
      : graph_body_(),
        kNumOfVertexes(num_of_vertexes),
        kNumOfEdges(num_of_edges),
        arr1(kNumOfVertexes),
        arr2(kNumOfVertexes),
        kSource(source),
        kOutfall(outfall),
        arr4(kNumOfVertexes) {}
  void SetEdge(const size_t& start, const size_t& finish,
               const int64_t& weight) {
    std::pair<size_t, size_t> res =
        graph_body_.PlusOneEdge({start, finish}, weight);
    arr4[start].emplace_back(res.first);
    arr4[finish].emplace_back(res.second);
  }
  void Dinic() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    int64_t thd = 0;
    bool res_bfs = true;
    int64_t res_dfs = 0;
    while (true) {
      BFS(res_bfs, kSource, kOutfall);
      if (!res_bfs) {
        break;
      }
      SetVal(arr2, 0);
      for (; true;) {
        DFS(res_dfs, kSource, INT64_MAX);
        if (res_dfs == 0) {
          break;
        }
        thd += res_dfs;
      }
    }
    std::cout << thd << '\n';
    for (size_t i = 0; i < kNumOfEdges * 2; i += 2) {
      std::cout << graph_body_.get_thd(i) << '\n';
    }
  }

 private:
  static void SetVal(std::vector<int>& arr, const int& val) {
    for (int& i : arr) {
      i = val;
    }
  }
  void Shiiiiiish(const unsigned long& idx, std::queue<uint64_t>& qee) {
    while (arr1[graph_body_.get_finish(idx)] == INT32_MAX &&
           graph_body_.get_thd(idx) < graph_body_.get_cap(idx)) {
      qee.emplace(graph_body_.get_finish(idx));
      arr1[graph_body_.get_finish(idx)] = arr1[graph_body_.get_start(idx)] + 1;
      break;
    }
  }
  static uint64_t GetAndPop(std::queue<uint64_t>& qee) {
    uint64_t tmp = qee.front();
    qee.pop();
    return tmp;
  }
  void BFS(bool& norm, const size_t& start, const size_t& finish) {
    SetVal(arr1, INT32_MAX);
    arr1[start] = 0;
    std::queue<uint64_t> qee;
    qee.emplace(start);
    for (; !qee.empty() && arr1[finish] == INT32_MAX;) {
      for (const unsigned long& j : arr4[GetAndPop(qee)]) {
        Shiiiiiish(j, qee);
      }
    }
    norm = arr1[finish] != INT32_MAX;
  }

  void EndDfs(int64_t& res, int64_t& res1, const size_t& cur_node,
              const size_t& idx) {
    graph_body_.get_thd(arr4[cur_node][idx]) += res1;
    graph_body_.get_thd((arr4[cur_node][idx]) ^ 1) -= res1;
    res = res1;
  }
  void JumpIntoDfs(int64_t thd, size_t cur_node, size_t idx, int64_t& res1) {
    size_t min_of_two =
        std::min(thd, graph_body_.get_cap(arr4[cur_node][idx]) -
                          graph_body_.get_thd(arr4[cur_node][idx]));
    DFS(res1, graph_body_.get_finish(arr4[cur_node][idx]), min_of_two);
  }
  bool MakeContinue(const size_t& idx, const size_t& cur_node) {
    return (graph_body_.get_cap(arr4[cur_node][idx]) -
                graph_body_.get_thd(arr4[cur_node][idx]) ==
            0) or
           (arr1[graph_body_.get_finish(arr4[cur_node][idx])] !=
            arr1[cur_node] + 1);
  }
  void DFS(int64_t& res, size_t cur_node, int64_t thd) {
    if (thd <= 0 or cur_node == kOutfall) {
      res = (thd <= 0) ? 0 : thd;
      return;
    }
    size_t idx = arr2[cur_node];
    int64_t res1 = 0;
    while (idx < arr4[cur_node].size()) {
      if (MakeContinue(idx, cur_node)) {
        ++arr2[cur_node];
        ++idx;
        continue;
      }
      JumpIntoDfs(thd, cur_node, idx, res1);
      if (res1 > 0) {
        EndDfs(res, res1, cur_node, idx);
        return;
      }
      ++idx;
    }
    res = 0;
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
    sol.SetEdge(start - 1, finish - 1, weight);
  }
  sol.Dinic();
}