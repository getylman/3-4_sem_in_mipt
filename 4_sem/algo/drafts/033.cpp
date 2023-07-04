#include <cstddef>
#include <iostream>
#include <vector>
#include <queue>

template <size_t Len = 26, char LowLatter = 'a'>
struct AhoKarasikBor {
  AhoKarasikBor(size_t num) : body_(1), strs(num), len_(0) {}
  void SetString(const std::string& str, const size_t& idx) {
    Vertex* cur_node = &body_[0];
    size_t counter = 0;
    for (char letter : str) {
      if (cur_node->kids[static_cast<size_t>(letter - LowLatter)] == nullptr) {
        body_.push_back(Vertex());
        cur_node->kids[static_cast<size_t>(letter - LowLatter)] = &body_.back();
      }
      cur_node = cur_node->kids[static_cast<size_t>(letter - LowLatter)];
      ++counter;
    }
    cur_node->is_terminal = true;
    cur_node->id_str = idx;
    cur_node->depth = counter;
  }
  void GetResult(const std::string& str) {
    Algo(str);
    for (const auto& id1 : strs) {
      std::cout << id1.size()  << ' ';
      for (auto id2 : id1) {
        std::cout << id2 << ' ';
      }
      std::cout << '\n';
    }
  }

 private:
  struct Vertex {
    std::vector<Vertex*> kids;
    size_t depth = 0;
    size_t id_str = 0;
    bool is_terminal;
    Vertex* leave_link;
    Vertex* suf_link;
    // std::vector<Vertex*> transitions;
    Vertex()
        : kids(Len, nullptr),
          is_terminal(false),
          leave_link(nullptr),
          suf_link(nullptr) {}
          // transitions(Len, nullptr) {}
  };
  std::vector<Vertex> body_;
  std::vector<std::vector<uint32_t>> strs;
  size_t len_;
  void BuildS() {
    std::queue<std::pair<std::pair<Vertex*, Vertex*>, char>> qee;  // src -> dst
    for (char i = 0; i < Len; ++i) {
      if (body_[0].kids[i] != nullptr) {
        qee.emplace(std::make_pair(&body_[0], body_[0].kids[i]), i);
      }
    }
    Vertex* suf;
    Vertex* cur_node;
    Vertex* prev_node;
    char cur_c;
    while (!qee.empty()) {
      cur_c = qee.front().second;
      prev_node = qee.front().first.first;
      cur_node = qee.front().first.second;
      qee.pop();
      suf = prev_node->suf_link;
      while (suf != nullptr and suf->kids[cur_c] == nullptr) {
        suf = suf->suf_link;
      }
      cur_node->suf_link = (suf != nullptr) ? suf->kids[cur_c] : &body_[0];
      for (char i = 0; i < Len; ++i) {
        if (cur_node->kids[i] != nullptr) {
          qee.emplace(std::make_pair(cur_node, cur_node->kids[i]), i);
        }
      }
    }
  }
  void BuildE() {
    std::queue<Vertex*> qee;
    for (char i = 0; i < Len; ++i) {
      if (body_[0].kids[i] != nullptr) {
        qee.emplace(body_[0].kids[i]);
      }
    }
    Vertex* suf;
    Vertex* cur_node;
    while (!qee.empty()) {
      cur_node = qee.front();
      qee.pop();
      suf = cur_node->suf_link;
      if (suf != &body_[0]) {
        cur_node->leave_link = (!suf->is_terminal) ? suf->leave_link : suf; 
      }
      for (char i = 0; i < Len; ++i) {
        if (cur_node->kids[i] != nullptr) {
          qee.emplace(cur_node->kids[i]);
        }
      }
    }
  }
  Vertex* Transitions(Vertex* cur_node, char letter) {
    for (; cur_node != nullptr and cur_node->kids[letter] == nullptr; cur_node = cur_node->suf_link);
    return (cur_node != nullptr) ? cur_node->kids[letter] : &body_[0];
  }
  void Algo(const std::string& str) {
    BuildS();
    BuildE();
    Vertex* vertex1 = &body_[0];
    Vertex* vertex2;
    for (size_t i = 0; i < str.size(); ++i) {
      vertex2 = Transitions(vertex1, str[i]);
      vertex1 = vertex2;
      if (!vertex2->is_terminal) {
        vertex2 = vertex2->leave_link;
      }
      for (;vertex2 != nullptr; vertex2 = vertex2->leave_link) {
        strs[vertex2->id_str].push_back(i - vertex2->depth + 2);
      }
    }
  }
};

int main() {
  std::string text;
  std::cin >> text;
  size_t num = 0;
  std::cin >> num;
  AhoKarasikBor<> sol(num);
  std::string str;
  for (size_t i = 0; i < num; ++i) {
    std::cin >> str;
    sol.SetString(str, i);
  }
  sol.GetResult(text);
}