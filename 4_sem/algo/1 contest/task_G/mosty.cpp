#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <vector>
// 1g
// wa26 
class Solution {
  enum Color { WHITE, GREY, BLACK };
  struct Node;
  std::vector<Node> adj_list_;
  std::vector<size_t> result_;
  // std::map<std::pair<size_t, size_t>, size_t> edge_num_;
  struct Node {
    Color color = Color::WHITE;
    size_t value = 0;
    std::vector<std::pair<size_t, std::pair<size_t, bool>>> neibors;
    size_t t_in = size_t(-1);
    size_t t_in_min = size_t(-1);
    void AddNeib(const std::pair<size_t, std::pair<size_t, bool>>& vertex) {
      neibors.push_back(vertex);
    }
  };
  void BuildAdjList(const size_t& vetrexes,
                    const std::map<std::pair<size_t, size_t>,
                                   std::pair<size_t, bool>>& edges) {
    adj_list_.resize(vetrexes + 1);
    for (size_t i = 1; i < adj_list_.size(); ++i) {
      adj_list_[i].value = i;
    }
    for (const auto& i : edges) {
      if (i.first.first == i.first.second) {
        continue;
      }
      adj_list_[i.first.first].AddNeib({i.first.second, i.second});
      adj_list_[i.first.second].AddNeib({i.first.first, i.second});
      // edge_num_.insert({{i.first.first, i.first.second}, i.second});
      // edge_num_.insert({{i.first.second, i.first.first}, i.second});
    }
  }
  void Nothing() {}
  void Cringe(Node*& cur_node, size_t& counter_of_time,
              const std::pair<size_t, std::pair<size_t, bool>>& iter);
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
      (i.first != par_node) ? (adj_list_[i.first].color != Color::BLACK)
                                  ? (Cringe(cur_node, counter_of_time, i))
                                  : (AnotherCringe(cur_node, i.first))
                            : (Nothing());
    }
  }

 public:
  void Answer(const size_t& vertexes,
              const std::map<std::pair<size_t, size_t>,
                             std::pair<size_t, bool>>& edges) {
    BuildAdjList(vertexes, edges);
    size_t counter_of_time = 0;
    for (auto& i : adj_list_) {
      (i.color == Color::WHITE) ? DFS(&i, counter_of_time) : Nothing();
    }
    std::cout << result_.size() << '\n';
    std::sort(result_.begin(), result_.end());
    for (const auto& i : result_) {
      std::cout << i << ' ';
    }
  }
};

void Solution::Cringe(Node*& cur_node, size_t& counter_of_time,
                      const std::pair<size_t, std::pair<size_t, bool>>& iter) {
  DFS(&adj_list_[iter.first], counter_of_time, cur_node->value);
  cur_node->t_in_min = (cur_node->t_in_min < adj_list_[iter.first].t_in_min)
                           ? cur_node->t_in_min
                           : adj_list_[iter.first].t_in_min;
  // std::pair<size_t, size_t> tmp = {cur_node->value,
  //                                  adj_list_[iter.first].value};
  (adj_list_[iter.first].t_in_min > cur_node->t_in and iter.second.second)
      ? ((void)result_.push_back(iter.second.first))
      : (Nothing());
}

int main() {
  size_t vertexes = 0;
  size_t num_of_edges = 0;
  std::cin >> vertexes >> num_of_edges;
  size_t start = 0;
  size_t finish = 0;
  // size_t num_of_edge = 0;
  std::map<std::pair<size_t, size_t>, std::pair<size_t, bool>> edges;
  // std::vector<std::pair<std::pair<size_t, size_t>, size_t>>
  // edges(num_of_edges);
  for (size_t i = 0; i < num_of_edges;) {
    std::cin >> start >> finish;
    if (edges.count({start, finish}) == 0) {
      edges.insert({{start, finish}, {i + 1, true}});
    } else {
      edges[{start, finish}].second = false;
    }
    ++i;
  }
  Solution().Answer(vertexes, edges);
}