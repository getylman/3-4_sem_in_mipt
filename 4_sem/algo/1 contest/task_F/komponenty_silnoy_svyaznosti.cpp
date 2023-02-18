#include <algorithm>
#include <iostream>
#include <set>
#include <vector>
// 1f
class Solution {
  enum Color { WHITE, GREY, BLACK };
  struct Node;
  std::vector<Node> adj_list_;
  std::vector<size_t> result_;
  std::vector<size_t> graph_top_sorted_;

  struct Node {
    Color color = Color::WHITE;
    size_t value = 0;
    std::set<size_t> neibors_norm;
    std::set<size_t> neibors_rev;
  };
  void AddNeib(const std::pair<size_t, size_t>& edge) {
    adj_list_[edge.first].neibors_norm.insert(edge.second);
    adj_list_[edge.second].neibors_rev.insert(edge.first);
  }
  void SetWhiteCol() {
    for (auto& i : adj_list_) {
      i.color = Color::WHITE;
    }
  }
  void Nothing() {}
  void BuildAdjList(const size_t& vetrexes,
                    const std::vector<std::pair<size_t, size_t>>& edges) {
    adj_list_.resize(vetrexes + 1);
    result_.resize(vetrexes + 1);
    for (size_t i = 1; i < adj_list_.size(); ++i) {
      adj_list_[i].value = i;
    }
    for (const auto& i : edges) {
      if (i.first == i.second) {
        continue;
      }
      AddNeib(i);
    }
  }
  void DFSForNormGraph(Node* cur_node) {
    cur_node->color = Color::GREY;
    for (const auto& i : adj_list_[cur_node->value].neibors_norm) {
      (adj_list_[i].color == Color::WHITE) ? DFSForNormGraph(&adj_list_[i])
                                           : Nothing();
    }
    cur_node->color = Color::BLACK;
    graph_top_sorted_.push_back(cur_node->value);
  }
  void DFSForRevGraph(Node* cur_node, size_t& count_num) {
    cur_node->color = Color::GREY;
    result_[cur_node->value] = count_num;
    for (const auto& i : adj_list_[cur_node->value].neibors_rev) {
      (adj_list_[i].color == Color::WHITE)
          ? DFSForRevGraph(&adj_list_[i], count_num)
          : Nothing();
    }
    cur_node->color = Color::BLACK;
  }

 public:
  void Answer(const size_t& vertexes,
              const std::vector<std::pair<size_t, size_t>>& edges) {
    BuildAdjList(vertexes, edges);
    for (auto& i : adj_list_) {
      (i.color == Color::WHITE) ? DFSForNormGraph(&i) : Nothing();
    }
    SetWhiteCol();
    size_t count_num = 0;
    std::reverse(graph_top_sorted_.begin(), graph_top_sorted_.end());
    for (const auto& i : graph_top_sorted_) {
      (adj_list_[i].color == Color::WHITE)
          ? (DFSForRevGraph(&adj_list_[i], ++count_num))
          : Nothing();
    }
    std::cout << count_num - 1 << '\n';
    for (size_t i = 1; i < result_.size(); ++i) {
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