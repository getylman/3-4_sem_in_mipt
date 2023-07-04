#include <complex>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

template <typename T>
class FFT {
 public:
  static void FFTF(std::vector<std::complex<double>>& arr,
                   bool invert = false) {
    Shaker(arr);
    FooAfterShkr(arr, invert);
  }

  static void Poow(const std::vector<T>& arr1,
                                 const std::vector<T>& arr2, std::vector<T>& result) {
    std::vector<std::complex<double>> arr01;
    Puuuuch(arr1, arr01);
    std::vector<std::complex<double>> arr02;
    Puuuuch(arr2, arr02);
    size_t idx = 1;
    for (; idx < ((arr1.size() > arr2.size()) ? arr1.size() : arr2.size());
         idx <<= 1)
      ;
    idx <<= 1;
    result.resize(idx);
    IncLens(arr01, arr02, idx);
    CallFFT(arr01, arr02, false);
    Foo2(arr01, arr02, result, idx);
  }

 private:
  static void Puuuuch(const std::vector<T>& arr, std::vector<std::complex<double>>& arr0) {
    arr0.reserve(arr.size());
    for (auto& idx : arr) {
      arr0.emplace_back(idx);
    }
  }
  static void Nothing() {}
  static void DeviderFoo(std::vector<std::complex<double>>& arr, size_t delta) {
    for (auto& idx : arr) {
      idx /= delta;
    }
  }
  static void FooAfterShkr(std::vector<std::complex<double>>& arr,
                           bool invert) {
    uint32_t id1 = 2;
    uint32_t id2 = 0;
    uint32_t id3 = 0;
    std::complex<double> omega1(1.);
    std::complex<double> omega(1.);
    std::complex<double> cur_omega(0.);
    std::complex<double> cur1(0.);
    std::complex<double> cur2(0.);
    while (id1 <= arr.size()) {
      double teta = M_PI * (!invert ? 2 : -2) / arr.size();
      cur_omega =
          (std::complex<double>(std::cos(teta), std::sin(teta)) += (id2 = 0));
      while (id2 <= arr.size()) {
        omega = (omega1 += (id3 = 0));
        while (id3 < (id1 >> 1)) {
          cur1 = arr[id2 + id3];
          cur2 = arr[id2 + id3 + (id1 >> 1)] * omega;
          arr[id2 + id3] += cur2;
          arr[id2 + id3 + (id1 >> 1)] = cur1 - cur2;
          omega *= cur_omega;
          ++id3;  // перенести в while
        }
        id2 += id1;
      }
      id1 <<= 1;
    }
    (!invert) ? (Nothing()) : (DeviderFoo(arr, arr.size()));
  }
  static void Shaker(std::vector<std::complex<double>>& arr) {
    size_t idx = 0;
    size_t num = 0;
    size_t id1 = 0;
    while (++idx < arr.size()) {
      num = (arr.size() >> 1) + (id1 = 0);
      while (id1 >= num) {
        id1 -= num;
        num >>= 1;
      }
      id1 += num;
      (idx > id1) ? (Nothing()) : (std::swap(arr[idx], arr[id1]));
    }
  }
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
      result[id1 - 1] = round(arr01[id1 - 1].real() +
                              ((!(arr01[id1 - 1].real() > 0)) ? -0.5 : 0.5));
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
  std::string num;
  std::cin >> num;
  std::vector<int32_t> bin_digits(num.size());
  std::vector<int32_t> result; 
  size_t idx = 0;
  while (((idx++ < num.size()) ? true : static_cast<bool>(idx = 0))) {
    bin_digits[idx - 1] = num[idx - 1] - '0';
  }
  while (((idx++ < 2) ? true : static_cast<bool>(idx = 0))) {
    FFT<int32_t>::Poow(bin_digits, bin_digits, result);
  }
  for (auto& id1 : result) {
    idx += ((static_cast<bool>(id1 % 2)) ? (id1 >> 1) : 0);
  }
  std::cout << idx;
  return 0;
}
