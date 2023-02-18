#include <iostream>
#include <set>
#include <vector>
// 1e
class Solution {
  enum Color { WHITE, GREY, BLACK };
  struct Node {
    Color color = Color::WHITE;
    size_t value = 0;
    std::vector<size_t> neibors;
    void AddNeib(const size_t& vertex) { neibors.push_back(vertex - 1); }
  };
  std::vector<Node> adj_list_;
  std::vector<size_t> result_;
  void BouildAdjList(const size_t& vetrexes,
                     const std::vector<std::pair<size_t, size_t>>& edges) {
    adj_list_.resize(vetrexes);
    for (size_t i = 0; i < adj_list_.size(); ++i) {
      adj_list_[i].value = i;
    }
    for (const auto& i : edges) {
      adj_list_[i.first - 1].AddNeib(i.second);
    }
  }
  void DFS(Node* cur_node) {
    if (cur_node->color == Color::BLACK) {
      return;
    }
    if (cur_node->color == Color::GREY) {
      throw 1;
    }
    cur_node->color = Color::GREY;
    for (const auto& i : cur_node->neibors) {
      DFS(&adj_list_[i]);
    }
    cur_node->color = Color::BLACK;
    result_.push_back(cur_node->value + 1);
  }

 public:
  void Answer(const size_t& vertexes,
              const std::vector<std::pair<size_t, size_t>>& edges) {
    BouildAdjList(vertexes, edges);
    try {
      for (auto& i : adj_list_) {
        DFS(&i);
      }
    } catch (...) {
      std::cout << -1;
      return;
    }
    for (int i = result_.size() - 1; i >= 0; --i) {
      std::cout << result_[i] << ' ';
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