#include <iostream>

uint32_t LoopSearcher(const int* begin, const int* end) {
  uint32_t left = 0;
  uint32_t right = end - begin - 1;
  uint32_t mid = 0;
  while (right >= left) {
    if (right - left == 1) {
      return (begin[left] > begin[right]) ? right : left;
    }
    if (begin[left] > begin[right]) {
      mid = left + (right - left) / 2;
      if (begin[mid] < begin[right]) {
        right = mid;
      } else {
        left = mid;
      }
    } else {
      break;
    }
  }
  return left;
}

int main() {
  uint32_t size_of_array = 0;
  std::cin >> size_of_array;
  int* arr = new int[size_of_array];
  for (uint32_t i = 0; i < size_of_array; ++i) {
    std::cin >> arr[i];
  }
  std::cout << LoopSearcher(arr, arr + size_of_array);
  delete[] arr;
  return 0;
}