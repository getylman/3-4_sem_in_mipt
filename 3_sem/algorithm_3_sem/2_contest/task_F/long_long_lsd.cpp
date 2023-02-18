#include <iostream>
#include <queue>
#include <vector>

void Merge(std::vector<size_t>& arr, std::vector<std::queue<size_t>>& voq) {
  size_t idx = 0;
  const int kMod4bt = 16;
  for (int i = 0; i < kMod4bt; ++i) {
    while (!voq[i].empty()) {
      arr[idx++] = voq[i].front();
      voq[i].pop();
    }
  }
}

void LSD(std::vector<size_t>& arr) {
  const int kMod4bt = 16;
  size_t div = 0;
  std::vector<std::queue<size_t>> voq(kMod4bt);
  for (int i = 0; i < kMod4bt; ++i) {
    for (size_t j = 0; j < arr.size(); ++j) {
      voq[(arr[j] >> div) & (kMod4bt - 1)].push(arr[j]);
    }
    div += 4;
    Merge(arr, voq);
  }
}

int main() {
  int size = 0;
  std::cin >> size;
  std::vector<size_t> arr;
  arr.reserve(size);
  size_t num = 0;
  for (int i = 0; i < size; ++i) {
    std::cin >> num;
    arr.push_back(num);
  }
  LSD(arr);
  for (size_t i = 0; i < arr.size(); ++i) {
    std::cout << arr[i] << '\n';
  }
}