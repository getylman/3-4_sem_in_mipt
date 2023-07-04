#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>
// 2a
template <typename VertexT = size_t,
          typename EdgeT = std::pair<VertexT, VertexT>>
class AbstractGraph {
  virtual size_t NumOfVertexes() const = 0;
  virtual size_t NumOfEdges() const = 0;
  virtual typename std::map<VertexT, std::set<EdgeT>>& GetNeighbours(
      const VertexT&) = 0;
  virtual
      typename std::map<VertexT, std::map<VertexT, std::set<EdgeT>>>::iterator
      GetIteratorOfNeighbours(const VertexT&) = 0;
  virtual ~AbstractGraph() = default;
  using vertex_type = VertexT;
  using edge_type = EdgeT;
};

template <typename VertexT = size_t,
          typename EdgeT = std::pair<VertexT, VertexT>>
class ALGraph : public AbstractGraph<VertexT, EdgeT> {
  // ALGraph -> adjacency list graph
 public:
  ALGraph(const size_t& num_of_vertexes, const std::vector<EdgeT>& edges,
          const bool& is_directed = false)
      : num_of_edges_(edges.size()), num_of_vertexes_(num_of_vertexes) {
    BuildMultiGraph<is_directed>(edges);
  }
  ~ALGraph() = default;
  size_t NumOfVertexes() const final {
    return num_of_vertexes_;
  }
  size_t NumOfEdges() const final {
    return num_of_edges_;
  }
  typename std::map<VertexT, std::set<EdgeT>>& GetNeighbours(
      const VertexT& vertex) final {
    return graph_.at(vertex);
  }
  typename std::map<VertexT, std::map<VertexT, std::set<EdgeT>>>::iterator
  GetIteratorOfNeighbours(const VertexT& vertex) final {
    return graph_.at(vertex).begin();
  }

 private:
  // хотелось конечно использовать хештаблицу но ничего не работало и смерился с
  // рбт
  std::map<VertexT, std::map<VertexT, std::set<EdgeT>>> graph_;
  size_t num_of_edges_ = 0;
  size_t num_of_vertexes_ = 0;
  // тобиш какая вершина с какой связана и с какими ребрами она связана
  template <bool IsDirected>
  void BuildMultiGraph(const std::vector<EdgeT>& edges) {
    for (const auto& i : edges) {
      if (graph_.count(i.first.first) != 0) {
        if (graph_[i.first.first].count(i.first.second) != 0) {
          graph_[i.first.first][i.first.second].insert(i);
        } else {
          graph_[i.first.first].insert({{i.first.second, {i}}});
        }
      } else {
        graph_.insert({i.first.first, {{i.first.second, {i}}}});
      }
      if (!IsDirected) {
        if (graph_.count(i.first.second) != 0) {
          if (graph_[i.first.second].count(i.first.first) != 0) {
            graph_[i.first.second][i.first.first].insert(i);
          } else {
            graph_[i.first.second].insert({{i.first.first, {i}}});
          }
        } else {
          graph_.insert({i.first.second, {{i.first.first, {i}}}});
        }
      }
    }
  }
};

template <typename DistanceT>
DistanceT MaxValueOFDistanceType() {
  if constexpr (std::is_same_v<int64_t, DistanceT>) {
    return INT64_MAX;
  }
  // для остальных типов было впадлу делать
}

template <typename DistanceT>
DistanceT ZeroValueOFDistanceType() {
  if constexpr (std::is_same_v<int64_t, DistanceT>) {
    return 0;
  }
  // для остальных типов было впадлу делать
}

template <typename VertexT = size_t,
          typename EdgeT = std::pair<VertexT, VertexT>,
          typename DistanceT = int64_t>
class Visitor {
 public:
  Visitor(const std::vector<VertexT>& vertexes) {
    for (const auto& i : vertexes) {
      distance_.insert({i, MaxValueOFDistanceType<DistanceT>});
      parents_.insert({i, VertexT()});
      used_.insert({i, false});
    }
  }
  DistanceT SetDistance(const VertexT& vertex, const DistanceT& distance) {
    return distance_.at(vertex) = distance;
  }
  void SetParent(const VertexT& vertex, const VertexT& parent) {
    parents_.at(vertex) = parent;
  }
  void SetUsed(const VertexT& vertex, const bool& used) {
    used_.at(vertex) = used;
  }
  bool GetUsed(const VertexT& vertex) const {
    return used_.at(vertex);
  }
  DistanceT GetDistance(const VertexT& vertex) const {
    return distance_.at(vertex);
  }
  std::map<VertexT, DistanceT>& GetAllDistance() const {
    return distance_;
  }

