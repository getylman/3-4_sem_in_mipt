#include <iostream>
#include <set>
#include <vector>
// 1d
class Solution {
  enum Color { WHITE, GREY, BLACK };
  struct Node {
    Color color = Color::WHITE;
    size_t value = 0;
    std::vector<size_t> neibors;
    void AddNeib(const size_t& vertex) { neibors.push_back(vertex - 1); }
  };
  std::vector<Node> adj_list_;
  std::vector<std::set<size_t>> result_;
  void BouildAdjList(const size_t& vetrexes,
                     const std::vector<std::pair<size_t, size_t>>& edges) {
    adj_list_.resize(vetrexes);
    for (size_t i = 0; i < adj_list_.size(); ++i) {
      adj_list_[i].value = i;
    }
    for (const auto& i : edges) {
      // adj_list_[i.first - 1].value = i.first - 1;
      adj_list_[i.first - 1].AddNeib(i.second);
      // adj_list_[i.second - 1].value = i.second - 1;
      adj_list_[i.second - 1].AddNeib(i.first);
    }
  }
  void DFS(Node* cur_node, std::set<size_t>& con_comp) {
    if (cur_node->color == Color::BLACK or cur_node->color == Color::GREY) {
      return;
    }
    cur_node->color = Color::GREY;
    con_comp.insert(cur_node->value + 1);
    for (const auto& i : cur_node->neibors) {
      DFS(&adj_list_[i], con_comp);
      // adj_list_[i].color = Color::BLACK;
    }
    cur_node->color = Color::BLACK;
  }

 public:
  void Answer(const size_t& vertexes,
              const std::vector<std::pair<size_t, size_t>>& edges) {
    BouildAdjList(vertexes, edges);
    std::set<size_t> group;
    for (auto& i : adj_list_) {
      group.clear();
      DFS(&i, group);
      if (!group.empty()) {
        result_.push_back(group);
      }
    }
    std::cout << result_.size() << '\n';
    for (const auto& i : result_) {
      std::cout << i.size() << '\n';
      for (const auto& j : i) {
        std::cout << j << ' ';
      }
      std::cout << '\n';
    }
  }
};

int main() {
  size_t vertexes = 0;
  size_t num_of_edges = 0;
  std::cin >> vertexes >> num_of_edges;
  std::vector<std::pair<size_t, size_t>> edges(num_of_edges);
  for (size_t i = 0; i < num_of_edges; ++i) {
    std::cin >> edges[i].first >> edges[i].second;
  }
  Solution().Answer(vertexes, edges);
}