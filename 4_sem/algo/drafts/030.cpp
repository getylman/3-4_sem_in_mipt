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
 private:
  struct EdgeKeeper {
    // структура для получения веса по заданному ребру
    // да понимаю можно было использовать хеши но мне было лень прописывать
    // хеш функции и как-то париться над этот счёт)
   private:
    std::map<EdgeT, std::vector<WeightT>> edge_keeper_;
    size_t num_of_edges_ = 0;

   public:
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
  };

 protected:
  EdgeKeeper edge_keeper_;
  size_t num_of_vertexes_ = 0;

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
};

template <typename VertexT = size_t,
          typename EdgeT = std::pair<VertexT, VertexT>,
          typename WeightT = int64_t>
class AdjListGraph : public AbstructGraph<VertexT, EdgeT, WeightT> {
  std::map<VertexT, std::set<VertexT>> graph_;
  std::set<std::pair<WeightT, EdgeT>> edges_;
  std::set<VertexT> vertexes_;

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
};

template <typename VertexT = size_t,
          typename EdgeT = std::pair<VertexT, VertexT>,
          typename WeightT = int64_t>
class DSU {
  std::map<VertexT, VertexT> body_;
  std::map<VertexT, uint64_t> ranks_;
  std::map<VertexT, WeightT> bands_;
  uint64_t total_val_ = 0;

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
    total_val_ += unit.first;
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
  uint64_t GetTotalWeight() { return total_val_; }
};

class Solution {
 private:
  DSU<> dsu_;

 public:
  Solution(const size_t& num_of_vertexes) {
    std::set<unsigned long> vertexes;
    for (uint32_t i = 0; i <= num_of_vertexes; ++i) {
      vertexes.emplace(i);
    }
    dsu_.SetVertexSet(vertexes);
  }
};

template <typename VertexT = size_t,
          typename EdgeT = std::pair<VertexT, VertexT>,
          typename WeightT = int64_t>
void ToSort(std::vector<std::pair<EdgeT, WeightT>>& edges) {
  std::sort(edges.begin(), edges.end(),
            [](const std::pair<EdgeT, WeightT>& left,
               const std::pair<EdgeT, WeightT>& right) {
              return (left.second == right.second) ? left.first < right.first
                                                   : left.second < right.second;
            });
}

template <typename VertexT = size_t,
          typename EdgeT = std::pair<VertexT, VertexT>,
          typename WeightT = int64_t>
uint64_t TotalWeight(const std::set<VertexT>& vertexes,
                     std::vector<std::pair<EdgeT, WeightT>>& edges) {
  DSU<VertexT, EdgeT, WeightT> dsu(vertexes);
  ToSort(edges);
  for (const auto& idx : edges) {
    dsu.Unite({idx.second, idx.first});
  }
  return dsu.GetTotalWeight();
}

int main() {
  uint32_t num_of_vertexes = 0;
  uint32_t num_of_edges = 0;
  std::cin >> num_of_vertexes >> num_of_edges;
  std::vector<std::pair<std::pair<uint32_t, uint32_t>, uint32_t>> edges(
      num_of_edges);
  for (uint32_t i = 0; i < num_of_edges; ++i) {
    std::cin >> edges[i].first.first >> edges[i].first.second >>
        edges[i].second;
    --edges[i].first.first;
    --edges[i].first.second;
  }
  std::set<uint32_t> vertexes;
  for (size_t i = 0; i < num_of_vertexes; ++i) {
    vertexes.emplace(i);
  }
  std::cout << TotalWeight(vertexes, edges);
}
