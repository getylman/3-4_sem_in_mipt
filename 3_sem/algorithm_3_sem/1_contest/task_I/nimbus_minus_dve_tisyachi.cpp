#include <iostream>

int main() {
  int size = 0;
  std::cin >> size;
  int arr[100000] = {};
  int pr_min[100000] = {};
  int sf_min[100000] = {};
  for (int i = 0; i < size; i++) {
    std::cin >> arr[i];
    pr_min[i] = (i == 0)
                    ? (arr[i])
                    : ((pr_min[i - 1] < arr[i]) ? (pr_min[i - 1]) : (arr[i]));
  }
  for (int i = size - 1; i >= 0; i--) {
    sf_min[i] = (i == size - 1)
                    ? (arr[i])
                    : ((sf_min[i + 1] < arr[i]) ? (sf_min[i + 1]) : (arr[i]));
  }
  int num_of_calls = 0;
  std::cin >> num_of_calls;
  for (int i = 0; i < num_of_calls; i++) {
    int lt, rt;
    std::cin >> lt >> rt;
    std::cout << ((pr_min[lt - 1] < sf_min[rt - 1]) ? (pr_min[lt - 1])
                                                    : (sf_min[rt - 1]));
    std::cout << std::endl;
  }
}