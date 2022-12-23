#include <iostream>
#include <vector>

int main() {
  size_t num_of_cor = 0;
  double x_cop = 0;  // x coord of pocemon
  double y_cop = 0;  // y coord of pocemon
  std::cin >> num_of_cor >> x_cop >> y_cop;
  std::vector<std::pair<double, double>> field(num_of_cor);
  double x_coc = 0;  // x coord of corner
  double y_coc = 0;  // y coord of corner
  for (size_t i = 0; i < num_of_cor; ++i) {
    std::cin >> x_coc >> y_coc;
    field[i] = {x_coc, y_coc};
  }
  bool ans = false;
  for (size_t i = 0, j = num_of_cor - 1; i < num_of_cor; j = i++) {
    if ((((field[i].second <= y_cop) && (y_cop < field[j].second)) ||
         ((field[j].second <= y_cop) && (y_cop < field[i].second))) &&
        (((field[j].second - field[i].second) != 0) && // таска А на 8 тесте падает блять
         (x_cop >
          ((field[j].first - field[i].first) * (y_cop - field[i].second) /
               (field[j].second - field[i].second) +
           field[i].first))))
      ans = !ans;
  }
  std::cout << ((ans) ? "YES" : "NO");
}