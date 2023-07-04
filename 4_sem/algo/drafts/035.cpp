#include <complex>
#include <cstddef>
#include <iostream>
#include <vector>

template <typename T>
class FFT {
 public:
  static void FFTF(std::vector<std::complex<double>>& arr,
                   bool invert = false) {
    if (arr.size() < 2) {
      return;
    }
    std::vector<std::complex<double>> arr0(arr.size() / 2);
    std::vector<std::complex<double>> arr1(arr.size() / 2);
    FFTloop(arr, arr0, arr1);
    CallFFT(arr0, arr1, invert);
    FooInFFT(arr, arr0, arr1, invert);
  }

  static std::vector<T> multiply(const std::vector<T>& arr1,
                                 const std::vector<T>& arr2) {
    std::vector<std::complex<double>> arr01;
    arr01.reserve(arr1.size());
    for (auto& idx : arr1) {
      arr01.emplace_back(idx);
    }
    std::vector<std::complex<double>> arr02;
    arr02.reserve(arr2.size());
    for (auto& idx : arr2) {
      arr02.emplace_back(idx);
    }
    size_t idx = 1;
    for (; idx < ((arr1.size() > arr2.size()) ? arr1.size() : arr2.size());
         idx <<= 1)
      ;
    idx <<= 1;
    std::vector<T> result(idx);
    IncLens(arr01, arr02, idx);
    CallFFT(arr01, arr02, false);
    Foo2(arr01, arr02, result, idx);
    return result;
  }

 private:
  static void IncLens(std::vector<std::complex<double>>& arr01,
                      std::vector<std::complex<double>>& arr02,
                      const size_t& idx) {
    arr01.resize(idx);
    arr02.resize(idx);
  }
  static void CallFFT(std::vector<std::complex<double>>& arr01,
                      std::vector<std::complex<double>>& arr02, bool ok) {
    FFTF(arr01, ok);
    FFTF(arr02, ok);
  }
  static void Foo1(std::vector<std::complex<double>>& arr01,
                   std::vector<std::complex<double>>& arr02,
                   const size_t& idx) {
    size_t id1 = 0;
    while (id1++ < idx) {
      arr01[id1 - 1] *= arr02[id1 - 1];
    }
    FFTF(arr01, true);
  }
  static void Foo2(std::vector<std::complex<double>>& arr01,
                   std::vector<std::complex<double>>& arr02,
                   std::vector<T>& result, const size_t& idx) {
    Foo1(arr01, arr02, idx);
    size_t id1 = 0;
    while (id1++ < idx) {
      result[id1 - 1] = round(arr01[id1 - 1].real());
    }
  }
  static void FooInFFT(std::vector<std::complex<double>>& arr,
                       std::vector<std::complex<double>>& arr0,
                       std::vector<std::complex<double>>& arr1, bool invert) {
    double teta = M_PI * (!invert ? 2 : -2) / arr.size();
    std::complex<double> omega(1.);
    std::complex<double> cur_omega(std::cos(teta), std::sin(teta));
    size_t idx = 0;
    while (idx++ < arr.size() / 2) {
      arr[idx - 1] = (arr0[idx - 1] + omega * arr1[idx - 1]) /
                     std::complex<double>((invert) ? 2 : 1);
      arr[idx - 1 + arr.size() / 2] = (arr0[idx - 1] - omega * arr1[idx - 1]) /
                                      std::complex<double>((invert) ? 2 : 1);
      omega *= cur_omega;
    }
  }
  static void FFTloop(std::vector<std::complex<double>>& arr,
                      std::vector<std::complex<double>>& arr0,
                      std::vector<std::complex<double>>& arr1) {
    size_t idx = 0;
    while (idx++ < arr.size() / 2) {
      arr0[idx - 1] = arr[(idx - 1) * 2];
      arr1[idx - 1] = arr[(idx - 1) * 2 + 1];
    }
  }
};

int main() {
  size_t alen = 0;
  size_t blen = 0;
  std::cin >> alen;
  std::vector<int> arr1(alen + 1);
  for (auto& idx : arr1) {
    std::cin >> idx;
  }
  std::cin >> blen;
  std::vector<int> arr2(blen + 1);
  for (auto& idx : arr2) {
    std::cin >> idx;
  }
  std::vector<int> res = FFT<int>::multiply(arr1, arr2);
  std::cout << alen + blen << ' ';
  size_t idx = 0;
  while (idx <= alen + blen) {
    std::cout << res[idx++] << " ";
  }
  return 0;
}
