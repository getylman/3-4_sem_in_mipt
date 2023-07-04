// 84436125
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>

template <typename VertexT = size_t,
          typename EdgeT = std::pair<VertexT, VertexT>,
          typename WeightT = int64_t>
class AbstructGraph {
 public:
  using VertexType = VertexT;
  using EdgeType = EdgeT;
  explicit AbstructGraph(const size_t& num_of_vertexes)
      : num_of_vertexes_(num_of_vertexes) {}
  void SetEdge(const EdgeT& edge, const WeightT& weight) {
    edge_keeper_.SetEdge(edge, weight);
  }
  size_t GetNumOfVertexes() const { return num_of_vertexes_; }
  size_t GetNumOfEdges() const { return edge_keeper_.GetNumOfEdges(); }
  virtual std::set<VertexT>& GetNeighbours(const VertexT& vertex) = 0;

 private:
  struct EdgeKeeper {
    // структура для получения веса по заданному ребру
    // да понимаю можно было использовать хеши но мне было лень прописывать
    // хеш функции и как-то париться над этот счёт)
    void SetEdge(const EdgeT& edge, const WeightT& weight) {
      if (edge_keeper_.contains(edge)) {
        edge_keeper_[edge].emplace_back(weight);
      } else {
        edge_keeper_.emplace({edge, std::vector<WeightT>(1, weight)});
      }
      ++num_of_edges_;
    }
    WeightT& GetMaxWeightOfEdge(const EdgeT& edge) {
      return *std::max_element(edge_keeper_[edge].begin(),
                               edge_keeper_[edge].end());
    }
    WeightT& GetMinWeightOfEdge(const EdgeT& edge) {
      return *std::min_element(edge_keeper_[edge].begin(),
                               edge_keeper_[edge].end());
    }
    size_t GetNumOfEdges() const { return num_of_edges_; }
    EdgeKeeper() : edge_keeper_(), num_of_edges_(0) {}
    ~EdgeKeeper() = default;

   private:
    std::map<EdgeT, std::vector<WeightT>> edge_keeper_;
    size_t num_of_edges_ = 0;
  };

 protected:
  EdgeKeeper edge_keeper_;
  size_t num_of_vertexes_ = 0;
};

template <typename VertexT = size_t,
          typename EdgeT = std::pair<VertexT, VertexT>,
          typename WeightT = int64_t>
class AdjListGraph : public AbstructGraph<VertexT, EdgeT, WeightT> {
 public:
  AdjListGraph(const std::set<VertexT>& vertexes,
               const std::vector<EdgeT>& edges,
               const std::vector<WeightT>& weights)
      : AbstructGraph<VertexT, EdgeT, WeightT>(vertexes.size(), edges.size()),
        vertexes_(vertexes) {
    for (size_t i = 0; i < edges.size(); ++i) {
      if (graph_.contains(edges[i].first)) {
        graph_[edges[i].first].emplace(edges[i].second);
      } else {
        graph_.emplace({edges[i].first, std::set<VertexT>(1, edges[i].second)});
      }
      if (graph_.contains(edges[i].second)) {
        graph_[edges[i].second].emplace(edges[i].first);
      } else {
        graph_.emplace({edges[i].second, std::set<VertexT>(1, edges[i].first)});
      }
      edges_.emplace({weights[i], edges[i]});
      AbstructGraph<VertexT, EdgeT, WeightT>::SetEdge(edges[i], weights[i]);
    }
  }
  std::set<VertexT>& GetNeighbours(const VertexT& vertex) override {
    return graph_[vertex];
  }
  typename std::set<VertexT>::iterator GetNeighIterator(const VertexT& vertex) {
    return graph_[vertex].begin();
  }
  const std::set<VertexT>& GetSetOfVertexes() { return vertexes_; }
  const std::set<std::pair<WeightT, EdgeT>>& GetSetOfEdges() { return edges_; }

 private:
  std::map<VertexT, std::set<VertexT>> graph_;
  std::set<std::pair<WeightT, EdgeT>> edges_;
  std::set<VertexT> vertexes_;
};

template <typename VertexT = size_t,
          typename EdgeT = std::pair<VertexT, VertexT>,
          typename WeightT = int64_t>
class DSU {
 public:
  DSU(const std::set<VertexT>& vertexes) {
    for (const auto& idx : vertexes) {
      body_.emplace(std::make_pair(idx, idx));
    }
    for (const auto& idx : vertexes) {
      ranks_.emplace(std::make_pair(idx, 0));
    }
  }
  DSU() = default;
  VertexT Get(const VertexT& vertex) {
    VertexT tmp = vertex;
    while (tmp != body_[tmp]) {
      tmp = body_[tmp];
    }
    return body_[vertex] = tmp;
  }
  void Unite(const std::pair<WeightT, EdgeT>& unit) {
    VertexT start = Get(unit.second.first);
    VertexT finish = Get(unit.second.second);
    if (start == finish) {
      return;
    }
    VertexT idx = (ranks_[start] > ranks_[finish]) ? finish : start;
    VertexT root = body_[idx] =
        (ranks_[start] > ranks_[finish]) ? start : finish;
    // total_val_ += unit.first;
    bands_[idx] +=
        bands_[(ranks_[start] > ranks_[finish]) ? finish : start] + unit.first;
    if (ranks_[start] == ranks_[finish]) {
      ++ranks_[root];
    }
  }
  void SetVertexSet(const std::set<VertexT>& vertexes) {
    for (const auto& idx : vertexes) {
      body_.emplace(std::make_pair(idx, idx));
    }
    for (const auto& idx : vertexes) {
      ranks_.emplace(std::make_pair(idx, 0));
    }
  }
  WeightT GetTotalWeight(const VertexT& vertex) { return bands_[Get(vertex)]; }

 private:
  std::map<VertexT, VertexT> body_;
  std::map<VertexT, uint64_t> ranks_;
  std::map<VertexT, WeightT> bands_;
  uint64_t total_val_ = 0;
};

class Solution {
 public:
  Solution(const size_t& num_of_vertexes) {
    std::set<unsigned long> vertexes;
    for (uint32_t i = 0; i <= num_of_vertexes; ++i) {
      vertexes.emplace(i);
    }
    dsu_.SetVertexSet(vertexes);
  }
  uint32_t Command(const uint32_t& vertex) {
    return dsu_.GetTotalWeight(vertex);
  }
  void Command(const std::pair<uint32_t, std::pair<uint32_t, uint32_t>>& edge) {
    dsu_.Unite(edge);
  }

 private:
  DSU<> dsu_;
};

int main() {
  uint32_t num_of_vertexes = 0;
  uint32_t num_of_calls = 0;
  std::cin >> num_of_vertexes >> num_of_calls;
  Solution sol(num_of_vertexes);
  std::pair<uint32_t, std::pair<uint32_t, uint32_t>> edge;
  uint32_t vertex = 0;
  uint16_t com;
  for (size_t i = 0; i < num_of_calls; ++i) {
    std::cin >> com;
    if (com == 1) {
      std::cin >> edge.second.first >> edge.second.second >> edge.first;
      if (edge.second.second == edge.second.first) {
        continue;
      }
      sol.Command(edge);
    } else {
      std::cin >> vertex;
      std::cout << sol.Command(vertex);
    }
  }
}
