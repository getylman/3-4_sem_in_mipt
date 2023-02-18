#include <iostream>
#include <vector>

int Split(std::vector<int>& arr, int lt, int rt) {
  int mid = lt - 1;
  int pivot_id = (rand() % (rt - lt)) + lt;
  if (rt - lt != 0) {
    std::swap(arr[pivot_id], arr[rt]);
  }
  int pivot = arr[rt];
  for (int i = lt; i <= rt; i++) {
    if (arr[i] <= pivot) {
      std::swap(arr[i], arr[++mid]);
    }
  }
  return mid;
}

int Search(std::vector<int>& arr, int elem) {
  int lt = 0, rt = arr.size() - 1;
  if (rt == lt + 1) {
    return arr[elem];
  }
  while (true) {
    int id = Split(arr, lt, rt);
    if (id < elem) {
      lt = id + 1;
    } else if (id == elem) {
      return arr[id];
    } else {
      rt = id - 1;
    }
  }
}

int main() {
  int size = 0, num = 0, arr0 = 0, arr1 = 0, arr2 = 0;
  std::cin >> size >> num >> arr0 >> arr1;
  std::vector<int> arr;
  arr.reserve(size);
  arr.push_back(arr0);
  arr.push_back(arr1);
  for (int i = 2; i < size; i++) {
    arr2 = (arr1 * 123 + arr0 * 45) % (10004321);
    arr.push_back(arr2);
    arr0 = arr1;
    arr1 = arr2;
  }
  std::cout << Search(arr, num - 1);
}