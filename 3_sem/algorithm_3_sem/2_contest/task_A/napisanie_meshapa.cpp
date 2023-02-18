#include <iostream>

int Split(std::pair<int, int>* arr, int lt, int rt) {
  int mid = lt;
  std::pair<int, int> pivot = arr[rand() % (rt - lt) + lt];
  for (int i = lt; i < rt; ++i) {
    if (arr[i] <= pivot) {
      std::swap(arr[i], arr[mid]);
      ++mid;
    }
  }
  return mid;
}

void QuickSort(std::pair<int, int>* arr, int begin, int end) {
  if (end - begin > 1) {
    int mid = Split(arr, begin, end);
    bool un_unq1 = false, un_unq2 = false;
    std::pair<int, int> frst1 = arr[begin], frst2 = arr[mid];
    for (int i = begin; i < mid; i++) {
      if (arr[i] != frst1) {
        un_unq1 = true;
        break;
      }
    }
    for (int i = mid; i < end; i++) {
      if (arr[i] != frst2) {
        un_unq2 = true;
        break;
      }
    }
    if (un_unq1) {
      QuickSort(arr, begin, mid);
    }
    if (un_unq2) {
      QuickSort(arr, mid, end);
    }
  }
}

void PrintAns(std::pair<int, int>* ans_arr, int size) {
  std::cout << size << std::endl;
  for (int i = 0; i < size; i++) {
    std::cout << ans_arr[i].first << ' ' << ans_arr[i].second;
    std::cout << std::endl;
  }
}

void TopMashUp(std::pair<int, int>* arr, int size) {
  std::pair<int, int> ans_arr[100000] = {};
  int id = 0;
  for (int i = 0; i < size; i++) {
    int lt = arr[i].first, rt = arr[i].second;
    if (i == size - 1) {
      ans_arr[id++] = {lt, rt};
      break;
    }
    for (int j = i + 1; j < size; j++) {
      if (arr[j].first <= rt) {
        if (rt <= arr[j].second) {
          rt = arr[j].second;
        }
        i++;
      } else {
        break;
      }
    }
    ans_arr[id++] = {lt, rt};
  }
  PrintAns(ans_arr, id);
}

int main() {
  int size = 0;
  std::cin >> size;
  std::pair<int, int> arr[100000] = {};
  for (int i = 0; i < size; i++) {
    std::cin >> arr[i].first >> arr[i].second;
  }
  QuickSort(arr, 0, size);
  TopMashUp(arr, size);
}
