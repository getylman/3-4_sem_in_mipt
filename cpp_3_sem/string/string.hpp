#pragma once
#include <string.h>

#include <istream>
#include <ostream>
#include <vector>

class String {
 public:
  // constructor
  String();                                           // done
  String(const size_t& size, const char& character);  // done
  String(const char* str);                            // done
  String(const String& str);                          // done
  String& operator=(const String& str);               // done
  ~String();                                          // done
  //
  // capacity
  void Clear();                                                // done
  size_t Size() const;                                         // done
  size_t Capacity() const;                                     // done
  bool Empty() const;                                          // done
  char* Data();                                                // done
  const char* Data() const;                                    // done
  void Resize(const size_t& new_size);                         // done
  void Resize(const size_t& new_size, const char& character);  // done
  void Reserve(const size_t& new_cap);                         // done
  void ShrinkToFit();                                          // done
  void Swap(String& other);                                    // done
  char& Front();                                               // done
  char& Back();                                                // done
  const char& Front() const;                                   // done
  const char& Back() const;                                    // done
  //
  // modifiers
  String& operator+=(const String& str);                       // done
  String& operator*=(const size_t& num);                       // done
  void PushBack(const char& character);                        // done
  void PopBack();                                              // done
  std::vector<String> Split(const String& delim = " ") const;  // done
  String Join(const std::vector<String>& strings) const;       // done
  //
  // element access
  const char& operator[](const size_t& pos) const;  // done
  char& operator[](const size_t& pos);              // done
  friend std::istream& operator>>(std::istream& ist, String& str);
  // friend String& operator=(String& str, const char* other);
  //
 private:
  char* str_;
  size_t size_ = 0, capacity_ = 1;
  void Refill();  //просто перезаписывает // done
  void RefillPro(
      const size_t& new_cap);  //увеличивает запас и перезаписывает // done
  bool Find(const String& sub_string, const size_t&,
            const size_t&) const;  // done
  //возвращает размер строки если не находит вхождение подстроки
};

std::istream& operator>>(std::istream& ist, String& str);        // done
std::ostream& operator<<(std::ostream& ost, const String& str);  // done
bool operator==(const String& str, const String& str_a);
bool operator!=(const String& str, const String& str_a);
bool operator<(const String& str, const String& str_a);
bool operator>=(const String& str, const String& str_a);
bool operator>(const String& str, const String& str_a);
bool operator<=(const String& str, const String& str_a);
// (str_a -> string another)
String operator+(const String& str, const String& str_a);
String operator*(const String& str, const size_t& num);
String operator*(const size_t& num, const String& str);
