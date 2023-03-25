#include <iostream>
#include <vector>
#include <set>

size_t Dfs(size_t current, size_t finish, std::vector<bool> used,
           const std::vector<std::vector<std::pair<size_t, size_t>>> &adj_list, size_t c_min = SIZE_MAX) {
  used[current - 1] = true;
  if (current == finish) {
    return c_min;
  }
  for (auto i: adj_list[current - 1]) {
    if (!used[i.first - 1]) {
      size_t ans = Dfs(i.first, finish, used, adj_list, std::min(c_min, i.second));
      if (ans) {
        return ans;
      }
    }
  }
  return 0;
}

std::vector<std::vector<std::pair<size_t, size_t>>>
Prim(std::vector<std::vector<size_t>> &adj_table, size_t start = 1) {
  std::vector<std::vector<std::pair<size_t, size_t>>> adj_list(adj_table.size());
  std::vector<size_t> dest(adj_table.size(), 0);
  std::set<std::pair<size_t, std::pair<size_t, size_t>>> queue;
  std::vector<bool> used_in_answer(adj_table.size(), false);
  std::vector<bool> selected(adj_table.size(), false);
  std::vector<size_t> parent(adj_table.size(), -1);
  dest[start - 1] = 0;
  parent[start - 1] = 0;
  queue.insert({dest[start - 1], {start, start}});
  std::vector<size_t> answer;
  size_t min = SIZE_MAX;
  while (!queue.empty()) {
    size_t current = queue.rbegin()->second.second;
    if (used_in_answer[current - 1]) {
      queue.erase(--queue.end());
      continue;
    }
    if (queue.rbegin()->first) {
      adj_list[queue.rbegin()->second.first - 1].emplace_back(current, queue.rbegin()->first);
      adj_list[current - 1].emplace_back(queue.rbegin()->second.first, queue.rbegin()->first);
    }
    used_in_answer[current - 1] = true;

    queue.erase(--queue.end());
    selected[current - 1] = true;
    for (size_t i = 0; i < adj_table[current - 1].size(); ++i) {
      if (selected[i]) {
        continue;
      }
      size_t weight = adj_table[current - 1][i];
      queue.insert({weight, {current, i + 1}});
    }
  }
  return adj_list;
}


int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  size_t N, M, K;
  std::cin >> N >> M >> K;
  std::vector<std::vector<size_t>> adj_table(N, std::vector<size_t>(N, 0));
  for (size_t i = 0; i < M; ++i) {
    size_t first, second, capacity;
    std::cin >> first >> second >> capacity;
    adj_table[first - 1][second - 1] = capacity;
    adj_table[second - 1][first - 1] = capacity;
  }
  size_t start, finish;
  std::vector<bool> visited(adj_table.size(), false);
  std::vector<std::vector<std::pair<size_t, size_t>>> adj_list(Prim(adj_table));
  std::vector<bool> used(adj_list.size(), false);
  for (size_t i = 0; i < K; ++i) {
    std::cin >> start >> finish;
    std::cout << Dfs(start, finish, used, adj_list) << '\n';
  }

}