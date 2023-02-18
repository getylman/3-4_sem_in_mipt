#pragma once
#include <algorithm>
#include <iostream>

class Vector;
class Point;
class Segment;
//=======================IShape======================
class IShape {
 public:
  virtual void Move(const Vector&);
  virtual bool ContainsPoint(const Point&) const;
  virtual bool CrossSegment(const Segment&) const;
  virtual IShape* Clone();
};
//===================================================
//=======================Point=======================
class Point : public IShape {
 public:
  // construcktors
  Point() { x_coord_ = y_coord_ = 0; }
  Point(const int64_t&, const int64_t&);
  //
  // getters
  int64_t GetX() const { return x_coord_; }
  int64_t GetY() const { return y_coord_; }
  //
  // abstruckt methods
  void Move(const Vector&);
  bool ContainsPoint(const Point& pnt) const { return *this == pnt; }
  bool CrossSegment(const Segment&) const;
  //Point* Clone(); 
  //
  // comparators
  bool operator==(const Point& point) const {
    return (x_coord_ == point.x_coord_) && (y_coord_ == point.y_coord_);
  }
  //

 private:
  int64_t x_coord_;
  int64_t y_coord_;
};
//===================================================
//====================Segment========================
class Segment : public IShape {
 public:
  // cosntructors
  Segment(const Point&, const Point&);
  //
  // getters
  Point GetA() const { return begin_; }
  Point GetB() const { return end_; }
  //
  // abstruckt methods
  void Move(const Vector&);
  bool ContainsPoint(const Point& pnt) const;
  bool CrossSegment(const Segment&) const;
  //Segment* Clone();
  //
  // comparators
  bool operator==(const Segment& other) const { return (begin_ == other.GetA()) && (end_ == other.GetB()); }
  bool operator!=(const Segment& other) const { return !(*this == other); }
  //

 private:
  Point begin_;
  Point end_;
};
//===================================================
//======================Line=========================
class Line : public IShape {
 public:
  // construcktors
  Line(const Point&, const Point&);
  //
  // getters
  int64_t GetA() const { return x_coef_; }
  int64_t GetB() const { return y_coef_; }
  int64_t GetC() const { return free_mem_; }
  //
  // abstruckt methods
  //

 private:
  int64_t x_coef_;
  int64_t y_coef_;
  int64_t free_mem_;
  void ToNormalizeCoefs();  // converts coefficients to standard form
};
//===================================================
//=====================VECTOR=======================
class Vector {
 public:
  // constructors
  Vector();
  Vector(const int64_t&, const int64_t&);
  Vector(const Vector&);
  Vector(const Point&, const Point&);
  Vector& operator=(const Vector&);
  ~Vector() = default;
  //
  // operators
  int64_t operator*(const Vector&) const;  // scal mult
  int64_t operator^(const Vector&) const;  // vec mult
  Vector& operator+=(const Vector&);
  Vector operator+(const Vector&) const;
  Vector& operator-=(const Vector&);
  Vector operator-(const Vector&) const;
  Vector& operator*=(const int64_t&);
  Vector operator*(const int64_t&) const;
  friend Vector operator*(const int64_t&, const Vector&);
  Vector operator-();
  //
  // gettors
  int64_t GetX() const;
  int64_t GetY() const;
  //

 private:
  int64_t x_comp_;
  int64_t y_comp_;
};

Vector operator*(const int64_t&, const Vector&);
//===================================================




