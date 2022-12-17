#include <iostream>
#include <vector>

void Merge(std::vector<int>& arr, int lt, int mid, int rt, size_t& counter) {
  int i = lt, j = mid + 1, k = lt;
  std::vector<int> tmp_arr;
  tmp_arr.reserve(arr.size());
  while (i <= mid && j <= rt) {
    if (arr[i] <= arr[j]) {
      tmp_arr[k++] = arr[i++];
    } else {
      counter += mid - i + 1;
      tmp_arr[k++] = arr[j++];
    }
  }
  while (i <= mid) {
    tmp_arr[k++] = arr[i++];
  }
  while (j <= rt) {
    tmp_arr[k++] = arr[j++];
  }
  for (i = lt; i <= rt; i++) {
    arr[i] = tmp_arr[i];
  }
}

void MergeSort(std::vector<int>& arr, int lt, int rt, size_t& counter) {
  if (lt < rt) {
    int mid = (lt + rt) / 2;
    MergeSort(arr, lt, mid, counter);
    MergeSort(arr, mid + 1, rt, counter);
    Merge(arr, lt, mid, rt, counter);
  }
}

int main() {
  int size = 0;
  size_t counter = 0;
  std::cin >> size;
  std::vector<int> arr;
  arr.reserve(size);
  int num = 0;
  for (int i = 0; i < size; i++) {
    std::cin >> num;
    arr.push_back(num);
  }
  MergeSort(arr, 0, arr.size() - 1, counter);
  std::cout << counter;
}