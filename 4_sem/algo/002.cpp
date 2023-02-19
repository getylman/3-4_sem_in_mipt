#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>

template <typename VertexT = size_t,
          typename EdgeT = std::pair<VertexT, VertexT>>
class Graph {
 public:
  virtual size_t NumOfVertexes() const = 0;
  virtual size_t NumOfEdges() const = 0;
  virtual std::vector<VertexT*>& GetNeighbours(const VertexT&) = 0;
  // virtual method - get by vertex iterator to its neighbours
  virtual ~Graph() = default;
};

template <typename VertexT = size_t,
          typename EdgeT = std::pair<VertexT, VertexT>>
class AMGraph : public Graph<VertexT, EdgeT> {
  // adj matrix
 public:
  // AMGraph(const size_t& num_of_vertexes, const std::vector<EdgeT>& edges)
  //     : num_of_edges_(edges.size()), num_of_vertexes_(num_of_vertexes);
  // size_t NumOfVertexes() const final { return graph_.size(); }
  // size_t NumOfEdges() const final { return num_of_edges_; }
  // std::vector<VertexT*>& GetNeighbours(const VertexT& vertex);
};

template <typename VertexT = size_t,
          typename EdgeT = std::pair<VertexT, VertexT>>
class ALGraph : public Graph<VertexT, EdgeT> {
  // adj list
  enum Color { WHITE, GRAY, BLACK };
  struct NodeEdge {
    EdgeT value;                 // значение ребра
    size_t weight = 1;           // вес
    size_t number = size_t(-1);  // его номер
    std::pair<VertexT, VertexT> connection;  // какие вершины он соединяет
  };
  struct NodeVertex {
    VertexT value;               // знвчение вершины
    size_t number = size_t(-1);  // его номер
    size_t t_in = size_t(-1);    // время захода
    size_t t_in_min = size_t(-1);  // минимальное время захода
    size_t t_out = size_t(-1);  // время выхода
    size_t t_out_min = size_t(-1);  // минимальное время выхода
    Color color = Color::WHITE;  // цвет вершины
    std::unordered_map<VertexT, std::vector<NodeEdge>>
        neighbours;  // для случая с кратными ребрами
  };
  NodeEdge SetNodeEdge(const EdgeT& value, const size_t& number,
                       const std::pair<size_t, size_t>& connection,
                       const size_t& weight = 1) { // для создания ребра
                                                   // мне на тот момент не хотелось писать конструктор
                                                   // может и напишу но позже
    NodeEdge node;
    node.connection = connection;
    node.number = number;
    node.value = value;
    node.weight = weight;
    return node;
  }

  std::unordered_map<VertexT, NodeVertex>
      graph_;  // храню граф как список смежности и по запросу на
               // значение вершины могу определить его соседей и не только
  // std::vector<EdgeT> edges_; // пока не знаю зачем
  size_t num_of_edges_ = 0; // количество ребер
  size_t num_of_vertexes_ = 0; // количество вершин

 public:
  using vertex_type = VertexT;
  using edge_type = EdgeT;
  ALGraph(const bool& directed, const size_t& num_of_vertexes,
          const std::vector<EdgeT>& edges)
      : num_of_edges_(edges.size()),
        num_of_vertexes_(num_of_vertexes),
        edges_(edges) { // конструктор графа от вектора ребер
    size_t counter_edge = 0;
    size_t counter_vertex = 0;
    std::pair<vertex_type, vertex_type> tmp_edge;
    NodeVertex tmp_node_vertex;
    NodeEdge tmp_node_edge;
    if (directed) {  // для ориентированного графа
      for (const auto& i : edges) {
        tmp_edge = reinterpret_cast<std::pair<vertex_type, vertex_type>>(i);
        // если не скастится то не мои проблемы)))
        if (graph_.count(tmp_edge.first) != 0) {
          if (graph_[tmp_edge.first].neighbours.count(tmp_edge.second) != 0) {
            graph_[tmp_edge.first].neighbours[tmp_edge.second].push_back(
                SetNodeEdge(i, ++counter_edge, tmp_edge));
          } else {
            graph_[tmp_edge.first].neighbours.insert(
                {tmp_edge.second, {SetNodeEdge(i, ++counter_edge, tmp_edge)}});
          }
        } else {
          tmp_node_vertex.value = tmp_edge.first;
          tmp_node_vertex.number = ++counter_vertex;
          tmp_node_vertex.neighbours.insert(
              {tmp_edge.second, {SetNodeEdge(i, ++counter_edge, tmp_edge)}});
          graph_.insert({tmp_edge.first, tmp_node_vertex});
        }
      }
    } else {  // для не ориентированного
      for (const auto& i : edges) {
        tmp_edge = reinterpret_cast<std::pair<vertex_type, vertex_type>>(i);
        // если не скастится то не мои проблемы)))
        if (graph_.count(tmp_edge.first) != 0) { // для случая когда есть первая вершина
          if (graph_[tmp_edge.first].neighbours.count(tmp_edge.second) != 0) {
            // для случая когда есть вторая вершина
            graph_[tmp_edge.first].neighbours[tmp_edge.second].push_back(
                SetNodeEdge(i, ++counter_edge, tmp_edge));
          } else {
            graph_[tmp_edge.first].neighbours.insert(
                {tmp_edge.second, {SetNodeEdge(i, ++counter_edge, tmp_edge)}});
          }
        } else {
          tmp_node_vertex.value = tmp_edge.first;
          tmp_node_vertex.number = ++counter_vertex;
          tmp_node_vertex.neighbours.insert(
              {tmp_edge.second, {SetNodeEdge(i, ++counter_edge, tmp_edge)}});
          graph_.insert({tmp_edge.first, tmp_node_vertex});
        }
        if (graph_.count(tmp_edge.second) != 0) {
          if (graph_[tmp_edge.second].neighbours.count(tmp_edge.first) != 0) {
            graph_[tmp_edge.second].neighbours[tmp_edge.first].push_back(
                SetNodeEdge(i, ++counter_edge,
                            {tmp_edge.second, tmp_edge.first}));
          } else {
            graph_[tmp_edge.second].neighbours.insert(
                {tmp_edge.first,
                 {SetNodeEdge(i, ++counter_edge,
                              {tmp_edge.second, tmp_edge.first})}});
          }
        } else {
          tmp_node_vertex.value = tmp_edge.second;
          tmp_node_vertex.number = ++counter_vertex;
          tmp_node_vertex.neighbours.insert(
              {tmp_edge.first,
               {SetNodeEdge(i, ++counter_edge,
                            {tmp_edge.second, tmp_edge.first})}});
          graph_.insert({tmp_edge.first, tmp_node_vertex});
        }
      }
    }
  }
  size_t NumOfVertexes() const final {
    return graph_.size();
  }
  size_t NumOfEdges() const final {
    return num_of_edges_;
  }
  std::vector<VertexT*>& GetNeighbours(const VertexT& vertex) final {
    return graph_[vertex];
  }
};

int main() {
}