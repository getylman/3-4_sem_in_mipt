#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

const int INF = 1e9;

struct Edge {
  int from, to, cost, id;
};

bool operator<(const Edge& a, const Edge& b) { return a.cost < b.cost; }

class Graph {
 private:
  int kNumOfVertexes;
  int kNumOfEdges;
  vector<int> arr1;
  vector<int> arr2;
  vector<vector<int>> arr3;
  vector<int> arr4;

 public:
  Graph(int n, int m) : kNumOfVertexes(n), kNumOfEdges(m), arr1(m), arr2(n), arr3(n, vector<int>(n)), arr4(n) {}

  void ReadGraph() {
    for (int i = 0; i < kNumOfEdges; ++i) {
      int from, to, cost;
      cin >> from >> to >> cost;
      --from, --to;
      arr1[i] = i;
      arr3[from][to] = arr3[to][from] = cost;
    }
  }

  int Find(int x) {
    if (arr4[x] == x) {
      return x;
    }
    return arr4[x] = Find(arr4[x]);
  }

  void Union(int x, int y) {
    x = Find(x);
    y = Find(y);
    if (x != y) {
      arr4[x] = y;
    }
  }

  void Kruskal() {
    vector<Edge> edges;
    for (int i = 0; i < kNumOfVertexes; ++i) {
      for (int j = i + 1; j < kNumOfVertexes; ++j) {
        if (arr3[i][j] != 0) {
          edges.push_back({i, j, arr3[i][j], (int)edges.size()});
        }
      }
    }
    sort(edges.begin(), edges.end());

    for (int i = 0; i < kNumOfVertexes; ++i) {
      arr4[i] = i;
    }

    vector<int> ans;
    int totalcost = 0;
    for (auto edge : edges) {
      if (Find(edge.from) != Find(edge.to)) {
        Union(edge.from, edge.to);
        totalcost += edge.cost;
        if (edge.id != -1) {
          ans.push_back(edge.id);
        }
      }
    }

    cout << ans.size() << " " << totalcost << endl;
    sort(ans.begin(), ans.end());
    for (auto id : ans) {
      cout << id + 1 << " ";
    }
  }
};

int main() {
  int n, m;
  cin >> n >> m;
  Graph graph(n, m);
  graph.ReadGraph();
  graph.Kruskal();
  return 0;
}