#include <iostream>
#include <queue>
#include <set>
#include <vector>
// 1h
class Solution {
  enum Color { WHITE, GREY, BLACK };
  struct Node {
    Color color = Color::WHITE;
    size_t value = 0;
    std::set<size_t> neibors;
    size_t t_in = size_t(-1);
    size_t t_in_min = size_t(-1);
    void AddNeib(const size_t& vertex) { neibors.insert(vertex); }
  };
  std::vector<Node> adj_list_;
  std::set<size_t> result_;
  void BouildAdjList(const size_t& vetrexes,
                     const std::vector<std::pair<size_t, size_t>>& edges) {
    adj_list_.resize(vetrexes + 1);
    for (size_t i = 1; i < adj_list_.size(); ++i) {
      adj_list_[i].value = i;
    }
    for (const auto& i : edges) {
      if (i.first == i.second) {
        continue;
      }
      adj_list_[i.first].AddNeib(i.second);
      adj_list_[i.second].AddNeib(i.first);
    }
  }
  void Nothink() {}
  void Cringe(Node*& cur_node, size_t& counter_of_time, const size_t& iter,
              size_t& num_of_kids, size_t& par_node);
  void AnotherCringe(Node*& cur_node, const size_t& iter) {
    cur_node->t_in_min =
        ((cur_node->t_in_min < adj_list_[iter].t_in) ? (cur_node->t_in_min)
                                                     : (adj_list_[iter].t_in));
  }
  void DFS(Node* cur_node, size_t& counter_of_time, size_t par_node = -1) {
    cur_node->color = Color::BLACK;
    cur_node->t_in_min = cur_node->t_in = counter_of_time++;
    size_t num_of_kids = 0;
    for (const auto& i : adj_list_[cur_node->value].neibors) {
      (i != par_node)
          ? (adj_list_[i].color != Color::BLACK)
                ? (Cringe(cur_node, counter_of_time, i, num_of_kids, par_node))
                : (AnotherCringe(cur_node, i))
          : (Nothink());
    }
    (par_node == size_t(-1) and num_of_kids > 1)
        ? (void)result_.insert(cur_node->value)
        : Nothink();
  }

 public:
  void Answer(const size_t& vertexes,
              const std::vector<std::pair<size_t, size_t>>& edges) {
    BouildAdjList(vertexes, edges);
    size_t counter_of_time = 0;
    for (auto& i : adj_list_) {
      (i.color == Color::WHITE) ? DFS(&i, counter_of_time) : Nothink();
    }
    std::cout << result_.size() << '\n';
    for (const auto& i : result_) {
      std::cout << i << '\n';
    }
  }
};

void Solution::Cringe(Node*& cur_node, size_t& counter_of_time,
                      const size_t& iter, size_t& num_of_kids,
                      size_t& par_node) {
  DFS(&adj_list_[iter], counter_of_time, cur_node->value);
  cur_node->t_in_min = (cur_node->t_in_min < adj_list_[iter].t_in_min)
                           ? cur_node->t_in_min
                           : adj_list_[iter].t_in_min;
  (adj_list_[iter].t_in_min >= cur_node->t_in and par_node != size_t(-1))
      ? ((void)result_.insert(cur_node->value))
      : (Nothink());
  ++num_of_kids;
}

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