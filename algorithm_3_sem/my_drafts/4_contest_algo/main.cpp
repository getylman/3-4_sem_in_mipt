#include <iomanip>
#include <iostream>
#include <vector>

const size_t kKhalf = 65536;
const size_t kKmaxUser = 100000;

class SegmentTree {
private:
  static bool HasRightBrother(size_t index);

public:
  size_t* arr;
  size_t* data;

  void Run(size_t user, size_t page);
  void Cheer(size_t index, size_t user, size_t pages, float& res);
  void Update(size_t index);

  SegmentTree();
  ~SegmentTree();
};

SegmentTree::SegmentTree() {
  arr = new size_t[kKhalf * 2];
  data = new size_t[kKmaxUser + 1];
  for (int i = kKhalf + (kKhalf - 1); i > 0; --i) {
    arr[i] = 0;
  }
  for (size_t i = 1; i <= kKmaxUser; ++i) {
    data[i] = 0;
  }
}

void SegmentTree::Update(size_t index) {
  if (index >= 1) {
    if (index * 2 + 1 <= kKhalf * 2 - 1) {
      arr[index] = arr[index * 2] + arr[index * 2 + 1];
    }
    Update(index / 2);
  }
}

SegmentTree::~SegmentTree() {
  delete[] arr;
  delete[] data;
}

void SegmentTree::Run(size_t user, size_t page) {
  if (data[user] != 0) {
    arr[kKhalf - 1 + data[user]]--;
    Update(kKhalf - 1 + data[user]);
  }
  data[user] = page;
  arr[kKhalf - 1 + data[user]]++;
  Update(kKhalf - 1 + data[user]);
}

void SegmentTree::Cheer(size_t index, size_t user, size_t pages, float& res) {
  if (arr[1] == 1 && data[user] != 0) {
    res = 1;
    return;
  }
  if (data[user] == 0) {
    res = 0;
    return;
  }

  if (index == 1) {
    res += float(arr[1]);
    res = res / float((arr[1] - 1));
    res = (res > 1) ? 0 : res;
    return;
  }
  if (!HasRightBrother(index)) {
    index /= 2;
    Cheer(index, user, pages, res);
  } else {
    res -= arr[index + 1];
    index /= 2;
    Cheer(index, user, pages, res);
  }
}

bool SegmentTree::HasRightBrother(size_t index) {
  if (index / 2 == 0) {
    return false;
  }
  size_t parent = index / 2;
  return (parent * 2 < kKhalf * 2 - 1 && parent * 2 + 1 != index);
}

int main() {
  SegmentTree tree;
  size_t n;
  size_t user;
  std::cin >> n;
  for (size_t i = 0; i < n; ++i) {
    std::string str;
    std::cin >> str;
    if (str == "CHEER") {
      std::cin >> user;
      float res = 0;
      size_t index = kKhalf - 1 + tree.data[user] - 1;
      tree.Cheer(index, user, tree.data[user], res);
      std::cout << std::setprecision(6) << res << '\n';
    } else {
      std::cin >> user;
      size_t page;
      std::cin >> page;
      tree.Run(user, page);
    }
  }
}