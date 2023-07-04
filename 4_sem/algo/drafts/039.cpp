#include <complex>
#include <cstddef>
#include <iostream>
#include <vector>

template <typename T>
class FFT {
 public:
  static void MakeFFT(std::vector<std::complex<double>>& arr,
                      bool invert = false) {
    if (arr.size() == 1) {
      return;
    }
    std::vector<std::complex<double>> arr0(arr.size() / 2);
    std::vector<std::complex<double>> arr1(arr.size() / 2);
    for (size_t i = 0; i < arr.size() / 2; ++i) {
      arr0[i] = arr[i * 2];
      arr1[i] = arr[i * 2 + 1];
    }
    MakeFFT(arr0, invert);
    MakeFFT(arr1, invert);  // ждемс ответа о использовании рекурсии
    double teta = M_PI / arr.size() * (invert ? -2 : 2);
    std::complex<double> omega(1);
    std::complex<double> cur_omega(std::cos(teta), std::sin(teta));
    for (size_t i = 0; i < arr.size() / 2; ++i) {
      arr[i] =
          (arr0[i] + omega * arr1[i]) / std::complex<double>((invert) ? 2 : 1);
      arr[i + arr.size() / 2] =
          (arr0[i] - omega * arr1[i]) / std::complex<double>((invert) ? 2 : 1);
      omega *= cur_omega;
    }
  }
  static std::vector<T> Multiply(const std::vector<T>& arr1,
                                 const std::vector<T>& arr2) {
    std::vector<std::complex<double>> arr01(arr1.begin(), arr1.end());
    std::vector<std::complex<double>> arr02(arr2.begin(), arr2.end());
    size_t idx = 1;
    while (idx < std::max(arr1.size(), arr2.size())) {
      idx <<= 1;
    }
    idx <<= 1;
    std::vector<T> result(idx);
    arr01.resize(idx);
    arr02.resize(idx);

    MakeFFT(arr01);
    MakeFFT(arr02);
    for (size_t i = 0; i < idx; ++i) {
      arr01[i] *= arr02[i];
    }
    MakeFFT(arr01, true);
    for (size_t i = 0; i < idx; ++i) {
      result[i] = round(arr01[i].real());
    }
    return result;
  }
};

int main() {
  size_t alen = 0;
  size_t blen = 0;
  std::cin >> alen;
  std::vector<short> arr1(alen + 1);
  for (auto& idx : arr1) {
    std::cin >> idx;
  }
  std::cin >> blen;
  std::vector<short> arr2(blen + 1);
  for (auto& idx : arr2) {
    std::cin >> idx;
  }
  std::vector<short> res = FFT<short>::Multiply(arr1, arr2);
  std::cout << alen + blen << ' ';
  size_t idx = 0;
  while (idx <= alen + blen) {
    std::cout << res[idx++] << " ";
  }
  return 0;
}