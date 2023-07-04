#include "complex"
#include "iostream"
#include "vector"
using namespace std;

void fft(vector<complex<double>> &a, bool invert) {
  if (a.size() == 1) return;

  vector<complex<double>> a0, a1;
  for (size_t i = 0; i < a.size(); ++i) {
    if (i % 2) {
      a1.push_back(a[i]);
    } else {
      a0.push_back(a[i]);
    }
  }
  fft(a0, invert);
  fft(a1, invert);

  double ang = 2 * M_PI / a.size() * (invert ? -1 : 1);
  complex<double> w(1), wn(cos(ang), sin(ang));
  for (int i = 0; i < a.size() / 2; ++i) {
    a[i] = a0[i] + w * a1[i];
    a[i + a.size() / 2] = a0[i] - w * a1[i];
    if (invert) a[i] /= 2, a[i + a.size() / 2] /= 2;
    w *= wn;
  }
}

void multiply(const vector<int> &a, const vector<int> &b, vector<int> &res) {
  vector<complex<double>> fa(a.begin(), a.end()), fb(b.begin(), b.end());
  size_t n = 1;
  while (n < max(a.size(), b.size())) {
    n <<= 1;
  }
  n <<= 1;
  fa.resize(n, 0), fb.resize(n, 0);
  fft(fa, false);
  fft(fb, false);
  for (size_t i = 0; i < n; ++i) fa[i] *= fb[i];
  fft(fa, true);

  res.resize(n);
  for (size_t i = 0; i < n; ++i)
    if (fa[i].real() > 0) {
      res[i] = int(fa[i].real() + 0.5);
    } else {
      res[i] = int(fa[i].real() - 0.5);
    }
}

int main() {
  std::vector<int> first, second;
  size_t first_sz, second_sz;
  std::cin >> first_sz;
  for (size_t i = 0; i < first_sz + 1; ++i) {
    int tmp;
    std::cin >> tmp;
    first.push_back(tmp);
  }
  std::cin >> second_sz;
  for (size_t i = 0; i < 1 + second_sz; ++i) {
    int tmp;
    std::cin >> tmp;
    second.push_back(tmp);
  }
  std::vector<int> res;
  multiply(first, second, res);
  std::cout << first_sz + second_sz << ' ';
  for (int i = 0; i < first_sz + second_sz + 1; ++i) {
    std::cout << res[i] << ' ';
  }
}