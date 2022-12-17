#include <iostream>

bool BinarySearch(const int* begin, const int* end, int target) {
  uint32_t left = 0;
  uint32_t right = end - begin - 1;
  uint32_t mid;
  while (right >= left) {
    mid = left + (right - left) / 2;
    if (begin[mid] == target) {
      return true;
    }
    if (begin[mid] > target) {
      right = mid - 1;
    } else {
      left = mid + 1;
    }
  }
  return false;
}

int main() {
  uint32_t size_of_array;
  std::cin >> size_of_array;
  int* arr = new int[size_of_array];
  for (uint32_t i = 0; i < size_of_array; ++i) {
    std::cin >> arr[i];
  }
  uint32_t number_of_calls = 0, left = 0, right = 0;
  int target = 0;
  std::cin >> number_of_calls;
  for (uint32_t i = 0; i < number_of_calls; ++i) {
    std::cin >> left >> right >> target;
    if (BinarySearch(arr + left, arr + right, target)) {
      std::puts("YES");
    } else {
      std::puts("NO");
    }
  }
  delete [] arr;
  return 0;
}