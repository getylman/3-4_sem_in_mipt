#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <queue>
#include <vector>

const int kMax = 1e9;

struct Edge {
  int from;
  int to;
  long long flow;
  int capacity;

  Edge(int from, int to, int capacity)
      : from(from), to(to), flow(0), capacity(capacity) {}

  Edge(int from, int to, long long flow, int capacity)
      : from(from), to(to), flow(flow), capacity(capacity) {}
};

class Graph {
 public:
  Graph(size_t num) : graph_(num, std::vector<size_t>()) {
    ptr_.resize(num, 0);
  }

  void AddEdge(int from, int to, int capacity) {
    edges_.emplace_back(from, to, capacity);
    size_t edge_id = edges_.size() - 1;
    edges_.emplace_back(to, from, 0);
    size_t back_edge_id = edges_.size() - 1;
    graph_[from].push_back(edge_id);
    graph_[to].push_back(back_edge_id);
  }

  bool BFS(size_t start, size_t end) {
    dist_.assign(graph_.size(), kMax);
    std::queue<size_t> queue;
    dist_[start] = 0;
    queue.push(start);
    while (!queue.empty() && dist_[end] == kMax) {
      size_t vertex = queue.front();
      queue.pop();
      for (size_t i = 0; i < graph_[vertex].size(); ++i) {
        size_t id = graph_[vertex][i];
        if (dist_[edges_[id].to] == kMax &&
            edges_[id].flow < edges_[id].capacity) {
          queue.push(edges_[id].to);
          dist_[edges_[id].to] = dist_[edges_[id].from] + 1;
        }
      }
    }
    return dist_[end] != kMax;
  }

  long long DFS(size_t vertex, size_t end, long long current_flow) {
    if (current_flow <= 0) {
      return 0;
    }
    if (vertex == end) {
      return current_flow;
    }
    for (size_t i = ptr_[vertex]; i < graph_[vertex].size(); ++i) {
      size_t to_id = graph_[vertex][i];
      size_t to = edges_[to_id].to;
      if (edges_[to_id].capacity - edges_[to_id].flow == 0) {
        ++ptr_[vertex];
        continue;
      }
      if (dist_[to] != dist_[vertex] + 1) {
        ++ptr_[vertex];
        continue;
      }
      int flow = DFS(
          to, end,
          std::min(current_flow, edges_[to_id].capacity - edges_[to_id].flow));
      if (flow > 0) {
        edges_[to_id].flow += flow;
        edges_[to_id ^ 1].flow -= flow;
        return flow;
      }
      ++ptr_[vertex];
    }

    return 0;
  }

  long long DinicAlgo(size_t start, size_t end) {
    long long flow = 0;
    while (BFS(start, end)) {
      long long delta = 0;
      std::fill(ptr_.begin(), ptr_.end(), 0);
      do {
        delta = DFS(start, end, kMax);
        // ptr.resize(end + 1, 0);
        flow += delta;
      } while (delta > 0);
    }
    return flow;
  }

  void ReturnFlows(int edge_count) {
    for (int i = 0; i < 2 * edge_count; i += 2) {
      std::cout << edges_[i].flow << '\n';
    }
  }

 private:
  std::vector<Edge> edges_;
  std::vector<std::vector<size_t>> graph_;
  std::vector<int> dist_;
  std::vector<int> ptr_;
};

int main() {
  int num;
  int edge_count;
  std::cin >> num >> edge_count;
  Graph gr(num);
  for (int i = 0; i < edge_count; ++i) {
    int from;
    int to;
    int capacity;
    std::cin >> from >> to >> capacity;
    gr.AddEdge(from - 1, to - 1, capacity);
  }
  std::cout << gr.DinicAlgo(0, num - 1) << '\n';
  gr.ReturnFlows(edge_count);
}