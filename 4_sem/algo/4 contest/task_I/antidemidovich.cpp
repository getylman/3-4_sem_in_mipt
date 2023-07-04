#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <vector>

struct Solution {
 private:
  struct Edge {
    uint16_t src = 0;
    uint16_t dst = 0;
    int64_t weight = 0;
    Edge(const uint16_t& src, const uint16_t& dst, const int64_t& weight)
        : src(src), dst(dst), weight(weight) {}
  };
  struct TestType {
    const uint16_t kNumOfVertexes = 0;
    std::vector<std::vector<Edge>> arr_g;
    std::vector<bool> arr_u;
    std::vector<int64_t> arr_w;
    TestType(const uint16_t& num_vertexes)
        : kNumOfVertexes(num_vertexes),
          arr_g(2 * (num_vertexes + 1),
                std::vector<Edge>(2 * (num_vertexes + 1), Edge(0, 0, 0))),
          arr_u(num_vertexes + 1),
          arr_w(num_vertexes + 1) {}
  };
  TestType body_;

 public:
  Solution(const uint16_t& num_of_vertexes) : body_(num_of_vertexes) {}
  void SetVert(const int16_t& vert, const uint16_t& index) {
    body_.arr_w[index] = vert;
    // (vert < 0);
    body_.arr_g[(vert < 0) ? index : 0]
               [(vert < 0) ? body_.kNumOfVertexes * 2 + 1 : index] =
        (vert < 0) ? Edge(index, body_.kNumOfVertexes * 2 + 1, -vert)
                   : Edge(0, index, vert);
  }
  void SetInG(const uint16_t& src, const uint16_t& dst) {
    body_.arr_g[src][dst] = Edge(src, dst, INT64_MAX);
  }

 private:
  uint64_t GetAns() const {
    uint64_t res = 0;
    for (const auto& idx : body_.arr_w) {
      res += (idx > 0) ? idx : 0;
    }
    return res;
  }
  void MakeEdge(const uint16_t& src, const uint16_t& dst) {
    body_.arr_g[src][dst] =
        (body_.arr_g[src][dst].dst != 0 or body_.arr_g[src][dst].src != 0 or
         body_.arr_g[src][dst].weight != 0)
            ? body_.arr_g[src][dst]
            : Edge(src, dst, 0);
  }
  int64_t Dfs(uint16_t cur_ver, int64_t val) {
    if (cur_ver == body_.kNumOfVertexes * 2 + 1) {
      return val;
    }
    body_.arr_u[cur_ver] = static_cast<bool>(1);
    for (auto& idx : body_.arr_g[cur_ver]) {
      if ((idx.dst == idx.src and idx.dst == static_cast<uint16_t>(0)) or
          body_.arr_u[idx.dst]) {
        continue;
      }
      while (idx.weight > 0) {
        int64_t dfs_res = Dfs(idx.dst, std::min(val, idx.weight));
        while (dfs_res > 0) {
          MakeEdge(idx.dst, cur_ver);
          idx.weight -= dfs_res;
          body_.arr_g[idx.dst][cur_ver].weight += dfs_res;
          return dfs_res;
        }
        break;
      }
    }
    return 0;
  }

 public:
  void MakeLagic() {
    int64_t ansss = 0;
    uint64_t total_anss = GetAns();
    for (;;) {
      int64_t pr_ans = ansss;
      std::fill(body_.arr_u.begin(), body_.arr_u.end(), false);
      ansss += Dfs(0, INT64_MAX);
      if (ansss == pr_ans) {
        std::cout << total_anss - pr_ans;
        return;
      }
    }
  }
  void PrintInside() {
    std::cout << '\n';
    for (const auto& idx : body_.arr_g) {
      for (const auto& jdx : idx) {
        std::cout << '(' << jdx.src << ", " << jdx.dst << ", " << jdx.weight
                  << ")\t";
      }
      std::cout << '\n';
    }
  }
};

void Read() {
  uint16_t big_n = 0;
  std::cin >> big_n;
  Solution sol(big_n);
  uint16_t tmp = big_n;
  int16_t num = 0;
  uint16_t dst_tmp = 0;
  while (tmp-- > (unsigned short)0) {
    std::cin >> num;
    sol.SetVert(num, big_n - tmp);
  }
  tmp = big_n;
  while (tmp-- > (unsigned short)0) {
    std::cin >> num;
    while (num-- > 0) {
      std::cin >> dst_tmp;
      sol.SetInG(big_n - tmp, dst_tmp);
    }
  }
  sol.MakeLagic();
}

int main() { Read(); }