#include <iostream>
#include <queue>

uint32_t Split(int* arr, uint32_t lt, uint32_t rt) {
  uint32_t mid = lt;
  int pivot = arr[rand() % (rt - lt) + lt];
  for (uint32_t i = lt; i < rt; ++i) {
    if (arr[i] <= pivot) {
      std::swap(arr[i], arr[mid]);
      ++mid;
    }
  }
  return mid;
}

bool CheckForUniq(int* arr, uint32_t lt, uint32_t rt) {//проверка вдруг в полуинтервале содержатся все схожие эл-ты
  int first_elem = arr[lt];
  for (uint32_t i = lt; i < rt; ++i) {
    if (arr[i] != first_elem) {
      return true;
    }
  }
  return false;
}

void QuickSort(int* arr, uint32_t begin, uint32_t end) {
  std::queue<std::pair<int, int>> vec;
  vec.push({begin, end});
  vec.push({-1, -1});
  uint32_t mid = 0;
  bool fall = true;
  while (true) {
    fall = true;
    while (vec.front().first != -1) {
      if (vec.front().second - vec.front().first > 1) {
        fall = false;
        mid = Split(arr, vec.front().first, vec.front().second);
        if (CheckForUniq(arr, vec.front().first, mid)) {
          vec.push({vec.front().first, mid});
        }
        if (CheckForUniq(arr, mid, end)) {
          vec.push({mid, vec.front().second});
        }
      }
      vec.pop();
    }
    if (fall) {
      break;
    }
    vec.pop();
    vec.push({-1, -1});
  }
}

int main() {
  uint32_t size = 0;
  std::cin >> size;
  int* arr = new int[size];
  for (uint32_t i = 0; i < size; ++i) {
    std::cin >> arr[i];
  }
  QuickSort(arr, 0, size);
  for (uint32_t i = 0; i < size; ++i) {
    std::cout << arr[i] << ' ';
  }
  delete [] arr;
}



/* working code
#include <iostream>

int Split(int* arr, int lt, int rt) {
  int mid = lt;
  int pivot = arr[rand() % (rt - lt) + lt];
  for (int i = lt; i < rt; ++i) {
    if (arr[i] <= pivot) {
      std::swap(arr[i], arr[mid]);
      ++mid;
    }
  }
  return mid;
}

void QuickSort(int* arr, int begin, int end) {
  if (end - begin > 1) {
    int mid = Split(arr, begin, end);
    bool un_unq1 = false, un_unq2 = false;
    int frst1 = arr[begin], frst2 = arr[mid];
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

int main() {
  int size = 0;
  std::cin >> size;
  int arr[100000] = {};
  for (int i = 0; i < size; ++i) {
    std::cin >> arr[i];
  }
  QuickSort(arr, 0, size);
  for (int i = 0; i < size; ++i) {
    std::cout << arr[i] << ' ';
  }
}
*/