#include <iostream>
#include <map>
#include <set>
#include <vector>

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);
  size_t num_of_g_s = 0;
  size_t num_roads = 0;
  size_t max_length = 0;
  std::cin >> num_of_g_s >> num_roads >> max_length;
  std::map<size_t, std::set<std::pair<size_t, size_t>>> gas_st;
  size_t x_cor = 0;
  size_t start = 0;
  size_t end;
  for (size_t i = 0; i < num_of_g_s; ++i) {
    std::cin >> x_cor >> start >> end;
    gas_st[x_cor].insert({start, end});
  }
  gas_st[max_length].insert({0, num_roads});
  std::vector<std::pair<std::pair<size_t, size_t>, size_t>> roads;
  roads.reserve(num_roads + 1);
  for (size_t i = 0; i <= num_roads; ++i) {
    roads.push_back({{0, 0}, 0});
  }
  for (auto i : gas_st) {
    for (auto j : i.second) {
      for (size_t k = j.first; k <= j.second; ++k) {
        roads[k].second = std::max(roads[k].second, i.first - roads[k].first.second);
        roads[k].first.first = roads[k].first.second;
        roads[k].first.second = i.first;
      }
    }
  }
  for (size_t i = 0; i < roads.size(); ++i) {
    std::cout << roads[i].second << "\n";
  }
  /*
  for (size_t i = 1; i <= max_length; ++i) {
    for (size_t j = 0; j <= num_roads; ++j) {
      ++roads[j].first;
      roads[j].second = std::max(roads[j].first, roads[j].second);
    }
    if (gas_st.count(i) != 0) {
      for (auto j : gas_st.at(i)) {
        for (size_t k = j.first; k <= j.second; ++k) {
          roads[k].first = 0;
        }
      }
    }*/
  //}
  //for (size_t i = 0; i < roads.size(); ++i) {
  //  std::cout << roads[i].second << std::endl;
  //}
}