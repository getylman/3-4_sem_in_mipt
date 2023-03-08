#include <algorithm>
// #include <deque>
#include <iostream>
#include <set>
#include <vector>
using namespace std;
const int64_t kMaxWay = 1000000;
std::vector<std::vector<pair<int64_t, pair<int64_t, int64_t>>>> adj_list;
int64_t min_way = kMaxWay;
vector<int64_t> res_way;
#include <unordered_set>
// map<size_t, pair<int64_t, int64_t>> answer;




void Djcstr(int64_t start, int64_t limit, vector<int64_t>& answer) {
  std::vector<pair<int64_t, int64_t>> dest((int64_t)adj_list.size(),
                                           {kMaxWay, 0});
  set<std::pair<int64_t, int64_t>> queue;
  dest[start].first = 0;
  vector<int64_t> parrent;
  parrent.resize(adj_list.size(), 0);
  std::pair<int64_t, int64_t> start_point64_t{dest[start].first, start};
  queue.insert(start_point64_t);
  while (!queue.empty()) {
    int64_t current = queue.begin()->second;
    queue.erase(queue.begin());
    for (int64_t i = 0; i < (int64_t)adj_list[current].size(); ++i) {
      int64_t to = adj_list[current][i].first - 1;
      int64_t time = adj_list[current][i].second.second;
      int64_t weight = adj_list[current][i].second.first;
      if (dest[current].first + time < dest[to].first) {
        queue.erase({dest[to].first, to});
        dest[to].first = dest[current].first + time;
        dest[to].second = dest[current].second + weight;
        parrent[to] = current;
        queue.insert({dest[to].first, to});
      }
    }
  }

  if (dest[adj_list.size() - 1].first <= limit) {
    for (int64_t i = (int64_t)adj_list.size() - 1; i > 0;
         i = (int64_t)parrent[i]) {
      answer.push_back(i + 1);
    }
    answer.push_back(1);
    min_way = dest[adj_list.size() - 1].second;
    reverse(answer.begin(), answer.end());
  }
}

int64_t limit;
void Dfs(int64_t current, int64_t way_cost, int64_t time,
         vector<int64_t> answer, unordered_set<int64_t> used) {
  used.insert(current);
  answer.push_back(current + 1);
  if (current + 1 == (int64_t)adj_list.size()) {
    if (time <= limit) {
      min_way = way_cost;
      res_way = answer;
    }
    return;
  }
  for (auto& i : adj_list[current]) {
    int64_t to = i.first - 1;
    if (used.find(to) != used.end()) {
      continue;
    }
    if (time + i.second.second > limit) {
      continue;
    }

    if (min_way <= way_cost + i.second.first) {
      continue;
    }

    Dfs(to, way_cost + i.second.first, time + i.second.second, answer, used);
  }
}

int main() {
  /*std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  */
  int64_t vertex_number, edge_number;
  std::cin >> vertex_number >> edge_number >> limit;
  int64_t first, second, time;
  int64_t weight;
  adj_list.resize(vertex_number);
  for (int64_t i = 0; i < edge_number; ++i) {
    std::cin >> first >> second >> weight >> time;
    adj_list[first - 1].push_back({second, {weight, time}});
    adj_list[second - 1].push_back({first, {weight, time}});
  }
  vector<int64_t> answer;
  Djcstr(0, limit, answer);
  if (!answer.empty()) {
    vector<int64_t> answer_dfs;
    unordered_set<int64_t> used;

    Dfs(0, 0, 0, answer_dfs, used);
    if (min_way < kMaxWay) {
      cout << min_way << '\n';
      if (!res_way.empty()) {
        cout << res_way.size() << '\n';
        for (auto j : res_way) {
          std::cout << j << ' ';
        }
      } else {
        cout << answer.size() << '\n';
        for (auto j : answer) {
          cout << j << ' ';
        }
      }
    }
  } else {
    std::cout << -1;
  }
}