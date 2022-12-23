#include "string.hpp"

String::String() : str_(nullptr) {}

String::String(const size_t& size, const char& character) {
  capacity_ = size_ = size;
  str_ = new char[++capacity_];
  for (size_t i = 0; i < size; ++i) {
    str_[i] = character;
  }
  str_[size_] = '\0';
}

String::String(const char* str) {
  if (str == nullptr) {
    capacity_ = 1;
    size_ = 0;
    str_ = new char[1];
    str_[0] = '\0';
  } else {
    size_t len = strlen(str);
    capacity_ = len + 1;
    size_ = len;
    str_ = new char[capacity_];
    for (size_t i = 0; i < size_; ++i) {
      str_[i] = str[i];
    }
    str_[size_] = '\0';
  }
}

String::String(const String& str) {
  if (str.Data() == nullptr) {
    capacity_ = 1;
    size_ = 0;
    str_ = new char[1];
    str_[0] = '\0';
  } else {
    str_ = new char[capacity_ = str.Capacity() + 1];
    for (size_t i = 0; i < str.Size(); ++i) {
      str_[i] = str[i];
    }
    str_[size_ = str.Size()] = '\0';
  }
}

String::~String() { delete[] str_; }

void String::Refill() {
  char* tmp_str = new char[capacity_];
  for (size_t i = 0; i < size_; ++i) {
    tmp_str[i] = str_[i];
  }
  delete[] str_;
  str_ = tmp_str;
}

void String::RefillPro(const size_t& new_cap) {
  if (Capacity() == 0) {
    ++capacity_;
  } else {
    while (Capacity() < new_cap) {
      capacity_ *= 2;
      --capacity_;
    }
  }
  Refill();
}

bool String::Find(const String& sub_string, const size_t& lft,
                  const size_t& rht) const {
  //ищем первое вхождение на этом интервале не включая rt есдт не найдёт то
  //вернёт rt
  if (rht > size_) {
    throw std::runtime_error("You want take non-existent element\n");
  }
  bool finded = false;
  if (rht - lft >= sub_string.Size()) {
    finded = true;
  }
  if (finded) {
    for (size_t j = lft; j < rht && j < sub_string.Size() + lft; ++j) {
      if (str_[j] != sub_string[j - lft]) {
        finded = false;
      }
    }
  }
  return finded;
}

String& String::operator=(const String& str) {
  if (str.Data() == nullptr) {
    Clear();
    return *this;
  }
  if (str.Data() == str_) {
    return *this;
  }
  delete[] str_;
  str_ = new char[capacity_ = str.Capacity() + 1];
  for (size_t i = 0; i < str.Size(); ++i) {
    str_[i] = str[i];
  }
  size_ = str.Size();
  return *this;
}

bool operator==(const String& str, const String& str_a) {
  return !(str < str_a || str > str_a);
}
bool operator!=(const String& str, const String& str_a) {
  return !(str == str_a);
}
bool operator<(const String& str, const String& str_a) {
  if (str.Data() == nullptr || str_a.Data() == nullptr) {
    throw std::runtime_error("You want take non-existent element\n");
  }
  size_t min_size = std::min(str.Size(), str_a.Size());
  for (size_t i = 0; i < min_size; ++i) {
    if (str[i] != *(str_a.Data() + i)) {
      return str[i] < *(str_a.Data() + i);
    }
  }
  return str.Size() < str_a.Size();
}
bool operator>=(const String& str, const String& str_a) {
  return !(str < str_a);
}
bool operator>(const String& str, const String& str_a) { return str_a < str; }
bool operator<=(const String& str, const String& str_a) {
  return !(str > str_a);
}

size_t String::Size() const { return size_; }
size_t String::Capacity() const { return capacity_ - 1; }
bool String::Empty() const { return size_ == 0; }
char* String::Data() { return str_; }
const char* String::Data() const { return str_; }
void String::Clear() {
  if (str_ == nullptr) {
    return;
  }
  str_[size_ = 0] = '\0';
}

void String::ShrinkToFit() {
  if (str_ == nullptr) {
    return;
  }
  capacity_ = size_;
  ++capacity_;
  Refill();
  str_[size_] = '\0';
}

void String::Resize(const size_t& new_size) {
  if (str_ == nullptr) {
    str_ = new char[new_size + 1];
    capacity_ = size_ = new_size;
    ++capacity_;
    str_[size_] = '\0';
    return;
  }
  if (new_size > size_) {
    if (Capacity() <= new_size) {
      RefillPro(new_size);
    }
  }
  str_[size_ = new_size] = '\0';
}

