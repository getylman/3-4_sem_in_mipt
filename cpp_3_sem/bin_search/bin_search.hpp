#pragma once

template <class T>
T* LowerBound(T* first, T* last, const T& target) {
  unsigned int dist = last - first;
  unsigned int sub_dist = 0;
  T* it = first;
  while (dist > 0) {
    it = first + (sub_dist = dist / 2);
    if (*it < target) {
      first = ++it;
      dist -= ++sub_dist;
    } else {
      dist = sub_dist;
    }
  }
  return first;
}
