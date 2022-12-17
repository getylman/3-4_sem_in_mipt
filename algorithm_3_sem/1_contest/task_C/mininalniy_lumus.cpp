#include <algorithm>
#include <iostream>

bool IsGoodLineSeg(const int* arr, int size, int coord, int spell) {
  int line_sgmnts = 1;
  int mx = arr[0];
  for (int i = 0; i < size; i++) {
    if (arr[i] > mx + coord) {
      line_sgmnts++;
      mx = arr[i];
    }
  }
  return (line_sgmnts <= spell);
}

void BinSearchOfDist(const int* arr, int size, int spell) {
  int lt = -1;
  int rt = arr[size - 1] - arr[0];
  while (rt - lt > 1) {
    int mid = (rt + lt) / 2;
    if (IsGoodLineSeg(arr, size, mid, spell)) {
      rt = mid;
    } else {
      lt = mid;
    }
  }
  std::cout << ((IsGoodLineSeg(arr, size, lt, spell)) ? (lt) : (rt));
}

int main() {
  int size = 0;
  int spell = 0;
  std::cin >> size >> spell;
  int arr[100003] = {};
  for (int i = 0; i < size; i++) {
    std::cin >> arr[i];
  }
  std::sort(arr, arr + size);
  BinSearchOfDist(arr, size, spell);
}