void String::Resize(const size_t& new_size, const char& character) {
  if (str_ == nullptr) {
    str_ = new char[new_size + 1];
    capacity_ = new_size;
    ++capacity_;
  }
  if (new_size > size_) {
    if (Capacity() <= new_size) {
      RefillPro(new_size);
    }
    for (size_t i = size_; i < new_size; ++i) {
      str_[i] = character;
    }
  }
  str_[size_ = new_size] = '\0';
}

void String::Reserve(const size_t& new_cap) {
  if (str_ == nullptr) {
    str_ = new char[new_cap];
    str_[size_ = 0] = '\0';
    capacity_ = new_cap + 1;
    return;
  }
  if (new_cap <= Capacity()) {
    return;
  }
  char* tmp_str = new char[new_cap + 1];
  for (size_t i = 0; i < size_; ++i) {
    tmp_str[i] = str_[i];
  }
  delete[] str_;
  capacity_ = new_cap + 1;
  str_ = tmp_str;
  str_[size_] = '\0';
}

void String::PushBack(const char& character) {
  if (str_ == nullptr) {
    str_ = new char[2];
    capacity_ = 2;
    size_ = 0;
  }
  if (size_ == Capacity()) {
    RefillPro(size_ + 1);
  }
  str_[size_++] = character;
  str_[size_] = '\0';
}

void String::PopBack() {
  if (!Empty()) {
    str_[--size_] = '\0';
  }
}

const char& String::operator[](const size_t& pos) const {
  if (pos < size_) {
    return *(str_ + pos);
  }
  throw std::runtime_error("You want take non-existent element\n");
}

char& String::operator[](const size_t& pos) {
  if (pos < size_) {
    return *(str_ + pos);
  }
  throw std::runtime_error("You want take non-existent element\n");
}

std::vector<String> String::Split(const String& delim) const {
  std::vector<String> res_of_split;
  if (str_ == nullptr) {
    res_of_split.push_back(String(""));
    return res_of_split;
  }
  String sub_string("");
  for (size_t i = 0; i < size_; ++i) {
    if (str_[i] == delim[0]) {
      if (Find(delim, i, size_)) {
        i += delim.Size() - 1;
        res_of_split.push_back(sub_string);
        sub_string.Clear();
        continue;
      }
    }
    sub_string.PushBack(str_[i]);
  }
  res_of_split.push_back(sub_string);
  return res_of_split;
}

String String::Join(const std::vector<String>& strings) const {
  String res_of_join("");
  for (size_t i = 0; i < strings.size(); ++i) {
    res_of_join += strings[i];
    if (i != strings.size() - 1) {
      res_of_join += *this;
    }
  }
  return res_of_join;
}

char& String::Front() {
  if (!Empty()) {
    return *str_;
  }
  throw std::runtime_error("You want take non-existent element\n");
}

char& String::Back() {
  if (!Empty()) {
    return *(str_ + size_ - 1);
  }
  throw std::runtime_error("You want take non-existent element\n");
}

const char& String::Front() const {
  if (!Empty()) {
    return *str_;
  }
  throw std::runtime_error("You want take non-existent element\n");
}

const char& String::Back() const {
  if (!Empty()) {
    return *(str_ + size_ - 1);
  }
  throw std::runtime_error("You want take non-existent element\n");
}

String& String::operator+=(const String& str) {
  for (size_t i = 0; i < str.Size(); ++i) {
    PushBack(str[i]);
  }
  return *this;
}

String operator+(const String& str, const String& str_a) {
  return String(str) += str_a;
}

String& String::operator*=(const size_t& num) {
  if (num == 0) {
    Clear();
    return *this;
  }
  if (str_ == nullptr) {
    str_ = new char[1];
    str_[size_ = 0] = '\0';
    return *this;
  }
  String tmp_str;
  for (size_t i = 0; i < num; ++i) {
    tmp_str += *this;
  }
  *this = tmp_str;
  return *this;
}

String operator*(const String& str, const size_t& num) {
  String tmp_str = str;
  return tmp_str *= num;
}

String operator*(const size_t& num, const String& str) { return str * num; }

void String::Swap(String& other) {
  std::swap(str_, other.str_);
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
}

std::ostream& operator<<(std::ostream& ost, const String& str) {
  if (str.Data() == nullptr) {
    return ost;
  }
  for (size_t i = 0; i < str.Size(); ++i) {
    ost << *(str.Data() + i);
  }
  return ost;
}

std::istream& operator>>(std::istream& ist, String& str) {
  const size_t kTmpSize = 10000;
  char* tmp_str = new char[kTmpSize];
  ist >> tmp_str;
  str = tmp_str;
  delete[] tmp_str;
  return ist;
}
