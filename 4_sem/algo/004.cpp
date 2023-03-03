#include <iostream>
#include <queue>
#include <vector>
// 2b
class Solution {
  const uint64_t kInf = uint64_t(-1);
  struct Node {
    std::vector<std::pair<uint64_t, uint64_t>> neighbours;
    uint64_t value = 0;
    uint64_t dist = kInf;
    bool used = false;

   private:
    const uint64_t kInf = uint64_t(-1);
  };
  std::vector<Node> adj_list_;
  std::priority_queue<std::pair<uint64_t, uint64_t>,
                      std::vector<std::pair<uint64_t, uint64_t>>,
                      std::greater<std::pair<uint64_t, uint64_t>>>
      heap_;
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
      adj_list_[i.first.second].neighbours.push_back({i.first.first, i.second});
    }
  }
  static uint64_t ExtractMin(
      std::priority_queue<std::pair<uint64_t, uint64_t>,
                          std::vector<std::pair<uint64_t, uint64_t>>,
                          std::greater<std::pair<uint64_t, uint64_t>>>& heap) {
    uint64_t extracted_elem = heap.top().second;
    heap.pop();
    return extracted_elem;
  }
  static void Nothing(){};
  void Dijkstra(const uint64_t& start) {
    adj_list_[start].dist = 0;
    heap_.push({0, start});
    uint64_t top_of_heap = 0;
    for (; !heap_.empty();) {
      top_of_heap = ExtractMin(heap_);
      if (!adj_list_[top_of_heap].used) {
        adj_list_[top_of_heap].used = !false;
        for (const auto& i : adj_list_[top_of_heap].neighbours) {
          (i.second - adj_list_[top_of_heap].dist < adj_list_[i.first].dist)
              ? heap_.push({adj_list_[i.first].dist =
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
      const std::vector<std::pair<std::pair<uint64_t, uint64_t>, uint64_t>>&
          edges,
      const std::vector<uint64_t>& infected_vertexes,
      const std::pair<uint64_t, uint64_t>& start_and_finsh) {
    BuildAdjList(vertexes, edges);
    Dijkstra(start_and_finsh.second);
    for (const auto& i : infected_vertexes) {
      if (adj_list_[i].dist < adj_list_[start_and_finsh.first].dist) {
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
  std::vector<uint64_t> infected_vertexes(num_of_infected);
  for (auto& i : infected_vertexes) {
    std::cin >> i;
  }
  std::vector<std::pair<std::pair<uint64_t, uint64_t>, uint64_t>> edges(
      num_of_edges);
  for (auto& i : edges) {
    std::cin >> i.first.first >> i.first.second >> i.second;
  }
  std::pair<uint64_t, uint64_t> start_and_finish;
  std::cin >> start_and_finish.first >> start_and_finish.second;
  Solution().Answer(vertexes, edges, infected_vertexes, start_and_finish);
}