 private:
  using vertex_type = VertexT;
  using edge_type = EdgeT;
  using distance_type = DistanceT;
  std::map<VertexT, VertexT> parents_;
  std::map<VertexT, DistanceT> distance_;
  std::map<VertexT, bool> used_;
};

template <typename VertexT, typename EdgeT = std::pair<VertexT, VertexT>,
          typename DistanceT = int64_t>
class Solution {
 public:
  Solution(const size_t& num_of_vertexes, const std::vector<EdgeT>& edges,
           const std::vector<VertexT>& vertexes)
      : graph_(num_of_vertexes, edges), visitor_(vertexes) {
  }
  ~Solution() = default;
  void Answer(const VertexT& start) {
    Dijkstra(start);
    for (const auto& i : visitor_.GetAllDistance()) {
      std::cout << i.second << ' ';
    }
    std::cout << '\n';
  }

 private:
  ALGraph<VertexT, EdgeT> graph_;
  Visitor<VertexT, EdgeT, DistanceT> visitor_;
  using HeapNode = std::pair<DistanceT, VertexT>;
  std::set<HeapNode> heap_;

  static HeapNode ExtractMin(std::set<HeapNode>& heap) {
    HeapNode extracted_elem = *heap.rbegin();
    heap.erase(extracted_elem);
    return extracted_elem;
  }

  void Dijkstra(const VertexT& start) {
    visitor_.SetDistance(start, ZeroValueOFDistanceType);
    heap_.insert({ZeroValueOFDistanceType, start});
    VertexT top_of_heap;
    while (!heap_.empty()) {
      top_of_heap = ExtractMin(heap_).second();
      if (visitor_.GetUsed(top_of_heap)) {
        continue;
      }
      visitor_.SetUsed(top_of_heap, true);
      for (const auto& i : graph_.GetNeighbours(top_of_heap)) {
        if (*(i.second.begin()).second + visitor_.GetDistance(top_of_heap) <
            visitor_.GetDistance(i.first)) {
          heap_.insert({-(visitor_.SetDistance(
                            i.first, visitor_.GetDistance(top_of_heap) +
                                         *(i.second.begin()).second)),
                        i.first});
        }
      }
    }
  }
};

std::vector<uint64_t> IntigerVertexes(const size_t& number) {
  std::vector<uint64_t> result(number);
  for (size_t i = 0; i < number; ++i) {
    result[i] = i;
  }
  return result;
}

void KillAllDoubleEdgesAndLoops(
    std::vector<std::pair<std::pair<uint64_t, uint64_t>, int64_t>>& edges) {
  std::map<std::pair<uint64_t, uint64_t>, int64_t> tmp_edges;
  for (const auto& i : edges) {
    if (i.first.first == i.first.second) {
      continue;
    }
    if (tmp_edges.count(i.first) == 0) {
      tmp_edges.emplace(i);
    } else {
      tmp_edges[i.first] = std::min(tmp_edges[i.first], i.second);
    }
    if (tmp_edges.count({i.first.second, i.first.first}) == 0) {
      tmp_edges.insert({{i.first.second, i.first.first}, i.second});
    } else {
      tmp_edges[{i.first.second, i.first.first}] = std::min(
          tmp_edges[std::make_pair(i.first.second, i.first.first)], i.second);
    }
  }
  edges.clear();
  for (const auto& i : tmp_edges) {
    edges.push_back(i);
  }
}

int main() {
  size_t num_of_proccess = 0;
  std::cin >> num_of_proccess;
  size_t num_vertexes = 0;
  size_t num_edges = 0;
  size_t start_vertex = 0;
  std::pair<std::pair<uint64_t, uint64_t>, int64_t> input_edge;
  std::vector<std::pair<std::pair<uint64_t, uint64_t>, int64_t>> edges;
  while (num_of_proccess-- > 0) {
    std::cin >> num_vertexes >> num_edges;
    edges.reserve(num_edges);
    while (num_edges-- > 0) {
      std::cin >> input_edge.first.first >> input_edge.first.second >>
          input_edge.second;
      edges.emplace_back(input_edge);
    }
    KillAllDoubleEdgesAndLoops(edges);
    std::cin >> start_vertex;
    Solution<uint64_t, std::pair<std::pair<uint64_t, uint64_t>, int64_t>,
             int64_t> sol(num_vertexes, edges, IntigerVertexes(num_vertexes));
    sol.Answer(start_vertex);
  }
}