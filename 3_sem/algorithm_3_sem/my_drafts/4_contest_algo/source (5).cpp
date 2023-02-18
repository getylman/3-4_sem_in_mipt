#include <iostream>
#include <vector>

struct Min2 {
  int min1 = 0;
  int min2 = 0;
  int x1 = 0;
  int x2 = 0;
};

Min2 GetMin(Min2 bord1, Min2 bord2) {
  int a1 = bord1.min1, a2 = bord1.min2, b1 = bord2.min1, b2 = bord2.min2;
  int x = bord1.x1, y = bord2.x1;
  Min2 res;
  if (a1 < b1) {
    res.min1 = a1;
    res.x1 = bord1.x1;
  } else {
    res.min1 = b1;
    res.x1 = bord2.x1;
  }
  if (x == y) {
    if (a2 < b2) {
      res.min2 = a2;
      res.x2 = bord1.x2;
    } else {
      res.min2 = b2;
      res.x2 = bord2.x2;
    }
  } else if (a1 < b1) {
    if (a2 < b1) {
      res.min2 = a2;
      res.x2 = bord1.x2;
    } else {
      res.min2 = b1;
      res.x2 = bord2.x1;
    }
  } else if (a1 >= b1) {
    if (a1 < b2) {
      res.min2 = a1;
      res.x2 = bord1.x1;
    } else if (a1 >= b2) {
      res.min2 = b2;
      res.x2 = bord2.x2;
    }
  }
  return res;
}
void Record(std::vector < std::vector < Min2>> & res, int n,
            std::vector<int>& input) {
  for (int j = 0; j < n; ++j) {
    res[1].resize(n);
    res[1][j] = {input[j], 1000000, j, 1};
  }
  for (int i = 2; i <= n; i *= 2) {
    res[i].resize(n);
    for (int j = 0; j <= n - i; j++) {
      res[i][j] = GetMin(res[i / 2][j], res[i / 2][j + i / 2]);
    }
  }
}
void PowTwo(std::vector<int>& pow, int n) {
  pow[0] = 0;
  for (int i = 1; i <= n; i++) {
    int j = 1;
    while (j <= i / 2) {
      j *= 2;
    }
    pow[i] = j;
    // std::cerr << pow[i] << "\n";
  }
}
void Solution(int bord1, int bord2, std::vector<int>& pow,
              std::vector < std::vector < Min2>> & res) {
  int length = bord2 - bord1 + 1;
  int powof2 = pow[length];
  int bord_l = bord1, bord_r = bord2;
  // std::cerr << res[powof2][bord_l].min1 << " " << res[powof2][bord_l].min2
  // << "\n";
  if (res[powof2][bord_l].x1 != res[powof2][bord_r - powof2 + 1].x1 &&
      res[powof2][bord_l].x2 != res[powof2][bord_r - powof2 + 1].x2) {
    Min2 ans = GetMin(res[powof2][bord_l], res[powof2][bord_r - powof2 + 1]);
    std::cout << ans.min2 << "\n";
    // std::cout << res[powof2][bord_r - powof2 + 1].min1 << " "<<
    // res[powof2][bord_r - powof2 + 1].min2 << "\n";
  } else if (res[powof2][bord_l].x1 != res[powof2][bord_r - powof2 + 1].x1) {
    std::cout << std::max(res[powof2][bord_l].min1,
                         res[powof2][bord_r - powof2 + 1].min1)
<< "\n";
  } else if (res[powof2][bord_l].x2 != res[powof2][bord_r - powof2 + 1].x2) {
    std::cout << std::min(res[powof2][bord_l].min2,
                         res[powof2][bord_r - powof2 + 1].min2)
<< "\n";
  } else {
    std::cout << res[powof2][bord_l].min2 << "\n";
  }
}

int main() {
  int n, m;
  std::cin >> n >> m;
  std::vector<int> input(n + 1);
  for (int i = 0; i < n; ++i) {
    std::cin >> input[i];
  }
  int pow_n = 0;
  int j = 1;
  while (j <= n / 2) {
    j *= 2;
  }
  pow_n = j;
  std::vector < std::vector < Min2>> res(pow_n * 2);
  Record(res, n, input);
  PowTwo(input, n);
  // std::vector<std::pair<int, int>> init_borders(M);
  int bord1, bord2;
  for (int i = 0; i < m; i++) {
    std::cin >> bord1 >> bord2;
    Solution(bord1 - 1, bord2 - 1, input, res);
    // std::cerr << bord1 - 1 <<" " << bord2 - 1<<"\n";
  }

  // Solution(init_borders, pow, res);
  // std::cout << res[4][3].min2<< "\n";
}