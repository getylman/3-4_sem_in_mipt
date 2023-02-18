#include <iostream>
#include <vector>
// 1c
// template <typename T>
class Solution {
  enum Color { WHITE, GREY, BLACK };
  struct Node {
    Color color = Color::WHITE;
    size_t value = 0;
    std::vector<size_t> neibors;
    // Node() : value(0), color(Color::WHITE) {}
    // Node(const size_t& vertex) : value(vertex), color(Color::WHITE) {}
    void AddNeib(const size_t& vertex) { neibors.push_back(vertex - 1); }
  };
  std::vector<Node> adj_list_;
  std::vector<size_t> result_;
  // std::vector<Color> colored;
  void BouildAdjList(const size_t& vetrexes,
                     const std::vector<std::pair<size_t, size_t>>& edges) {
    adj_list_.resize(vetrexes);
    for (const auto& i : edges) {
      adj_list_[i.first - 1].value = i.first - 1;
      adj_list_[i.first - 1].AddNeib(i.second);
    }
    // colored(vetrexes, Color::WHITE);
  }
  void DFS(Node* cur_node, bool& finded_loop, size_t& loop_start,
           std::vector<size_t>& loop) {
    if (cur_node->color == Color::GREY) {
      finded_loop = true;
      loop_start = cur_node->value;
      return;
    }
    cur_node->color = Color::GREY;
    for (const auto& i : cur_node->neibors) {
      if (adj_list_[i].color != Color::BLACK) {
        DFS(&adj_list_[i], finded_loop, loop_start, loop);
        if (finded_loop) {
          loop.push_back(i + 1);
          if (cur_node->value == loop_start) {
            result_ = loop;
          }
          return;
        }
        adj_list_[i].color = Color::BLACK;
      }
    }
    cur_node->color = Color::BLACK;
  }

 public:
  bool HaveLoop(const size_t& vertexes,
                const std::vector<std::pair<size_t, size_t>>& edges,
                std::vector<size_t>& res) {
    BouildAdjList(vertexes, edges);
    std::vector<size_t> tmp_way;
    bool finded_loop = false;
    size_t loop_start;
    for (auto& i : adj_list_) {
      if (i.color == Color::WHITE) {
        DFS(&i, finded_loop, loop_start, tmp_way);
        if (finded_loop) {
          res = result_;
          return true;
        }
      }
    }
    return false;
  }
};

int main() {
  size_t vertexes = 0;
  size_t num_of_edges = 0;
  std::cin >> vertexes >> num_of_edges;
  std::vector<std::pair<size_t, size_t>> edges(num_of_edges);
  for (size_t i = 0; i < num_of_edges; ++i) {
    std::cin >> edges[i].first >> edges[i].second;
    if (edges[i].first == edges[i].second) {
      std::cout << "YES\n";
      std::cout << edges[i].first << ' ' << edges[i].first;
      return 0;
    }
  }
  // edges = {{1, 2}, {2, 1}};
  std::vector<size_t> res;
  bool have_loop = Solution().HaveLoop(vertexes, edges, res);
  if (have_loop) {
    std::cout << "YES\n";
    for (int i = res.size() - 1; i >= 0; --i) {
      std::cout << res[i] << ' ';
    }
  } else {
    std::cout << "NO\n";
  }
}