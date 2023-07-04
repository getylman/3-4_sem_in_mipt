#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

class Solution {
 private:
  struct DSU {
    uint16_t Get(const uint16_t& vertex) {
      uint16_t node = vertex;
      for (; node != body_[node]; node = body_[node]);
      return body_[vertex] = node;
    }

    uint64_t GetTotalWeight() const { return total_val_; }

    void Union(const std::pair<std::pair<uint16_t, uint16_t>, uint32_t>& unit) {
      std::pair<uint16_t, uint16_t> two = {Get(unit.first.first),
                                           Get(unit.first.second)};
      uint16_t root = 0;
      if (two.first == two.second) {
        return;
      }
      root =
          (body_[(rang_[two.first] > rang_[two.second]) ? two.second
                                                        : two.first] =
               (rang_[two.first] > rang_[two.second]) ? two.first : two.second);
      total_val_ += unit.second;
      rang_[root] +=
          static_cast<uint16_t>(rang_[two.first] == rang_[two.second]);
    }
    DSU(const uint16_t& num_of_vertexes)
        : body_(num_of_vertexes + 1), rang_(num_of_vertexes + 1) {
      std::iota(body_.begin(), body_.end(), 0);
    }

   private:
    std::vector<uint32_t> body_;
    std::vector<uint32_t> rang_;
    uint64_t total_val_ = 0;
  };
  void ToSort(
      std::vector<std::pair<std::pair<uint16_t, uint16_t>, uint32_t>>& edges) {
    std::sort(
        edges.begin(), edges.end(),
        [](const std::pair<std::pair<uint16_t, uint16_t>, uint32_t>& left,
           const std::pair<std::pair<uint16_t, uint16_t>, uint32_t>& right) {
          return (left.second == right.second) ? left.first < right.first
                                               : left.second < right.second;
        });
  }
  DSU dsu_;

 public:
  Solution(
      const uint16_t& num_of_vertexes,
      std::vector<std::pair<std::pair<uint16_t, uint16_t>, uint32_t>>& edges)
      : dsu_(num_of_vertexes) {
    ToSort(edges);
    for (const auto& i : edges) {
      dsu_.Union(i);
    }
  }
  uint64_t GetAnswer() const {
    return dsu_.GetTotalWeight();
  }
};

int main() {
  uint16_t num_of_vertexes = 0;
  std::cin >> num_of_vertexes;
  std::vector<std::pair<std::pair<uint16_t, uint16_t>, uint32_t>> edges;
  edges.reserve(num_of_vertexes * num_of_vertexes);
  uint32_t weight = 0;
  for (uint16_t i = 1; i <= num_of_vertexes; ++i) {
    for (uint16_t j = 1; j <= num_of_vertexes; ++j) {
      std::cin >> weight;
      if (i >= j) {
        continue;
      }
      edges.push_back({{i, j}, weight});
    }
  }
  for (uint16_t i = 1; i <= num_of_vertexes; ++i) {
    std::cin >> weight;
    edges.push_back({{i, num_of_vertexes + 1}, weight});
  }
  Solution solution(num_of_vertexes + 1, edges);
  std::cout << solution.GetAnswer();
}