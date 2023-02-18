#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <vector>
// 1i
class Solution {
  enum Color { WHITE, GREY, BLACK };
  struct Node;
  std::vector<Node> adj_list_;
  std::set<size_t> result_;
  std::map<std::pair<size_t, size_t>, size_t> edge_num_;
  struct Node {
    Color color = Color::WHITE;
    size_t value = 0;
    std::set<size_t> neibors;
    size_t t_in = size_t(-1);
    size_t t_in_min = size_t(-1);
    void AddNeib(const size_t& vertex) { neibors.insert(vertex); }
  };
  void BuildAdjList(
      const size_t& vetrexes,
      const std::vector<std::pair<std::pair<size_t, size_t>, size_t>>& edges) {
    adj_list_.resize(vetrexes + 1);
    for (size_t i = 1; i < adj_list_.size(); ++i) {
      adj_list_[i].value = i;
    }
    for (const auto& i : edges) {
      if (i.first.first == i.first.second) {
        continue;
      }
      adj_list_[i.first.first].AddNeib(i.first.second);
      adj_list_[i.first.second].AddNeib(i.first.first);
      edge_num_.insert({{i.first.first, i.first.second}, i.second});
      edge_num_.insert({{i.first.second, i.first.first}, i.second});
    }
  }
  void Nothing() {}
  void Cringe(Node*& cur_node, size_t& counter_of_time, const size_t& iter);
  void AnotherCringe(Node*& cur_node, const size_t& iter) {
    cur_node->t_in_min =
        ((cur_node->t_in_min < adj_list_[iter].t_in) ? (cur_node->t_in_min)
                                                     : (adj_list_[iter].t_in));
  }
  void DFS(Node* cur_node, size_t& counter_of_time,
           size_t par_node = size_t(-1)) {
    cur_node->color = Color::BLACK;
    cur_node->t_in = counter_of_time;
    cur_node->t_in_min = counter_of_time;
    ++counter_of_time;
    for (const auto& i : adj_list_[cur_node->value].neibors) {
      (i != par_node) ? (adj_list_[i].color != Color::BLACK)
                            ? (Cringe(cur_node, counter_of_time, i))
                            : (AnotherCringe(cur_node, i))
                      : (Nothing());
    }
  }

 public:
  void Answer(
      const size_t& vertexes,
      const std::vector<std::pair<std::pair<size_t, size_t>, size_t>>& edges) {
    BuildAdjList(vertexes, edges);
    size_t counter_of_time = 0;
    for (auto& i : adj_list_) {
      (i.color == Color::WHITE) ? DFS(&i, counter_of_time) : Nothing();
    }
    std::cout << result_.size() << '\n';
    for (const auto& i : result_) {
      std::cout << i << ' ';
    }
  }
};

void Solution::Cringe(Node*& cur_node, size_t& counter_of_time,
                      const size_t& iter) {
  DFS(&adj_list_[iter], counter_of_time, cur_node->value);
  cur_node->t_in_min = (cur_node->t_in_min < adj_list_[iter].t_in_min)
                           ? cur_node->t_in_min
                           : adj_list_[iter].t_in_min;
  std::pair<size_t, size_t> tmp = {cur_node->value, adj_list_[iter].value};
  (adj_list_[iter].t_in_min > cur_node->t_in)
      ? ((void)result_.insert(edge_num_[tmp]))
      : (Nothing());
}

int main() {
  size_t vertexes = 0;
  size_t num_of_edges = 0;
  std::cin >> vertexes >> num_of_edges;
  std::vector<std::pair<std::pair<size_t, size_t>, size_t>> edges(num_of_edges);
  for (size_t i = 0; i < num_of_edges; ++i) {
    std::cin >> edges[i].first.first >> edges[i].first.second;
    edges[i].second = i + 1;
  }
  Solution().Answer(vertexes, edges);
}