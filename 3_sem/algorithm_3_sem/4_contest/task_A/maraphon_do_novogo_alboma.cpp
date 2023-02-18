#include <iomanip>
#include <iostream>
#include <vector>

const size_t kDvaV16 = 65536;
const size_t kK10Pow5 = 100000;

class DO {
 public:
  size_t* arr;
  size_t* telo;

  void Run(size_t ur, size_t p_g);
  void ToMakeCheer(const size_t& ix, const size_t& usr, const size_t& p_gs,
                   double& answer);
  void MakeUpDate(const size_t& id);

  DO();
  ~DO();

 private:
  static bool EstLiPraviyBrat(const size_t& id);
};

DO::DO() {
  arr = new size_t[kDvaV16 * 2];
  for (int i = kDvaV16 + (kDvaV16 - 1); i > 0; --i) {
    arr[i] = 0;
  }
  telo = new size_t[kK10Pow5 + 1];
  for (size_t i = 1; i <= kK10Pow5; ++i) {
    telo[i] = 0;
  }
}

DO::~DO() {
  delete[] arr;
  delete[] telo;
}

bool DO::EstLiPraviyBrat(const size_t& id) {
  if (id / 2 != 0) {
    size_t pahan = id / 2;
    return (pahan * 2 < kDvaV16 * 2 - 1 && pahan * 2 + 1 != id);
  }
  return false;
}

void DO::MakeUpDate(const size_t& id) {
  if (id >= 1) {
    if (id * 2 + 1 <= kDvaV16 * 2 - 1) {
      arr[id] = arr[id * 2] + arr[id * 2 + 1];
    }
    MakeUpDate(id / 2);
  }
}

void DO::Run(size_t ur, size_t p_g) {
  if (telo[ur] != 0) {
    --arr[kDvaV16 - 1 + telo[ur]];
    MakeUpDate(kDvaV16 - 1 + telo[ur]);
  }
  telo[ur] = p_g;
  ++arr[kDvaV16 - 1 + telo[ur]];
  MakeUpDate(kDvaV16 - 1 + telo[ur]);
}

void DO::ToMakeCheer(const size_t& ix, const size_t& usr, const size_t& p_gs,
                     double& answer) {
  if (arr[1] == 1 && telo[usr] != 0) {
    answer = 1;
    return;
  }
  if (telo[usr] == 0) {
    answer = 0;
    return;
  }
  if (ix == 1) {
    answer += double(arr[1]);
    answer = answer / double((arr[1] - 1));
    answer = (answer > 1) ? 0 : answer;
    return;
  }
  if (!EstLiPraviyBrat(ix)) {
    ToMakeCheer(ix / 2, usr, p_gs, answer);
  } else {
    answer -= arr[ix + 1];
    ToMakeCheer(ix / 2, usr, p_gs, answer);
  }
}

int main() {
  DO moyo_derevo;
  size_t num_of_calls = 0;
  size_t usr = 0;
  size_t p_g = 0;
  double answer = 0;
  size_t index = 0;
  std::string com;
  std::cin >> num_of_calls;
  for (size_t i = 0; i < num_of_calls; ++i) {
    std::cin >> com;
    if (com == "RUN") {
      std::cin >> usr >> p_g;
      moyo_derevo.Run(usr, p_g);
    } else {
      std::cin >> usr;
      index = kDvaV16 - 1 + moyo_derevo.telo[usr] - 1;
      moyo_derevo.ToMakeCheer(index, usr, moyo_derevo.telo[usr], answer = 0);
      std::cout << std::setprecision(6) << answer << '\n';
    }
  }
}