/// --------------CPP----------------
//===========VECTOR=============
//---------construstors---------
Vector::Vector() : x_comp_(0), y_comp_(0) {}
Vector::Vector(const int64_t& x, const int64_t& y) : x_comp_(x), y_comp_(y) {}
Vector::Vector(const Point& p_begin, const Point& p_end) {
  x_comp_ = p_end.GetX() - p_begin.GetX();
  y_comp_ = p_end.GetY() - p_begin.GetY();
}
Vector::Vector(const Vector& vec) : x_comp_(vec.x_comp_), y_comp_(vec.y_comp_) {}
Vector& Vector::operator=(const Vector& vec) {
  x_comp_ = vec.x_comp_;
  y_comp_ = vec.y_comp_;
  return *this;
}
//------------------------------
//----------operators-----------
int64_t Vector::operator*(const Vector& vec) const {
  return x_comp_ * vec.x_comp_ + y_comp_ * vec.y_comp_;
}
int64_t Vector::operator^(const Vector& vec) const {
  return x_comp_ * vec.y_comp_ - vec.x_comp_ * y_comp_;
}
Vector& Vector::operator+=(const Vector& vec) {
  x_comp_ += vec.x_comp_;
  y_comp_ += vec.y_comp_;
  return *this;
}
Vector Vector::operator+(const Vector& vec) const {
  return Vector(*this) += vec;
}
Vector& Vector::operator-=(const Vector& vec) {
  return *this += vec * -1;
}
Vector Vector::operator-(const Vector& vec) const {
  return Vector(*this) -= vec;
}
Vector& Vector::operator*=(const int64_t& num) {
  x_comp_ *= num;
  y_comp_ *= num;
  return *this;
}
Vector Vector::operator*(const int64_t& num) const {
  return Vector(*this) *= num;
}
Vector operator*(const int64_t& num, const Vector& vec) {
  return Vector(vec) *= num;
}
Vector Vector::operator-() { return Vector(*this) *= -1; }
//------------------------------
//-----------getters------------
int64_t Vector::GetX() const { return x_comp_; }
int64_t Vector::GetY() const { return y_comp_; }
//------------------------------
//==============================
//=============Point============
void Point::Move(const Vector& vec) {
  x_coord_ += vec.GetX();
  y_coord_ += vec.GetY();
}
bool Point::CrossSegment(const Segment& sgmnt) const {
  if (x_coord_ < sgmnt.GetA().GetX() || x_coord_ > sgmnt.GetB().GetX()) {
    return false;
  }
  if (y_coord_ < sgmnt.GetA().GetY() || y_coord_ > sgmnt.GetB().GetY()) {
    return false;
  }
  Vector a_to_point(sgmnt.GetA(), *this);
  Vector a_to_b(sgmnt.GetA(), sgmnt.GetB());
  return (a_to_b ^ a_to_point) == 0;
}
//==============================
//=============Segment==========
// construstors
Segment::Segment(const Point& p_first, const Point& p_second)
    : begin_(p_first), end_(p_second) {}
//
// necessary functions
bool RangeIntersec(int64_t comp_beg_1, int64_t comp_end_1, int64_t comp_beg_2,
                   int64_t comp_end_2) {
  if (comp_beg_1 > comp_end_1) {
    std::swap(comp_beg_1, comp_end_1);
  }
  if (comp_beg_2 > comp_end_2) {
    std::swap(comp_beg_2, comp_end_2);
  }
  return std::max(comp_beg_1, comp_beg_2) <= std::min(comp_end_1, comp_end_2);
}
//
void Segment::Move(const Vector& vec) {
  begin_.Move(vec);
  end_.Move(vec);
}
bool Segment::ContainsPoint(const Point& pnt) const {
  return pnt.CrossSegment(*this);
}
bool Segment::CrossSegment(const Segment& sgmnt) const {
  if (RangeIntersec(begin_.GetX(), end_.GetX(), sgmnt.begin_.GetX(),
                    sgmnt.end_.GetX()) &&
      RangeIntersec(begin_.GetY(), end_.GetY(), sgmnt.begin_.GetY(),
                    sgmnt.end_.GetY())) {
    Vector vec_of_this(begin_, end_);
    Vector vec_of_sgmnt(sgmnt.begin_, sgmnt.end_);
    return ((vec_of_this ^ Vector(begin_, sgmnt.begin_)) *
                (vec_of_this ^ Vector(begin_, sgmnt.end_)) <=
            0) &&
           ((vec_of_sgmnt ^ Vector(sgmnt.begin_, begin_)) *
                (vec_of_sgmnt ^ Vector(sgmnt.begin_, end_)) <=
            0);
  }
  return false;
}
//==============================
//=============Line=============
// construcktors
Line::Line(const Point& p_first, const Point& p_second) {
  int64_t dif_by_x = p_second.GetX() - p_first.GetX();
  int64_t dif_by_y = p_second.GetY() - p_first.GetY();
  y_coef_ = dif_by_x;
  x_coef_ = -dif_by_y;
  free_mem_ = -p_first.GetY() * dif_by_x + p_second.GetX() * dif_by_y;
  ToNormalizeCoefs();
}
//
// abstruckt methods

//
// necessery methods
void Line::ToNormalizeCoefs() {
  if (x_coef_ < 0) {
    x_coef_ *= -1;
    y_coef_ *= -1;
    free_mem_ *= -1;
  }
  int64_t com_divisor = std::__gcd(std::__gcd(x_coef_, (int64_t)abs(y_coef_)),
                                   (int64_t)abs(free_mem_));
  x_coef_ /= com_divisor;
  y_coef_ /= com_divisor;
  free_mem_ /= com_divisor;
}
//
//==============================
/// ---------------------------------