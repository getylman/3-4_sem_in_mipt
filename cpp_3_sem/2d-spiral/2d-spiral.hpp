int** MakeSpiral(int n) {
  int** arr = new int*[n];
  for (int i = 0; i < n; ++i) {
    arr[i] = new int[n];
  }
  int num = 0;
  int cols = 0;
  int lines = 0;
  for (int i = 0; i < (n + 1) / 2; ++i) {
    cols = lines = i;
    if (((n & 1) != 0) && (i == (n + 1) / 2 - 1)) {
      arr[cols][cols] = ++num;
      break;
    }
    for (; cols < n - i - 1; ++cols) {
      arr[lines][cols] = ++num;
    }
    for (; lines < n - i - 1; ++lines) {
      arr[lines][cols] = ++num;
    }
    for (; cols > i; --cols) {
      arr[lines][cols] = ++num;
    }
    for (; lines > i; --lines) {
      arr[lines][cols] = ++num;
    }
  }
  return arr;
}
