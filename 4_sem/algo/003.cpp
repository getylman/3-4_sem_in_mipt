#include <iostream>
#include <queue>
#include <vector>
// 2b
class Solution {
  const uint32_t kInf = 0xffffffff;
  using Edge = std::pair<uint32_t, std::pair<uint32_t, uint32_t>>;
  struct Node {
    std::vector<std::pair<uint32_t, uint32_t>> neighbours;
    uint32_t value = 0;
    uint32_t dist = kInf;
    bool used = false;

   private:
    const uint32_t kInf = 0xffffffff;
  };
  std::vector<Node> adj_list_;
  std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> heap_;
  void BuildAdjList(
      const size_t& vertexes,
      const std::vector<std::pair<std::pair<uint32_t, uint32_t>, uint32_t>>&
          edges) {
    adj_list_.resize(vertexes + 1);
    for (size_t i = 1; i <= vertexes; ++i) {
      adj_list_[i].value = i;
      adj_list_[i].dist = kInf;
    }
    for (const auto& i : edges) {
      adj_list_[i.first.first].neighbours.push_back({i.first.second, i.second});
      adj_list_[i.first.second].neighbours.push_back({i.first.first, i.second});
    }
  }
  static Edge ExtractMin(
      std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>>& heap) {
    Edge extracted_elem = heap.top();
    heap.pop();
    return extracted_elem;
  }
  static void Nothing(){};
  void Dijkstra(const uint32_t& start) {
    adj_list_[start].dist = 0;
    for (const auto& i : adj_list_[start].neighbours) {
      heap_.push({start, i});
    }
    Edge top_of_heap;
    for (; !heap_.empty();) {
      top_of_heap = ExtractMin(heap_);
      if (!adj_list_[top_of_heap.second.first].used) {
        adj_list_[top_of_heap.second.first].used = !false;
        for (const auto& i : adj_list_[top_of_heap.second.first].neighbours) {
          (i.second - adj_list_[top_of_heap.second.first].dist < adj_list_[i.first].dist)
              ? heap_.push({adj_list_[i.first].dist = // zaputalsya
                                adj_list_[top_of_heap].dist + i.second,
                            i.first})
              : Nothing();
        }
      }
    }
  }

 public:
  void Answer(
      const size_t& vertexes,
      const std::vector<std::pair<std::pair<uint32_t, uint32_t>, uint32_t>>&
          edges,
      const std::vector<uint32_t>& infected_vertexes,
      const std::pair<uint32_t, uint32_t>& start_and_finsh) {
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
  std::vector<uint32_t> infected_vertexes(num_of_infected);
  for (auto& i : infected_vertexes) {
    std::cin >> i;
  }
  std::vector<std::pair<std::pair<uint32_t, uint32_t>, uint32_t>> edges(
      num_of_edges);
  for (auto& i : edges) {
    std::cin >> i.first.first >> i.first.second >> i.second;
  }
  std::pair<uint32_t, uint32_t> start_and_finish;
  std::cin >> start_and_finish.first >> start_and_finish.second;
  Solution().Answer(vertexes, edges, infected_vertexes, start_and_finish);
}