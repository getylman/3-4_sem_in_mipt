#include <iostream>
#include <unordered_map>
#include <vector>

const int64_t kThree = 3;
const int64_t kInf = 1e9;
int64_t digit;
int64_t vertexes;
std::vector<int64_t> weight;
std::unordered_map<int64_t, std::unordered_map<int64_t, std::vector<int64_t>>>
    graph;
std::unordered_map<int64_t, bool> visited;

std::vector<int64_t> MakeEdge(int64_t vertex1, int64_t vertex2,
                              int64_t weight) {
  std::vector<int64_t> value(kThree, 0);
  value[0] = vertex1;
  value[1] = vertex2;
  value[2] = weight;
  return value;
}

void Insert(int64_t type) {
  if (weight[type] < 0) {
    graph[type][2 * vertexes + 1] =
        MakeEdge(type, 2 * vertexes + 1, -weight[type]);
  } else {
    graph[0][type] = MakeEdge(0, type, weight[type]);
  }
}

void InitializeEdge(int64_t vertex1, int64_t vertex2) {
  if (graph[vertex1][vertex2].empty()) {
    std::vector<int64_t> value;
    value.push_back(vertex1);
    value.push_back(vertex2);
    value.push_back(0);
    graph[vertex1][vertex2] = value;
  }
}

int64_t DFS(int64_t vertex, int64_t minimal) {
  if (vertex == 2 * vertexes + 1) {
    return minimal;
  }
  visited[vertex] = true;
  for (auto& it : graph[vertex]) {
    if (it.second.empty()) {
      continue;
    }
    if (visited[it.second[1]]) {
      continue;
    }
    if (it.second[2] > 0) {
      int64_t digit = DFS(it.second[1], std::min(minimal, it.second[2]));
      if (digit > 0) {
        InitializeEdge(it.second[1], vertex);
        it.second[2] -= digit;
        graph[it.second.at(1)][vertex][2] += digit;
        return digit;
      }
    }
  }
  return 0;
}

void Cin() {
  std::cin >> vertexes;
  weight.resize(vertexes + 1);
  for (int64_t i = 1; i < vertexes + 1; ++i) {
    std::cin >> digit;
    weight[i] = digit;
    Insert(i);
  }
  int64_t amount;
  for (int64_t i = 0; i < vertexes; ++i) {
    std::cin >> amount;
    for (int64_t j = 0; j < amount; ++j) {
      std::cin >> digit;
      graph[i + 1][digit] = MakeEdge(i + 1, digit, kInf);
    }
  }
}

int64_t Answer() {
  int64_t answer_end = 0;
  for (auto it : weight) {
    if (it < 0) {
      continue;
    }
    answer_end += it;
  }
  return answer_end;
}

int main() {
  Cin();
  int64_t answer = 0;
  int64_t answer_end = Answer();
  while (true) {
    int64_t answer_prev = answer;
    answer += DFS(0, kInf);
    visited.clear();
    if (answer == answer_prev) {
      std::cout << answer_end - answer;
      return 0;
    }
  }
}