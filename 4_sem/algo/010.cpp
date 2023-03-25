#include <algorithm>
#include <iostream>
#include <vector>

template <typename T>
class DSU {
 public:
  void Unite(T a, T b) {
    if (a == b) return;
    a = FindLeader(a);
    b = FindLeader(b);
    if (a == b) return;
    if (ranks[a] > ranks[b]) {
      std::swap(a, b);
    }
    ranks[b] += ranks[a];
    pow[b] += pow[a];
    parents[a] = b;
  }

  DSU(T n) {
    parents.resize(n);
    ranks.resize(n);
    pow.resize(n, 0);
    for (int i = 0; i < n; ++i) {
      parents[i] = i;
      ranks[i] = 1;
    }
  }

  size_t GetPower(T elem) {
    return pow[FindLeader(elem)];
  }

  void AddPower(size_t pow_, T elem1) {
    pow[FindLeader(elem1)] += pow_;
  }

 private:
  /// тут поменяй без рекурсии и из привата вытащи
  int FindLeader(T elem) {
    return (parents[elem] == elem) ? elem
                                   : parents[elem] = FindLeader(parents[elem]);
  }
  std::vector<T> parents;
  std::vector<T> ranks;
  std::vector<size_t> pow;
};

struct Solution {
  static void solution() {
    size_t size;
    size_t answers;
    std::cin >> size >> answers;
    DSU<size_t> dsu(size);
    size_t ans = 0;
    size_t vertex1 = 0;
    size_t vertex2 = 0;
    size_t power = 0;
    for (size_t i = 0; i < answers; ++i) {
      std::cin >> ans;
      if (ans == 2) {
        std::cin >> vertex1;
        std::cout << dsu.GetPower(vertex1 - 1) << '\n';
      }
      if (ans == 1) {
        std::cin >> vertex1 >> vertex2 >> power;
        dsu.Unite(vertex1 - 1, vertex2 - 1);
        if (vertex1 != vertex2) dsu.AddPower(power, vertex1 - 1);
      }
    }
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  Solution::solution();
  return 0;
}