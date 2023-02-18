#include <iostream>
#include <string>

const int kN = 2e5 + 5;
std::pair<int, int> heap_min[kN];
std::pair<int, int> heap_max[kN];
std::pair<int, int> pos[kN];
int size_min;
int size_max;
int pos_size;

int SiftUpMin(int ind) {
  int i = ind;
  while (i > 0) {
    int par = (i - 1) / 2;
    if (heap_min[i].first < heap_min[par].first) {
      pos[heap_min[par].second].first = i;
      std::swap(heap_min[i], heap_min[par]);
      i = par;
    } else {
      break;
    }
  }
  return i;
}

int SiftUpMax(int ind) {
  int i = ind;
  while (i > 0) {
    int par = (i - 1) / 2;
    if (heap_max[i].first > heap_max[par].first) {
      pos[heap_max[par].second].second = i;
      std::swap(heap_max[i], heap_max[par]);
      i = par;
    } else {
      break;
    }
  }
  return i;
}

int SiftDownMin(int ind) {
  int i = ind;
  while (i < size_min) {
    int son1 = (2 * i + 1 < size_min ? 2 * i + 1 : i);
    int son2 = (2 * i + 2 < size_min ? 2 * i + 2 : i);
    if (heap_min[i].first >
        std::min(heap_min[son1].first, heap_min[son2].first)) {
      if (heap_min[son1].first <= heap_min[son2].first) {
        pos[heap_min[son1].second].first = i;
        std::swap(heap_min[i], heap_min[son1]);
        i = son1;
      } else {
        pos[heap_min[son2].second].first = i;
        std::swap(heap_min[i], heap_min[son2]);
        i = son2;
      }
    } else {
      break;
    }
  }
  return i;
}

int SiftDownMax(int ind) {
  int i = ind;
  while (i < size_max) {
    int son1 = (2 * i + 1 < size_max ? 2 * i + 1 : i);
    int son2 = (2 * i + 2 < size_max ? 2 * i + 2 : i);
    if (heap_max[i].first <
        std::max(heap_max[son1].first, heap_max[son2].first)) {
      if (heap_max[son1].first >= heap_max[son2].first) {
        pos[heap_max[son1].second].second = i;
        std::swap(heap_max[i], heap_max[son1]);
        i = son1;
      } else {
        pos[heap_max[son2].second].second = i;
        std::swap(heap_max[i], heap_max[son2]);
        i = son2;
      }
    } else {
      break;
    }
  }
  return i;
}

void InsertMin(int x, int ind) {
  heap_min[size_min].first = x;
  heap_min[size_min].second = ind;
  size_min++;
  pos[ind].first = SiftUpMin(size_min - 1);
  pos_size = ind;
}

void InsertMax(int x, int ind) {
  heap_max[size_max].first = x;
  heap_max[size_max].second = ind;
  size_max++;
  pos[ind].second = SiftUpMax(size_max - 1);
}

void Insert(int x, int ind) {
  InsertMin(x, ind);
  InsertMax(x, ind);
  std::cout << "ok\n";
}

void ExtractMin() {
  if (size_min == 0) {
    std::cout << "error\n";
    return;
  }
  std::cout << heap_min[0].first << "\n";
  int ind_max = pos[heap_min[0].second].second;
  size_min--;
  std::swap(pos[heap_min[0].second].first,
            pos[heap_min[size_min].second].first);
  std::swap(heap_min[0], heap_min[size_min]);
  int ind = heap_min[0].second;
  pos[ind].first = SiftDownMin(0);
  size_max--;
  std::swap(pos[heap_max[ind_max].second].second,
            pos[heap_max[size_max].second].second);
  std::swap(heap_max[ind_max], heap_max[size_max]);
  ind = heap_max[ind_max].second;
  if (heap_max[ind_max].first > heap_max[(ind_max - 1) / 2].first) {
    pos[ind].second = SiftUpMax(ind_max);
  } else {
    pos[ind].second = SiftDownMax(ind_max);
  }
}

void ExtractMax() {
  if (size_max == 0) {
    std::cout << "error\n";
    return;
  }
  std::cout << heap_max[0].first << "\n";
  int ind_min = pos[heap_max[0].second].first;
  size_max--;
  std::swap(pos[heap_max[0].second].second,
            pos[heap_max[size_max].second].second);
  std::swap(heap_max[0], heap_max[size_max]);
  int ind = heap_max[0].second;
  pos[ind].second = SiftDownMax(0);
  size_min--;
  std::swap(pos[heap_min[ind_min].second].first,
            pos[heap_min[size_min].second].first);
  std::swap(heap_min[ind_min], heap_min[size_min]);
  ind = heap_min[ind_min].second;
  if (heap_min[ind_min].first < heap_min[(ind_min - 1) / 2].first) {
    pos[ind].first = SiftUpMin(ind_min);
  } else {
    pos[ind].first = SiftDownMin(ind_min);
  }
}

void GetMax() {
  if (size_max == 0) {
    std::cout << "error\n";
  } else {
    std::cout << heap_max[0].first << "\n";
  }
}

void GetMin() {
  if (size_min == 0) {
    std::cout << "error\n";
  } else {
    std::cout << heap_min[0].first << "\n";
  }
}

void Clear() {
  size_min = size_max = 0;
  std::cout << "ok\n";
}

int main() {
  int n;
  std::cin >> n;
  for (int i = 0; i < n; i++) {
    std::string s;
    std::cin >> s;
    if (s == "insert") {
      int x;
      std::cin >> x;
      Insert(x, i);
    }
    if (s == "get_min") {
      GetMin();
    }
    if (s == "get_max") {
      GetMax();
    }
    if (s == "size") {
      std::cout << size_min << "\n";
    }
    if (s == "clear") {
      Clear();
    }
    if (s == "extract_min") {
      ExtractMin();
    }
    if (s == "extract_max") {
      ExtractMax();
    }
  }
}