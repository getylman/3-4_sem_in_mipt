#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

class Solution {
  struct DSU;

 public:
  Solution(const uint32_t& num_of_vertexes) : dsu_(num_of_vertexes) {
  }
  void Command(const std::pair<std::pair<uint32_t, uint32_t>, uint32_t>& edge) {
    dsu_.Unite(edge);
    // dsu_.IncreasWeight({edge.second, edge.first.first});
  }
  void Command(const uint32_t& vertex) {
    printf("%lu\n", dsu_.GetWeight(vertex));
  }

 private:
  struct DSU {
    DSU(const uint32_t& num_of_vertexes)
        : body_(num_of_vertexes + 1),
          rang_(num_of_vertexes + 1),
          bands_(num_of_vertexes + 1) {
      std::iota(body_.begin(), body_.end(), 0);
    }

    void Unite(const std::pair<std::pair<uint32_t, uint32_t>, uint32_t>& unit) {
      std::pair<uint32_t, uint32_t> two = {Get(unit.first.first),
                                           Get(unit.first.second)};
      uint32_t root = 0;
      if (two.first == two.second) {
        bands_[two.first] += unit.second;
        return;
      }
      root =
          (body_[(rang_[two.first] > rang_[two.second]) ? two.second
                                                        : two.first] =
               (rang_[two.first] > rang_[two.second]) ? two.first : two.second);
      bands_[(rang_[two.first] > rang_[two.second]) ? two.first : two.second] +=
          bands_[(rang_[two.first] > rang_[two.second]) ? two.second
                                                        : two.first] +
          unit.second;
      // total_val_ += unit.second;
      rang_[root] +=
          static_cast<uint16_t>(rang_[two.first] == rang_[two.second]);
    }
    uint64_t GetWeight(const uint32_t& vertex) {
      return bands_[Get(vertex)];
    }

   private:
    std::vector<uint32_t> body_;
    std::vector<uint32_t> rang_;
    std::vector<uint64_t> bands_;
    uint32_t Get(const uint32_t& vertex) {
      uint32_t node = vertex;
      for (; node != body_[node]; node = body_[node]) {
      }
      return body_[vertex] = node;
    }
  };
  DSU dsu_;
};

int main() {
  uint32_t num_of_vertexes = 0;
  uint32_t num_of_calls = 0;
  scanf("%u%u", &num_of_vertexes, &num_of_calls);
  Solution solution(num_of_vertexes);
  uint16_t cmd = 0;
  std::pair<std::pair<uint32_t, uint32_t>, uint32_t> edge;
  uint32_t vertex = 0;
  while (num_of_calls-- > 0) {
    scanf("%hu", &cmd);
    if (cmd == 2) {
      scanf("%u", &vertex);
      solution.Command(vertex);
    } else {
      scanf("%u%u%u", &edge.first.first, &edge.first.second, &edge.second);
      if (edge.first.first == edge.first.second) {
        continue;
      }
      solution.Command({{edge.first.first, edge.first.second}, edge.second});
    }
  }
}