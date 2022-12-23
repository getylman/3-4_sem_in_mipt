#pragma once
#include <math.h>

#include <algorithm>
#include <iostream>

class Point;
//=====================VECTOR=======================
class Vector {
 public:
  // constructors
  Vector();
  Vector(const int64_t& x_c, const int64_t& y_c);
  Vector(const Vector& vec);
  Vector(const Point& p_begin, const Point& p_end);
  Vector& operator=(const Vector& vec);
  ~Vector() = default;
  //
  // operators
  int64_t operator*(const Vector& vec) const;  // scal mult
  int64_t operator^(const Vector& vec) const;  // vec mult
  Vector& operator+=(const Vector& vec);
  Vector operator+(const Vector& vec) const;
  Vector& operator-=(const Vector& vec);
  Vector operator-(const Vector& vec) const;
  Vector& operator*=(const int64_t& num);
  Vector operator*(const int64_t& num) const;
  friend Vector operator*(const int64_t& num, const Vector& vec);
  Vector operator-();
  //
  // gettors
  int64_t GetX() const;
  int64_t GetY() const;
  //
  // comparators
  bool operator==(const Vector& a_v) const;
  bool operator!=(const Vector& a_v) const;
  //
  // necessery methods
  size_t Len() const;
  //

 private:
  int64_t x_comp_;
  int64_t y_comp_;
};

Vector operator*(const int64_t& num, const Vector& vec);
//===================================================
class Segment;
//=======================IShape======================
class IShape {
 public:
  virtual void Move(const Vector& vec) = 0;
  virtual bool ContainsPoint(const Point& pnt) const = 0;
  virtual bool CrossSegment(const Segment& sgmnt) const = 0;
  virtual IShape* Clone() const = 0;
  virtual ~IShape() = default;
};
//===================================================
//=======================Point=======================
class Point : public IShape {
 public:
  // construcktors
  Point();
  Point(const int64_t& x_c, const int64_t& y_c);
  Point(const Point& pnt);
  Point& operator=(const Point& pnt);
  //
  // getters
  int64_t GetX() const;
  int64_t GetY() const;
  //
  // abstruckt methods
  void Move(const Vector& vec) override;
  bool ContainsPoint(const Point& pnt) const override;
  bool CrossSegment(const Segment& sgmnt) const override;
  IShape* Clone() const override;
  //
  // comparators
  bool operator==(const Point& pnt) const;
  bool operator!=(const Point& pnt) const;
  //
  // arifmethic operators
  Vector operator-(const Point& pnt) const;
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
  Segment();
  Segment(const Point& p_first, const Point& p_second);
  Segment(const Segment& sgmnt);
  Segment& operator=(const Segment& sgmnt);
  //
  // getters
  Point GetA() const;
  Point GetB() const;
  //
  // abstruckt methods
  void Move(const Vector& vec) override;
  bool ContainsPoint(const Point& pnt) const override;
  bool CrossSegment(const Segment& sgmnt) const override;
  IShape* Clone() const override;
  //
  // comparators
  bool operator==(const Segment& sgmnt) const;
  bool operator!=(const Segment& sgmnt) const;
  //

 private:
  Point begin_;
  Point end_;
};
// necessery functions
bool RangeIntersec(int64_t comp_beg_1, int64_t comp_end_1, int64_t comp_beg_2,
                   int64_t comp_end_2);
//
//===================================================

//======================Line=========================
class Line : public IShape {
 public:
  // construcktors
  Line();
  Line(const Point& p_first, const Point& p_second);
  Line(const Line& lne);
  Line& operator=(const Line& lne);
  //
  // getters
  int64_t GetA() const;
  int64_t GetB() const;
  int64_t GetC() const;
  //
  // abstruckt methods
  void Move(const Vector& vec) override;
  bool ContainsPoint(const Point& pnt) const override;
  bool CrossSegment(const Segment& sgmnt) const override;
  IShape* Clone() const override;
  //
  // comparators
  bool operator==(const Line& lne) const;
  bool operator!=(const Line& lne) const;
  //

 private:
  int64_t x_coef_;
  int64_t y_coef_;
  int64_t free_mem_;
  Point p_start_1_;         // first start point
  Point p_start_2_;         // second start point
  void ToNormalizeCoefs();  // converts coefficients to standard form
};
//===================================================

//=======================Ray=========================
class Ray : public IShape {
 public:
  // constructors
  Ray();
  Ray(const Point& p_f, const Point& p_s);
  Ray(const Ray& a_r);
  Ray& operator=(const Ray& a_r);
  //
  // getters
  Point GetA() const;
  Vector GetVector() const;
  //
  // abstract methods
  void Move(const Vector& vec) override;
  bool ContainsPoint(const Point& pnt) const override;
  bool CrossSegment(const Segment& sgmnt) const override;
  IShape* Clone() const override;
  //
  // comparators
  bool operator==(const Ray& a_r) const;
  bool operator!=(const Ray& a_r) const;
  //
  // necessery methods
  void ToNormolizeRay();
  //

 private:
  Point p_start_;  // point of start
  Vector g_v_;     // guide_vector
};
//===================================================
//========================Circle=====================
class Circle : public IShape {
 public:
  // constructors
  Circle();
  Circle(const Point& pnt, const size_t& rad);
  Circle(const Circle& crcl);
  Circle& operator=(const Circle& crcl);
  //
  // getters
  Point GetCentre() const;
  size_t GetRadius() const;
  //
  // abstract methods
  void Move(const Vector& vec) override;
  bool ContainsPoint(const Point& pnt) const override;
  bool CrossSegment(const Segment& sgmnt) const override;
  IShape* Clone() const override;
  //
  // comparators
  bool operator==(const Circle& crcl) const;
  bool operator!=(const Circle& crcl) const;
  //

 private:
  // (cnt -> center)
  Point cnt_;
  // (rad -> radius)
  size_t rad_;
};
//===================================================
