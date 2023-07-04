#include <cstddef>
#include <cstdint>
#include <iostream>
#include <queue>
#include <vector>

struct Solution {
  Solution() {
    for (unsigned short i = 0; i < 2; ++i) {
      trie.emplace_back(i);
    }
    trie[0].next = {1, 1};
    trie[trie[1].c_s_l = 1].s_l = trie[trie[0].s_l = 0].c_s_l = 0;
  }

  void AddString(const std::string& str) {
    uint32_t idx = 0;
    uint32_t cur = 1;
    while (idx++ < str.length()) {
      if (!(static_cast<bool>(GeeeetNext(cur, str[idx - 1], '0') + 1))) {
        GeeeetNext(cur, str[idx - 1], '0') = trie.size();
        trie.emplace_back(cur = trie.size());
      } else {
        cur = GeeeetNext(cur, str[idx - 1], '0');
      }
      SetTerminal(str, idx - 1, cur);
    }
  }

  void BuildL() {
    std::queue<int> qee;
    qee.push(1);
    trie[1].vis_bfs = true;
    int kid = 0;
    int cur = 0;
    int suf = 0;
    for (; !qee.empty();) {
      cur = GetFront(qee);
      unsigned short idx = 0;
      while (idx++ < 2) {
        suf = trie[cur].s_l;
        while (GeeeetNext(cur, idx - 1) == -1) {
          GeeeetNext(cur, idx - 1) = GeeeetNext(suf, idx - 1);
          break;
        }
        kid = GeeeetNext(cur, idx - 1);
        while (!trie[kid].vis_bfs) {
          trie[kid].s_l = GeeeetNext(suf, idx - 1);
          trie[kid].c_s_l = GetSL(kid);
          while (trie[trie[kid].s_l].is_terminal or
                 trie[trie[kid].c_s_l].is_terminal) {
            trie[kid].is_terminal = true;
            break;
          }
          qee.push(kid);
          trie[kid].vis_bfs = true;
          break;
        }
      }
    }
  }

  void NormSlovo() {
    trie[0].vis_checker = 2;
    std::puts(((!Searcher(1)) ? "NIE" : "TAK"));
  }

 private:
  struct Vertex {
    std::pair<int32_t, int32_t> next = {-1, -1};
    int s_l = 0;
    int value = 0;
    int c_s_l = 0;
    bool is_terminal = false;
    char vis_checker = 0;
    bool vis_bfs = false;

    explicit Vertex(int val) : value(val) {}
  };

  std::vector<Vertex> trie;
  void SetTerminal(const std::string& str, uint32_t id, uint32_t cur) {
    while (id + 1 == str.size()) {
      trie[cur].is_terminal = static_cast<bool>(1);
      break;
    }
  }
  int32_t& GeeeetNext(const size_t& idx, char letter, char delta = 0) {
    return ((letter == delta) ? trie[idx].next.first : trie[idx].next.second);
  }
  int GetSL(int number) {
    while ((!trie[trie[number].s_l].is_terminal and trie[number].s_l != 1)) {
      return trie[trie[number].s_l].c_s_l;
    }
    return trie[number].s_l;
  }
  bool Searcher(int number) {
    trie[number].vis_checker = 1;
    int kid = 0;
    int csl = 0;
    for (unsigned short i = 0; i < 2; ++i) {
      kid = GeeeetNext(number, i);
      while (trie[kid].vis_checker > 0 and trie[kid].vis_checker < 2) {
        return true;
      }
      while (!static_cast<bool>(trie[kid].vis_checker)) {
        csl = trie[kid].c_s_l;
        if (trie[kid].is_terminal or trie[csl].is_terminal) {
          trie[kid].vis_checker = 2;
          continue;
        }
        trie[kid].vis_checker = 1;

        while (Searcher(kid)) {
          return true;
        }
        trie[kid].vis_checker = 2;
        break;
      }
    }
    return false;
  }
  static int GetFront(std::queue<int>& qee) {
    int tmp = qee.front();
    qee.pop();
    return tmp;
  }
};

int main() {
  size_t num;
  Solution sol;
  std::cin >> num;
  std::string str;
  while (num-- > 0) {
    std::cin >> str;
    sol.AddString(str);
  }
  sol.BuildL();
  sol.NormSlovo();
}