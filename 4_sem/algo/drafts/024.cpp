#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <limits>
#include <queue>
#include <vector>

class Solution {
  struct Edge {
    unsigned short src;
    unsigned short dst;
    int64_t thd;
    int64_t cap;

    Edge(const unsigned short& src, const unsigned short& dst,
         const int64_t& thd, const int64_t& cap)
        : src(src), dst(dst), thd(thd), cap(cap) {}
  };

  struct TaskStruct {
    std::vector<std::vector<unsigned>> arr1;
    std::vector<int64_t> arr_d;
    std::vector<int64_t> arr_p;
    const unsigned short kSource = 0;
    const unsigned short kOutfall = 0;
    const unsigned short kNumOfEdges = 0;
    const unsigned short kNumOfVertexes = 0;

    TaskStruct(const unsigned short& num_of_vertexes,
               const unsigned short& num_of_edges, const unsigned short& sourse,
               const unsigned short& outfall)
        : arr1(num_of_vertexes),
          arr_d(num_of_vertexes),
          arr_p(num_of_vertexes),
          kSource(sourse),
          kOutfall(outfall),
          kNumOfEdges(num_of_edges),
          kNumOfVertexes(num_of_vertexes) {}
  };

  TaskStruct body_;
  std::vector<Edge> edges_;

  static size_t GetsrcQ(std::queue<size_t>& qee) {
    size_t tmp = qee.front();
    qee.pop();
    return tmp;
  }

  bool Bfs() {
    SetVal(body_.arr_d, INT64_MAX);
    std::queue<size_t> qee;
    qee.emplace(body_.arr_d[body_.kSource] = 0);
    for (; !qee.empty() and body_.arr_d[body_.kOutfall] == INT64_MAX;) {
      for (const unsigned& idx : body_.arr1[GetsrcQ(qee)]) {
        while (body_.arr_d[edges_[idx].dst] == INT64_MAX and
               edges_[idx].thd < edges_[idx].cap) {
          qee.emplace(edges_[idx].dst);
          body_.arr_d[edges_[idx].dst] = body_.arr_d[edges_[idx].src] + 1;
          break;
        }
      }
    }
    return body_.arr_d[body_.kOutfall] != INT64_MAX;
  }

  int64_t Dfs(unsigned short cur_vertex, unsigned short end, int64_t cur_thd) {
    while (cur_thd <= 0 or cur_vertex == end) {
      return (cur_thd <= 0) ? 0 : cur_thd;
    }
    size_t idx = body_.arr_p[cur_vertex];
    size_t outfall_id = 0;
    size_t dest = 0;
    while (idx++ < body_.arr1[cur_vertex].size()) {
      outfall_id = body_.arr1[cur_vertex][idx - 1];
      dest = edges_[outfall_id].dst;
      if ((edges_[outfall_id].cap - edges_[outfall_id].thd == 0) or
          (body_.arr_d[dest] != body_.arr_d[cur_vertex] + 1)) {
        ++body_.arr_p[cur_vertex];
        continue;
      }
      int64_t thd = Dfs(
          dest, end,
          std::min(cur_thd, edges_[outfall_id].cap - edges_[outfall_id].thd));
      while (thd > 0) {
        edges_[outfall_id].thd += thd;
        edges_[outfall_id ^ 1].thd -= thd;
        return thd;
      }
      ++body_.arr_p[cur_vertex];
    }
    return 0;
  }

  static void SetVal(std::vector<int64_t>& vec, const int64_t& val) {
    std::fill(vec.begin(), vec.end(), val);
  }

 public:
  Solution(const unsigned short& num_of_vertexes,
           const unsigned short& num_of_edges, const unsigned short& sourse,
           const unsigned short& outfall)
      : body_(num_of_vertexes, num_of_edges, sourse, outfall) {}

  void PlusOneEdge(const unsigned short& src, const unsigned short& dst,
                   const int64_t& cap) {
    edges_.emplace_back(src, dst, 0, cap);
    edges_.emplace_back(dst, src, 0, 0);
    body_.arr1[src].emplace_back(edges_.size() - 2);
    body_.arr1[dst].emplace_back(edges_.size() - 1);
  }

  void Dinic(const unsigned short& start, const unsigned short& end) {
    int64_t thd = 0;
    for (; Bfs();) {
      int64_t delta = 1;
      SetVal(body_.arr_p, 0);
      for (; delta != 0;) {
        delta = Dfs(start, end, INT64_MAX);
        thd += delta;
      }
    }
    std::cout << thd << '\n';
    thd = 0;
    while (thd < (int64_t)edges_.size()) {
      std::cout << edges_[(thd += 2) - 2].thd << '\n';
    }
  }
};

int main() {
  unsigned short num_of_vertexes;
  unsigned short num_of_edges;
  std::cin >> num_of_vertexes >> num_of_edges;
  uint16_t head = 0;
  uint16_t tail = 0;
  uint32_t cap = 0;
  Solution gr(num_of_vertexes, num_of_edges, 0, num_of_vertexes - 1);
  while (num_of_edges-- != (unsigned short)0) {
    std::cin >> head >> tail >> cap;
    gr.PlusOneEdge(--head, --tail, cap);
  }
  gr.Dinic(0, num_of_vertexes - 1);
}