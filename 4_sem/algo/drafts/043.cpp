#include <iostream>
#include <queue>
#include <vector>

const int base = 2;

class AhoKarasik {
 private:
  int GetCode(char ch) { return ch - '0'; }

  struct Node {
    int value;
    int next[2];
    int suflink = 0;
    int comp_suflink = 0;
    bool is_terminal = false;

    explicit Node(int val) {
      value = val;
      next[0] = -1;
      next[1] = -1;
    }
  };

 public:
  std::vector<Node> trie;

  void AddString(const std::string &str) {
    int current = 1;
    for (int i = 0; i < str.length(); ++i) {
      if (trie[current].next[GetCode(str[i])] == -1) {
        trie[current].next[GetCode(str[i])] = trie.size();
        trie.emplace_back(trie.size());
        if (i + 1 == str.length()) {
          trie.back().is_terminal = true;
        }
        current = int(trie.size()) - 1;
      } else {
        current = trie[current].next[GetCode(str[i])];
        if (i + 1 == str.length()) {
          trie[current].is_terminal = true;
        }
      }
    }
  }

  void BuildTrie() {
    trie.emplace_back(0);
    trie.emplace_back(1);
    trie[0].next[0] = 1;
    trie[0].next[1] = 1;
    trie[1].suflink = 0;
    trie[1].comp_suflink = 1;
    trie[0].comp_suflink = 0;
    trie[0].suflink = 0;
  }

  int GetShortLink(int number) {
    if (trie[trie[number].suflink].is_terminal || trie[number].suflink == 1) {
      return trie[number].suflink;
    }
    return trie[trie[number].suflink].comp_suflink;
  }

  void BuildLinks() {
    std::queue<int> queue;
    std::vector<bool> visited(trie.size(), false);
    queue.push(1);
    visited[1] = true;
    while (!queue.empty()) {
      int current = queue.front();
      queue.pop();
      for (int jump = 0; jump < base; jump++) {
        int suf_link = trie[current].suflink;
        if (trie[current].next[jump] == -1) {
          trie[current].next[jump] = trie[suf_link].next[jump];
        }
        int children = trie[current].next[jump];
        if (!visited[children]) {
          trie[children].suflink = trie[suf_link].next[jump];
          trie[children].comp_suflink = GetShortLink(children);
          if (trie[trie[children].suflink].is_terminal ||
              trie[trie[children].comp_suflink].is_terminal) {
            trie[children].is_terminal = true;
          }
          queue.push(children);
          visited[children] = true;
        }
      }
    }
  }

  bool IsCicle(int number, std::vector<char> &visited) {
    visited[number] = 1;
    for (int children : trie[number].next) {
      if (visited[children] == 1) {
        return true;
      }
      if (visited[children] != 0) {
        continue;
      }

      int compressed_suf_link = trie[children].comp_suflink;
      if (trie[compressed_suf_link].is_terminal || trie[children].is_terminal) {
        visited[children] = 2;
        continue;
      }
      visited[children] = 1;

      if (IsCicle(children, visited)) {
        return true;
      }
      visited[children] = 2;
    }
    return false;
  }

  AhoKarasik() { BuildTrie(); }

  void IsGoodWord() {
    std::vector<char> visited(trie.size(), false);
    visited[0] = 2;
    if (IsCicle(1, visited)) {
      std::cout << "TAK";
    } else {
      std::cout << "NIE";
    }
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int size;
  AhoKarasik trie;
  std::cin >> size;
  for (int i = 0; i < size; i++) {
    std::string str;
    std::cin >> str;
    trie.AddString(str);
  }
  trie.BuildLinks();
  trie.IsGoodWord();
  return 0;
}