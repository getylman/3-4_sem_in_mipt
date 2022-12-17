#include <iostream>
#include <vector>

void Merge(std::vector<std::pair<int, int>>& arr, int lt, int mid, int rt,
           std::vector<int>& ans_arr) {
  int i = lt, j = mid + 1, k = 0;
  std::vector<std::pair<int, int>> tmp_arr;
  tmp_arr.reserve(arr.size());
  while (i <= mid && j <= rt) {
    if (arr[i].first <= arr[j].first) {
      tmp_arr[k++] = arr[j++];
    } else {
      ans_arr[arr[i].second] += rt - j + 1;
      tmp_arr[k++] = arr[i++];
    }
  }
  while (i <= mid) {
    tmp_arr[k++] = arr[i++];
  }
  while (j <= rt) {
    tmp_arr[k++] = arr[j++];
  }
  for (i = lt; i <= rt; i++) {
    arr[i] = tmp_arr[i - lt];
  }
}

void MergeSort(std::vector<std::pair<int, int>>& arr, int lt, int rt,
               std::vector<int>& ans_arr) {
  if (lt < rt) {
    int mid = (lt + rt) / 2;
    MergeSort(arr, lt, mid, ans_arr);
    MergeSort(arr, mid + 1, rt, ans_arr);
    Merge(arr, lt, mid, rt, ans_arr);
  }
}

int main() {
  int size = 0;
  std::cin >> size;
  std::vector<std::pair<int, int>> arr;
  std::vector<int> ans_arr;
  arr.reserve(size);
  ans_arr.reserve(size);
  int num = 0;
  for (int i = 0; i < size; i++) {
    std::cin >> num;
    arr.push_back({num, i});
    ans_arr.push_back(0);
  }
  MergeSort(arr, 0, arr.size() - 1, ans_arr);
  for (size_t i = 0; i < ans_arr.size(); i++) {
    std::cout << ans_arr[i] << ' ';
  }
}