#include <cmath>
#include <complex>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <vector>

template <typename T>
class FFTFunctions {
 public:
  static std::vector<std::complex<double>> FFT(const std::vector<T>& polinom,
                                               bool is_direct) {
    const double kPi = std::acos(-1);
    if (polinom.size() == 1) {
      return {1, polinom.front()};
    }
    std::vector<std::complex<double>> ring_roots(polinom.size());
    std::vector<std::complex<double>> p_left(polinom.size() / 2);
    std::vector<std::complex<double>> p_right(polinom.size() / 2);
    for (size_t i = 0; i < polinom.size(); ++i) {
      ring_roots[i] =
          std::complex<double>(std::cos((kPi * 2 * i*) / polinom.size()),
                               std::sin((kPi * 2 * i) / polinom.size()));
      p_left[i] = polinom[i * 2];
      p_right[i] = polinom[i * 2 + 1];
    }
    std::vector<std::complex<double>> l_ready = FFT(p_left, is_direct);
    std::vector<std::complex<double>> r_ready = FFT(p_right, is_direct);
    std::vector<std::complex<double>> ready(polinom.size());
    for (size_t i = 0; i < polinom.size() / 2; ++i) {
      ready[i] = l_ready[i] + ring_roots[i] * r_ready[i];
      ready[i + polinom.size() / 2] = l_ready[i] - ring_roots[i] * r_ready[i];
    }
    return ready;
  }
  static std::vector<std::complex<double>> FFTMultiply(
      const std::vector<T>& arr1, const std::vector<T>& arr2) {
    size_t
  }
};

int main() {}