#include <iostream>
#include <map>
#include <queue>
#include <vector>
// 2b
class Solution {
  const uint64_t kInf = 9223372036854775807;
  using Edge = std::pair<uint64_t, std::pair<uint64_t, uint64_t>>;
  struct Node {
    std::vector<std::pair<uint64_t, uint64_t>> neighbours;
    uint64_t value = 0;
    uint64_t dist = 0;
    bool used = false;

   private:
    // const uint64_t kInf = uint64_t(-1);
  };
  std::vector<Node> adj_list_;
  std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> heap_;
  void BuildAdjList(
      const size_t& vertexes,
      const std::vector<std::pair<std::pair<uint64_t, uint64_t>, uint64_t>>&
          edges) {
    adj_list_.resize(vertexes + 1);
    for (size_t i = 1; i <= vertexes; ++i) {
      adj_list_[i].value = i;
      adj_list_[i].dist = kInf;
    }
    for (const auto& i : edges) {
      adj_list_[i.first.first].neighbours.push_back({i.first.second, i.second});
      // adj_list_[i.first.second].neighbours.push_back({i.first.first,
      // i.second});
    }
  }
  static Edge ExtractMin(
      std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>>& heap) {
    Edge extracted_elem = heap.top();
    heap.pop();
    return extracted_elem;
  }
  static void Nothing(){};
  void Dijkstra(const uint64_t& start) {
    adj_list_[start].dist = 0;
    for (const auto& i : adj_list_[start].neighbours) {
      heap_.push({start, i});
    }
    Edge top_of_heap;
    for (; !heap_.empty();) {
      top_of_heap = ExtractMin(heap_);
      if (adj_list_[top_of_heap.second.first].dist < kInf) {
        continue;
      }
      adj_list_[top_of_heap.second.first].dist =
          top_of_heap.second.second + adj_list_[top_of_heap.first].dist;
      for (const auto& i : adj_list_[top_of_heap.second.first].neighbours) {
        heap_.push({top_of_heap.second.first, {i.first, i.second}});
      }
    }
  }

 public:
  void Answer(
      const size_t& vertexes,
      const std::vector<std::pair<std::pair<uint64_t, uint64_t>, uint64_t>>& edges,
      const std::vector<uint64_t>& infected_vertexes,
      const std::pair<uint64_t, uint64_t>& start_and_finsh) {
    BuildAdjList(vertexes, edges);
    Dijkstra(start_and_finsh.second);
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
  std::pair<std::pair<uint64_t, uint64_t>, uint64_t> input_edge;
  std::vector<uint64_t> infected_vertexes(num_of_infected);
  for (auto& i : infected_vertexes) {
    std::cin >> i;
  }
  std::vector<std::pair<std::pair<uint64_t, uint64_t>, uint64_t>> edges;
  edges.reserve(num_of_edges);
  std::map<std::pair<uint64_t, uint64_t>, uint64_t> tmp_edges;
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
  std::pair<uint64_t, uint64_t> start_and_finish;
  std::cin >> start_and_finish.first >> start_and_finish.second;
  Solution().Answer(vertexes, edges, infected_vertexes, start_and_finish);
}