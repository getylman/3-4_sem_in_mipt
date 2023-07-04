#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>
// 2b
class Solution {
  const int64_t kInf = INT64_MAX;
  using Edge = std::pair<int64_t, int64_t>;
  struct Node {
    std::vector<std::pair<int64_t, int64_t>> neighbours;
    int64_t value = 0;
    int64_t dist = 0;
    bool used = false;

   private:
    // const int64_t kInf = int64_t(-1);
  };
  std::vector<Node> adj_list_;
  std::set<Edge> heap_;
  void BuildAdjList(
      const size_t& vertexes,
      const std::vector<std::pair<std::pair<int64_t, int64_t>, int64_t>>&
          edges) {
    adj_list_.resize(vertexes + 1);
    for (size_t i = 1; i <= vertexes; ++i) {
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
  void Dijkstra(const int64_t& start) {
    adj_list_[start].dist = 0;
    heap_.insert({0, start});
    int64_t top_of_heap;
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
      const std::vector<std::pair<std::pair<int64_t, int64_t>, int64_t>>& edges,
      const std::vector<int64_t>& infected_vertexes,
      const std::pair<int64_t, int64_t>& start_and_finsh) {
    BuildAdjList(vertexes, edges);
    Dijkstra(start_and_finsh.second);
    if (adj_list_[start_and_finsh.first].dist == kInf) {
      std::puts("-1");
      return;
    }
    for (const auto& i : infected_vertexes) {
      if (adj_list_[i].dist <= adj_list_[start_and_finsh.first].dist) {
        std::puts("-1");
        return;
      }
    }
    std::puts((std::to_string(adj_list_[start_and_finsh.first].dist)).data());
  }
};

int main() {
  size_t vertexes = 0;
  size_t num_of_edges = 0;
  size_t num_of_infected = 0;
  std::cin >> vertexes >> num_of_edges >> num_of_infected;
  std::pair<std::pair<int64_t, int64_t>, int64_t> input_edge;
  std::vector<int64_t> infected_vertexes(num_of_infected);
  for (auto& i : infected_vertexes) {
    std::cin >> i;
  }
  std::vector<std::pair<std::pair<int64_t, int64_t>, int64_t>> edges;
  edges.reserve(num_of_edges);
  std::map<std::pair<int64_t, int64_t>, int64_t> tmp_edges;
  for (; num_of_edges-- > 0;) {
    std::cin >> input_edge.first.first >> input_edge.first.second >>
        input_edge.second;
    if (input_edge.first.first == input_edge.first.second) {
      continue;
    }
    if (tmp_edges.count(input_edge.first) == 0) {
      tmp_edges.emplace(input_edge);
    } else {
      tmp_edges[input_edge.first] =
          std::min(tmp_edges[input_edge.first], input_edge.second);
    }
    if (tmp_edges.count({input_edge.first.second, input_edge.first.first}) ==
        0) {
      tmp_edges.insert({{input_edge.first.second, input_edge.first.first},
                        input_edge.second});
    } else {
      tmp_edges[{input_edge.first.second, input_edge.first.first}] =
          std::min(tmp_edges[{input_edge.first.second, input_edge.first.first}],
                   input_edge.second);
    }
  }
  for (const auto& i : tmp_edges) {
    edges.push_back(i);
  }
  std::pair<int64_t, int64_t> start_and_finish;
  std::cin >> start_and_finish.first >> start_and_finish.second;
  Solution().Answer(vertexes, edges, infected_vertexes, start_and_finish);
}