#include <iostream>
#include <vector>

template<class T>
class HashT {
 public:
  HashT(size_t HashF(const T&)) {
    body.reserve(kMod);
  }
  void Insert(const T& ft, const T& sd, size_t HashF(const T&)) {
    body[HashF(ft)] = sd;
    body[HashF(sd)] = ft;
  }
 private:
  std::vector<std::vector<T>> body;
  const size_t kMod = 100000;
};