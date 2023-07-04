#include <iostream>
#include <vector>
// 2g 
class Solution {
 public:
  static void Answer(const size_t& vertexes, std::vector<bool>& connections) {
    int idx1 = -1;
    int idx2 = -1;
    int idx3 = -1;
    while (0ul + ++idx1 < vertexes) {
      idx2 = -1;
      while (0ul + ++idx2 < vertexes) {
        idx3 = -1;
        while (0ul + ++idx3 < vertexes) {
          connections[idx2 * vertexes + idx3] =
              connections[idx2 * vertexes + idx3] ||
              (connections[idx2 * vertexes + idx1] &&
               connections[idx1 * vertexes + idx3]);
        }
      }
    }
  }
};

int main() {
  size_t vertexes = 0;
  std::cin >> vertexes;
  std::vector<bool> connections(vertexes * vertexes);
  char input_charecter;
  for (size_t i = 0; i < vertexes * vertexes; ++i) {
    std::cin >> input_charecter;
    connections[i] = (input_charecter == '1');
  }
  Solution::Answer(vertexes, connections);
  for (size_t i = 0; i < vertexes * vertexes; ++i) {
    std::cout << connections[i]
              << ((i % vertexes == vertexes - 1) ? '\n' : ' ');
  }
}