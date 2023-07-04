#include <iostream>
#include <numeric>
#include <vector>

class Solution {
 private:
  struct DSU {
    DSU(const uint32_t& num_of_vertexes)
        : par_(num_of_vertexes + 1), rang_(num_of_vertexes + 1) {
      std::iota(par_.begin(), par_.end(), 0);
    }
    void Union(const std::pair<uint32_t, uint32_t>& unit) {
      std::pair<uint32_t, uint32_t> two = {GetPar(unit.first),
                                           GetPar(unit.second)};
      uint64_t root = 0;
      if (two.first == two.second) {
        return;
      }
      root =
          (par_[(rang_[two.first] > rang_[two.second]) ? two.second
                                                       : two.first] =
               (rang_[two.first] > rang_[two.second]) ? two.first : two.second);
      rang_[root] +=
          static_cast<uint32_t>(rang_[two.first] == rang_[two.second]);
    }
    bool Ask(const std::pair<uint32_t, uint32_t>& vertex_pair) {
      return GetPar(vertex_pair.first) == GetPar(vertex_pair.second);
    }

   private:
    std::vector<uint32_t> par_;
    std::vector<uint32_t> rang_;

    uint32_t GetPar(const uint32_t& vertex) {
      uint32_t node = vertex;
      for (; node != par_[node]; node = par_[node]);
      return par_[vertex] = node;
    }
  };
  DSU dsu_;

 public:
  Solution(const uint32_t& num_of_vertexes) : dsu_(num_of_vertexes) {}
  void Answer(
      const std::vector<std::pair<std::pair<uint32_t, uint32_t>, bool>>& calls,
      const uint32_t& num_of_asks) {
    uint32_t idx = calls.size();
    uint32_t answer_tail = num_of_asks;
    std::vector<bool> answer(num_of_asks);
    while (idx-- > 0) {
      if (calls[idx].second) {
        dsu_.Union(calls[idx].first);
      } else {
        answer[--answer_tail] = dsu_.Ask(calls[idx].first);
      }
    }
    for (const auto& i : answer) {
      std::cout << (i ? "YES\n" : "NO\n");
    }
  }
};

int main() {
  uint32_t num_of_vertexes = 0;
  uint32_t num_of_edges = 0;
  uint32_t num_of_calls = 0;
  std::cin >> num_of_vertexes >> num_of_edges >> num_of_calls;
  std::vector<std::pair<uint32_t, uint32_t>> edges(num_of_edges);
  while (num_of_edges-- > 0) {
    std::cin >> edges[edges.size() - num_of_edges - 1].first >>
        edges[edges.size() - num_of_edges - 1].first;
  }
  std::string cmd;
  std::pair<uint32_t, uint32_t> vertex_pair;
  std::vector<std::pair<std::pair<uint32_t, uint32_t>, bool>> calls(
      num_of_calls);
  while (num_of_calls-- > 0) {
    std::cin >> cmd >> vertex_pair.first >> vertex_pair.second;
    calls[calls.size() - num_of_calls - 1] = {vertex_pair, cmd != "ask"};
    num_of_edges += static_cast<uint32_t>(cmd == "ask");
  }
  Solution solution(num_of_vertexes);
  solution.Answer(calls, num_of_edges + 1);
}