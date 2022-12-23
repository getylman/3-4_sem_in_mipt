#include <cmath>
#include <iostream>
#include <vector>

std::pair<int, int> MakeVec(const int& x1, const int& y1, const int& x2,
                            const int& y2) {
  return {x2 - x1, y2 - y1};
}

double VecLen(const int& x, const int& y) {
  int64_t x_com = x;
  int64_t y_com = y;
  return sqrt(x_com * x_com + y_com * y_com);
}

double NormField(const std::vector<std::pair<int, int>>& field,
                 const size_t& id1, const size_t& id2, const size_t& id3) {
  const std::pair<int, int> vec1 = MakeVec(field[id1].first, field[id1].second,
                                           field[id2].first, field[id2].second);
  const std::pair<int, int> vec2 = MakeVec(field[id1].first, field[id1].second,
                                           field[id3].first, field[id3].second);
  double scal_mul =
      0.1 * vec1.first * vec2.first + 0.1 * vec1.second * vec2.second;
  double corner = acos(scal_mul / (VecLen(vec1.first, vec1.second) *
                                   VecLen(vec2.first, vec2.second)));
  return corner;
}

int main() {
  size_t size = 0;
  std::cin >> size;
  std::vector<std::pair<int, int>> field(size);
  int x_cor = 0;
  int y_cor = 0;
  for (size_t i = 0; i < size; ++i) {
    std::cin >> x_cor >> y_cor; // таска В хз не тестил пока
    field[i] = {x_cor, y_cor};
  }
  std::cout << NormField(field, 0, 1, 2);
}