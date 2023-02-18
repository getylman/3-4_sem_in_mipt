#include <iostream>
#include <queue>
#include <vector>
// 1b
class Solution {
  size_t num_of_vertexes_ = 0;
  size_t max_weight_ = 0;
  enum Color { WHITE, GREY, BLACK };
  struct Way {
    size_t vertex = 0;
    size_t dist = -1;
    Way() : vertex(0), dist(-1) {}
    Way(const std::pair<size_t, size_t>& way)
        : vertex(way.first), dist(way.second) {}
    // Way(const size_t& vertex, const size_t& dist) : vertex(vertex),
    // dist(dist) {}
  };
  struct Node {
    Color color = Color::WHITE;
    size_t value = 0;
    std::vector<Way> neibors;
    void AddNeib(const std::pair<size_t, size_t>& way) {
      neibors.push_back(Way(way));
    }
  };
  std::vector<Node> adj_list_;
  std::vector<size_t> result_;
  std::vector<std::queue<size_t>> bfs1_k_;
  std::vector<size_t> dist_;
  void BouildAdjList(
      const size_t& vetrexes,
      const std::vector<std::pair<size_t, std::pair<size_t, size_t>>>& edges,
      const size_t& max_weight) {
    num_of_vertexes_ = vetrexes;
    this->max_weight_ = max_weight;
    adj_list_.resize(vetrexes + 1);
    for (size_t i = 1; i < adj_list_.size(); ++i) {
      adj_list_[i].value = i;
    }
    for (const auto& i : edges) {
      adj_list_[i.first].AddNeib(i.second);
    }
  }
  size_t GetFirst(std::queue<size_t>& que, size_t& num) {
    size_t first = que.front();
    que.pop();
    --num;
    return first;
  };

  void SetToQ(std::queue<size_t>& que, size_t& num, size_t& pushing) {
    que.push(pushing);
    ++num;
  }

  // void DFS(Node* cur_node) {
  //   if (cur_node->color == Color::BLACK) {
  //     return;
  //   }
  //   if (cur_node->color == Color::GREY) {
  //     throw 1;
  //   }
  //   cur_node->color = Color::GREY;
  //   for (const auto& i : cur_node->neibors) {
  //     DFS(&adj_list_[i]);
  //   }
  //   cur_node->color = Color::BLACK;
  //   result_.push_back(cur_node->value + 1);
  // }
  size_t index_ = 0;
  size_t amount_ = 1;
  size_t cur_neig_ = 0;
  size_t weight_ = 0;
  size_t cur_of_cur_ = 0;
  void BFS(Node* cur_node) {
    const size_t kSize = max_weight_ + 1;
    bfs1_k_.resize(kSize);
    dist_.resize(num_of_vertexes_ + 1, size_t(-1));
    dist_[cur_node->value] = 0;
    bfs1_k_.front().push(cur_node->value);
    while (amount_ != 0) {
      while (bfs1_k_[index_ % kSize].empty()) {
        ++index_;
      }
      cur_neig_ = GetFirst(bfs1_k_[index_ % kSize], amount_);
      if (adj_list_[cur_neig_].color != Color::BLACK) {
        adj_list_[cur_neig_].color = Color::BLACK;
        for (auto& i : adj_list_[cur_neig_].neibors) {
          weight_ = i.dist;
          cur_of_cur_ = i.vertex;
          if (dist_[cur_neig_] + weight_ < dist_[cur_of_cur_]) {
            dist_[cur_of_cur_] =
                weight_ +
                ((dist_[cur_of_cur_] == size_t(-1))
                     ? ((cur_node->value == cur_neig_) ? 0 : dist_[cur_neig_])
                     : dist_[cur_neig_]);
            SetToQ(bfs1_k_[dist_[cur_of_cur_] % kSize], amount_, cur_of_cur_);
          }
        }
      }
    }
  }

 public:
  void Answer(
      const size_t& vertexes,
      const std::vector<std::pair<size_t, std::pair<size_t, size_t>>>& edges,
      const std::pair<size_t, size_t>& two, const size_t& max_weight) {
    BouildAdjList(vertexes, edges, max_weight);
    BFS(&adj_list_[two.first]);
    if (dist_[two.second] == size_t(-1)) {
      std::cout << -1;
      return;
    }
    std::cout << dist_[two.second];
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);
  size_t vertexes = 0;
  size_t num_of_edges = 0;
  std::pair<size_t, size_t> two;
  std::cin >> vertexes >> num_of_edges >> two.first >> two.second;
  if (two.first == two.second) {
    std::cout << 0;
    return 0;
  }
  size_t max_weight = 0;
  // std::vector<std::pair<size_t, std::pair<size_t, size_t>>> input = {
  //     {1, {2, 1}}, {1, {4, 3}}, {2, {4, 1}}, {2, {3, 2}},
  //     {4, {5, 1}}, {3, {6, 5}}, {5, {6, 1}}};
  std::vector<std::pair<size_t, std::pair<size_t, size_t>>> edges(num_of_edges);
  for (size_t i = 0; i < num_of_edges; ++i) {
    std::cin >> edges[i].first >> edges[i].second.first >>
        edges[i].second.second;
    // edges[i] = input[i];
    max_weight = std::max(max_weight, edges[i].second.second);
  }
  Solution().Answer(vertexes, edges, two, max_weight);
}