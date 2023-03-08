#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

class Solution {
 private:
  const uint64_t kInf = INT64_MAX;
  using Edge = std::pair<uint64_t, std::pair<uint64_t, uint64_t>>;
  using Pair = std::pair<uint64_t, uint64_t>;
  struct Node {
    std::vector<std::pair<uint64_t, std::pair<uint64_t, uint64_t>>> neighbours;
    uint64_t value = 0;
    uint64_t dist = INT64_MAX;
    uint64_t parent = 0;
    uint64_t time = 0;
    bool used = false;

   private:
    // const int64_t kInf = int64_t(-1);
  };
  std::vector<Node> adj_list_;
  std::vector<uint64_t> result_1_;
  std::vector<uint64_t> result_2_;
  std::vector<uint64_t> total_res_;
  size_t main_time_ = 0;
  size_t lower_dist_ = kInf;

  void BuildAdjList(
      const size_t& vertexes,
      const std::vector<std::pair<std::pair<uint64_t, uint64_t>,
                                  std::pair<uint64_t, uint64_t>>>& edges,
      const size_t& main_time) {
    main_time_ = main_time;
    adj_list_.resize(vertexes);
    for (size_t i = 0; i < vertexes; ++i) {
      adj_list_[i].value = i;
      adj_list_[i].dist = kInf;
      adj_list_[i].used = false;
    }
    for (const auto& i : edges) {
      adj_list_[i.first.first - 1].neighbours.push_back(
          {i.first.second, i.second});
      adj_list_[i.first.second - 1].neighbours.push_back(
          {i.first.first, i.second});
    }
  }

  static Pair ExtractMin(std::set<Pair>& heap) {
    Pair extracted_elem = *heap.begin();
    heap.erase(extracted_elem);
    return extracted_elem;
  }
  static void Reverse(std::vector<uint64_t>& vec) {
    std::reverse(vec.begin(), vec.end());
  }

  void Dijkstra(const size_t& start) {
    std::set<Pair> heap;
    adj_list_[start].time = 0;
    heap.insert({0, start});
    uint64_t top_of_heap;
    for (; !heap.empty();) {
      top_of_heap = ExtractMin(heap).second;
      for (const auto& i : adj_list_[top_of_heap].neighbours) {
        if (adj_list_[top_of_heap].time + i.second.second <
            adj_list_[i.first - 1].time) {
          heap.erase({adj_list_[i.first - 1].time, i.first - 1});
          adj_list_[i.first - 1].dist =
              adj_list_[top_of_heap].dist + i.second.first;
          adj_list_[i.first - 1].time =
              adj_list_[top_of_heap].time + i.second.second;
          adj_list_[i.first - 1].parent = top_of_heap;
          heap.insert({adj_list_[i.first - 1].time, i.first - 1});
        }
      }
    }
    if (adj_list_[adj_list_.size() - 1].time <= main_time_) {
      for (size_t i = adj_list_.size() - 1; i != 0; i = adj_list_[i].parent) {
        result_1_.push_back(i + 1);
      }
      result_1_.push_back(1);
      lower_dist_ = adj_list_.back().dist;
      Reverse(result_1_);
    }
  }

  void DFS(Node* cur, Pair lims) {
    adj_list_[cur->value].used = true;
    result_2_.push_back(cur->value + 1);
    if (cur->value + 1 != adj_list_.size()) {
      for (const auto& i : adj_list_[cur->value].neighbours) {
        if (adj_list_[i.first - 1].used or
            lims.second + i.second.second > main_time_ or
            lower_dist_ <= lims.first + i.second.first) {
          continue;
        }
        DFS(&adj_list_[i.first - 1], {lims.first + i.second.first, lims.second + i.second.second});
      }
    } else {
      if (lims.second <= main_time_) {
        lower_dist_ = lims.first;
        total_res_ = result_2_;
      }
    }
  }

 public:
  void Answer(const size_t& num_of_vertexes, const std::vector<std::pair<std::pair<uint64_t, uint64_t>,
                                  std::pair<uint64_t, uint64_t>>>& edges, const size_t& main_time) {
    BuildAdjList(num_of_vertexes, edges, main_time);
    Dijkstra(0);
    if (result_1_.empty()) {
      std::puts("-1");
    } else {
      DFS(&adj_list_[0], {0, 0});
      if (lower_dist_ < kInf) {
        std::puts(std::to_string(lower_dist_).data());
        if (total_res_.empty()) {
          std::puts(std::to_string(result_1_.size()).data());
          for (const auto& i : result_1_) {
            std::cout << i << ' ';
          }
          std::cout << "-----";
        } else {
          std::puts(std::to_string(total_res_.size()).data());
          for (const auto& i : total_res_) {
            std::cout << i << ' ';
          }
          std::cout << "++++++";
        }
      }
    }
  }
};

int main() {
  size_t num_of_vertexes = 0;
  size_t num_of_edges = 0;
  size_t main_time = 0;
  std::cin >> num_of_vertexes >> num_of_edges >> main_time;
  std::vector<
      std::pair<std::pair<uint64_t, uint64_t>, std::pair<uint64_t, uint64_t>>>
      edges(num_of_edges);
  for (auto& i : edges) {
    std::cin >> i.first.first >> i.first.second >> i.second.first >>
        i.second.second;
  }
  Solution().Answer(num_of_vertexes, edges, main_time);
}