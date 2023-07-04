#include <iostream>
#include <queue>
#include <cstring>

using namespace std;

const int MAXN = 505;
const int INF = 1e9;

int n, m;
int c[MAXN][MAXN], f[MAXN][MAXN], d[MAXN], ptr[MAXN];
vector<int> g[MAXN];

bool bfs() {
    memset(d, -1, sizeof(d));
    d[1] = 0;
    queue<int> q;
    q.push(1);
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (int i = 0; i < g[v].size(); i++) {
            int to = g[v][i];
            if (d[to] == -1 && f[v][to] < c[v][to]) {
                d[to] = d[v] + 1;
                q.push(to);
            }
        }
    }
    return d[n] != -1;
}

int dfs(int v, int flow) {
    if (v == n || !flow) {
        return flow;
    }
    for (int& i = ptr[v]; i < g[v].size(); i++) {
        int to = g[v][i];
        if (d[to] == d[v] + 1 && f[v][to] < c[v][to]) {
            int delta = dfs(to, min(flow, c[v][to] - f[v][to]));
            if (delta) {
                f[v][to] += delta;
                f[to][v] -= delta;
                return delta;
            }
        }
    }
    return 0;
}

int dinic() {
    int flow = 0;
    while (bfs()) {
        memset(ptr, 0, sizeof(ptr));
        while (int delta = dfs(1, INF)) {
            flow += delta;
        }
    }
    return flow;
}

int main() {
    cin >> n >> m;
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        g[u].push_back(v);
        g[v].push_back(u);
        c[u][v] += w;
    }
    cout << dinic() << endl;
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < g[i].size(); j++) {
            int to = g[i][j];
            if (f[i][to]) {
                cout << f[i][to] << endl;
            } else {
                cout << "0\n";
            }
        }
    }
    return 0;
}

/*
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
  const size_t kSource = 0;
  const size_t kOutfall = 0;
  std::vector<int64_t> arr1;
  std::vector<int64_t> arr2;
  std::vector<std::vector<size_t>> arr4;

 public:
  Solution(const size_t& num_of_vertexes, const size_t& num_of_edges,
           const size_t& source, const size_t& outfall)
      : graph_body_(),
        kNumOfVertexes(num_of_vertexes),
        kSource(source),
        kOutfall(outfall),
        kNumOfEdges(num_of_edges),
        arr1(kNumOfVertexes),    // d
        arr2(kNumOfVertexes),    // ptr
        arr4(kNumOfVertexes) {}  // g
  void SetEdge(const size_t& start, const size_t& finish,
               const int64_t& weight) {
    std::pair<size_t, size_t> res =
        graph_body_.PlusOneEdge({start, finish}, weight);
    arr4[start].push_back(res.first);
    arr4[finish].push_back(res.second);
  }
  void Dinic() {
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
  static void SetVal(std::vector<int64_t>& arr, const int64_t& val) {
    for (auto& i : arr) {
      i = val;
    }
    /// TODO: переебать с помощью функции из numeric
  }
  static uint64_t GetAndPop(std::queue<uint64_t>& qee) {
    uint64_t tmp = qee.front();
    qee.pop();
    return tmp;
  }
  void BFS(bool& norm, const size_t& start, const size_t& finish) {
    SetVal(arr1, INT32_MAX);
    arr1[start] = 0;  // dist
    std::queue<uint64_t> qee;
    qee.emplace(start);
    for (; !qee.empty() && arr1[finish] == INT32_MAX;) {
      for (const auto& j : arr4[GetAndPop(qee)]) {
        if (arr1[graph_body_.get_finish(j)] == INT32_MAX &&
            graph_body_.get_thd(j) < graph_body_.get_cap(j)) {
          qee.emplace(graph_body_.get_finish(j));
          arr1[graph_body_.get_finish(j)] = arr1[graph_body_.get_start(j)] + 1;
        }
      }
    }
    norm = arr1[finish] != INT32_MAX;
  }
  void DFS(int64_t& res, size_t cur_node, int64_t thd) {
    if (thd <= 0) {
      res = 0;
      return;
    }
    if (cur_node == kOutfall) {
      res = thd;
      return;
    }
    size_t idx = arr2[cur_node];
    size_t dest_id = 0;
    size_t dest = 0;
    int64_t res1 = 0;
    while (idx < arr4[cur_node].size()) {
      dest_id = arr4[cur_node][idx];
      dest = graph_body_.get_finish(dest_id);
      if (graph_body_.get_cap(dest_id) -
              graph_body_.get_thd(dest_id) ==
          0) {
        ++arr2[cur_node];
        ++idx;
        continue;
      }
      if (arr1[dest] != arr1[cur_node] + 1) {
        ++arr2[cur_node];
        ++idx;
        continue;
      }
      size_t min_of_two = std::min(thd, graph_body_.get_cap(arr4[cur_node][idx]) -
                            graph_body_.get_thd(arr4[cur_node][idx]));
      DFS(res1, dest, min_of_two);
      if (res1 > 0) {
        graph_body_.get_thd(arr4[cur_node][idx]) += res1;
        graph_body_.get_thd((arr4[cur_node][idx]) ^ 1) -= res1;
        res = res1;
        return;
      }
      ++idx;
    }
    res = 0;
  }
};

int main() {
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
*/