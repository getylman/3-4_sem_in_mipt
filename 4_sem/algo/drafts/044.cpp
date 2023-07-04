#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

void KMP(std::string& substr, std::string& tstr) {
  int kkkk = -1;
  size_t size_s = substr.size();
  size_t sizet = tstr.size();
  size_t size_b_s = 2 * size_s + 1;
  size_t idx = 0;
	// substr += ' ' + tstr;
	std::vector<int32_t> dpp(substr.size() * 2);
	dpp[0] = -1;
  while (idx++ <= size_s + sizet + 1) {
    kkkk = dpp[idx - 1];
		for (; kkkk >= 0 and (substr[kkkk] != substr[idx - 1]); ) {
			kkkk = dpp[kkkk];
		}
		dpp[idx] = kkkk + 1;
	}
  idx = 0;
  for (auto i : dpp) {
    std::cout << i << ' ';
  }
	while (idx++ <= size_s + sizet + 1) {
		while (dpp[idx - 1] == size_s) {
			std::cout << idx - size_b_s - 1 << '\n';
      break;
    }
	}
}

int main(){
	std::string tstr;
  std::string substr;
	std::cin >> substr;
	KMP(substr, tstr);
	return 0;
}