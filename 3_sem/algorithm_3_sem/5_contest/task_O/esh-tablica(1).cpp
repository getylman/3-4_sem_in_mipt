#include <iostream>
#include <vector>

class HT {
 public:
  HT() {
    body_.resize(kBase, 0);
    ToBuildBitMask();
  }
  void Insert(const uint32_t& elem) {
    body_[elem / kK2in5] |= bit_mask_[elem & (kK2in5 - 1)];
  }
  bool IsExist(const uint32_t& elem) const {
    return (body_[elem / kK2in5] & bit_mask_[elem & (kK2in5 - 1)]) ==
           bit_mask_[elem & (kK2in5 - 1)];
  }
  void Erase(const uint32_t& elem) {
    body_[elem / kK2in5] &= ~bit_mask_[elem & (kK2in5 - 1)];
  }

 private:
  const uint32_t kBase = 31'250'000;
  const uint32_t kK2in5 = 32;
  std::vector<uint32_t> body_;
  std::vector<uint32_t> bit_mask_;
  void ToBuildBitMask() {
    bit_mask_.resize(kK2in5, 0);
    for (uint32_t i = 0; i < kK2in5; ++i) {
      bit_mask_[i] = 1 << i;
    }
  }
};

int main() {
  HT ht;
  uint32_t num_of_calls = 0;
  std::cin >> num_of_calls;
  char com;
  uint32_t elem;
  for (uint32_t i = 0; i < num_of_calls; ++i) {
    std::cin >> com >> elem;
    if (com == '?') {
      std::cout << ((ht.IsExist(elem)) ? "YES\n" : "NO\n");
    } else if (com == '-') {
      ht.Erase(elem);
    } else {
      ht.Insert(elem);
    }
  }
  return 0;
}