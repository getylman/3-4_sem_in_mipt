#include <cmath>
#include <iostream>
int main() {
  uint32_t size = 0;
  std::cin >> size;
  double* pr_arr = new double[size + 1];
  double num = 0;
  pr_arr[0] = 0;
  for (uint32_t i = 0; i < size; ++i) {
    std::cin >> num;
    pr_arr[i + 1] = std::log(num) + pr_arr[i];
  }
  uint32_t num_of_calls = 0, lt = 0, rt = 0;
  std::cin >> num_of_calls;
  std::cout << std::fixed;
  std::cout.precision(10);
  for (uint32_t i = 0; i < num_of_calls; ++i) {
    std::cin >> lt >> rt;
    std::cout << std::exp((pr_arr[rt + 1] - pr_arr[lt]) / (rt + 1 - lt))
              << std::endl;
  }
  delete [] pr_arr;
}