#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>
// 2a
// wa7 
class Solution {
  const uint64_t kInf = INT64_MAX;
  using Edge = std::pair<uint64_t, uint64_t>;
  struct Node {
    std::vector<std::pair<uint64_t, uint64_t>> neighbours;
    uint64_t value = 0;
    uint64_t dist = 0;
    bool used = false;

   private:
    // const int64_t kInf = int64_t(-1);
  };
  std::vector<Node> adj_list_;
  std::set<Edge> heap_;
  void BuildAdjList(
      const size_t& vertexes,
      const std::vector<std::pair<std::pair<uint64_t, uint64_t>, uint64_t>>&
          edges) {
    adj_list_.resize(vertexes);
    for (size_t i = 0; i < vertexes; ++i) {
      adj_list_[i].value = i;
      adj_list_[i].dist = kInf;
      adj_list_[i].used = false;
    }
    for (const auto& i : edges) {
      adj_list_[i.first.first].neighbours.push_back({i.first.second, i.second});
      // adj_list_[i.first.second].neighbours.push_back({i.first.first,
      // i.second});
    }
  }
  static Edge ExtractMin(std::set<Edge>& heap) {
    Edge extracted_elem = *heap.rbegin();
    heap.erase(extracted_elem);
    return extracted_elem;
  }
  static void Nothing(){};
  void Dijkstra(const uint64_t& start) {
    adj_list_[start].dist = 0;
    heap_.insert({0, start});
    uint64_t top_of_heap;
    for (; !heap_.empty();) {
      top_of_heap = ExtractMin(heap_).second;
      if (!adj_list_[top_of_heap].used) {
        adj_list_[top_of_heap].used = true;
        for (const auto& i : adj_list_[top_of_heap].neighbours) {
          (i.second + adj_list_[top_of_heap].dist < adj_list_[i.first].dist)
              ? (void)heap_.insert(
                    {-(adj_list_[i.first].dist =
                           adj_list_[top_of_heap].dist + i.second),
                     i.first})
              : Nothing();
        }
      }
    }
  }

 public:
  void Answer(
      const size_t& vertexes,
      const std::vector<std::pair<std::pair<uint64_t, uint64_t>, uint64_t>>&
          edges,
      const size_t& start) {
    BuildAdjList(vertexes, edges);
    Dijkstra(start);
    const size_t kBig = 2009000999;
    for (const auto& i : adj_list_) {
      std::cout << ((i.dist == kInf) ? kBig : i.dist) << ' ';
    }
    std::cout << '\n';
  }
};

void KillAllDoubleEdgesAndLoops(
    std::vector<std::pair<std::pair<uint64_t, uint64_t>, uint64_t>>& edges) {
  std::map<std::pair<uint64_t, uint64_t>, uint64_t> tmp_edges;
  for (const auto& i : edges) {
    if (i.first.first == i.first.second) {
      continue;
    }
    if (tmp_edges.count(i.first) == 0) {
      tmp_edges.emplace(i);
    } else {
      tmp_edges[i.first] = std::min(tmp_edges[i.first], i.second);
    }
    if (tmp_edges.count({i.first.second, i.first.first}) == 0) {
      tmp_edges.insert({{i.first.second, i.first.first}, i.second});
    } else {
      tmp_edges[{i.first.second, i.first.first}] = std::min(
          tmp_edges[std::make_pair(i.first.second, i.first.first)], i.second);
    }
  }
  edges.clear();
  for (const auto& i : tmp_edges) {
    edges.push_back(i);
  }
}

int main() {
  size_t num_of_proccess = 0;
  std::cin >> num_of_proccess;
  size_t num_vertexes = 0;
  size_t num_edges = 0;
  size_t start_vertex = 0;
  std::pair<std::pair<uint64_t, uint64_t>, uint64_t> input_edge;
  std::vector<std::pair<std::pair<uint64_t, uint64_t>, uint64_t>> edges;
  while (num_of_proccess-- > 0) {
    std::cin >> num_vertexes >> num_edges;
    edges.reserve(num_edges);
    while (num_edges-- > 0) {
      std::cin >> input_edge.first.first >> input_edge.first.second >>
          input_edge.second;
      edges.emplace_back(input_edge);
    }
    KillAllDoubleEdgesAndLoops(edges);
    std::cin >> start_vertex;
    Solution().Answer(num_vertexes, edges, start_vertex);
  }
}