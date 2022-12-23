#include "geometry.hpp"

//===========VECTOR=============
//---------construstors---------
Vector::Vector() : x_comp_(0), y_comp_(0) {}
Vector::Vector(const int64_t& x_c, const int64_t& y_c)
    : x_comp_(x_c), y_comp_(y_c) {}
Vector::Vector(const Point& p_begin, const Point& p_end) {
  x_comp_ = p_end.GetX() - p_begin.GetX();
  y_comp_ = p_end.GetY() - p_begin.GetY();
}
Vector::Vector(const Vector& vec)
    : x_comp_(vec.x_comp_), y_comp_(vec.y_comp_) {}
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
Vector& Vector::operator-=(const Vector& vec) { return *this += vec * -1; }
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
//-----------comparators--------
// (a_v -> another vector)
bool Vector::operator==(const Vector& a_v) const {
  return (x_comp_ == a_v.x_comp_) && (y_comp_ == a_v.y_comp_);
}
bool Vector::operator!=(const Vector& a_v) const { return !(*this == a_v); }
//------------------------------
//------necessery-methods-------
size_t Vector::Len() const {
  // Yes, I know that it is misstake, but I need this method
  return (size_t)(x_comp_ * x_comp_) + y_comp_ * y_comp_;
}
//------------------------------
//==============================

//=============Point============
Point::Point() : x_coord_(0), y_coord_(0) {}
Point::Point(const int64_t& x_c, const int64_t& y_c)
    : x_coord_(x_c), y_coord_(y_c) {}
Point::Point(const Point& pnt)
    : x_coord_(pnt.x_coord_), y_coord_(pnt.y_coord_) {}
Point& Point::operator=(const Point& pnt) {
  x_coord_ = pnt.x_coord_;
  y_coord_ = pnt.y_coord_;
  return *this;
}
int64_t Point::GetX() const { return x_coord_; }
int64_t Point::GetY() const { return y_coord_; }
bool Point::operator==(const Point& pnt) const {
  return (x_coord_ == pnt.x_coord_) && (y_coord_ == pnt.y_coord_);
}
bool Point::operator!=(const Point& pnt) const { return !(*this == pnt); }
void Point::Move(const Vector& vec) {
  x_coord_ += vec.GetX();
  y_coord_ += vec.GetY();
}
bool Point::ContainsPoint(const Point& pnt) const { return *this == pnt; }
bool Point::CrossSegment(const Segment& sgmnt) const {
  Vector p_to_a(*this, sgmnt.GetA());
  Vector p_to_b(*this, sgmnt.GetB());
  if ((p_to_a ^ p_to_b) == 0) {
    return (std::min(sgmnt.GetA().GetX(), sgmnt.GetB().GetX()) <= x_coord_ &&
            std::max(sgmnt.GetA().GetX(), sgmnt.GetB().GetX()) >= x_coord_) &&
           (std::min(sgmnt.GetA().GetY(), sgmnt.GetB().GetY()) <= y_coord_ &&
            std::max(sgmnt.GetA().GetY(), sgmnt.GetB().GetY()) >= y_coord_);
  }
  return false;
}
IShape* Point::Clone() const { return new Point(*this); }
// arifmethic operators
Vector Point::operator-(const Point& pnt) const { return Vector(pnt, *this); }
//
//==============================
//============Segment===========
// construstors
Segment::Segment() { begin_ = end_ = Point(); }
Segment::Segment(const Point& p_first, const Point& p_second)
    : begin_(p_first), end_(p_second) {}
Segment::Segment(const Segment& sgmnt) {
  begin_ = sgmnt.begin_;
  end_ = sgmnt.end_;
}
Segment& Segment::operator=(const Segment& sgmnt) {
  begin_ = sgmnt.begin_;
  end_ = sgmnt.end_;
  return *this;
}
//
// getters
Point Segment::GetA() const { return begin_; }
Point Segment::GetB() const { return end_; }
//
// abstruckt methods
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
IShape* Segment::Clone() const { return new Segment(*this); }
//
// comparators
bool Segment::operator==(const Segment& sgmnt) const {
  return (begin_ == sgmnt.begin_) && (end_ == sgmnt.end_);
}
bool Segment::operator!=(const Segment& sgmnt) const {
  return !(*this == sgmnt);
}
//
// necessery functions
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
//==============================

//=============Line=============
// construcktors
Line::Line() : x_coef_(1), y_coef_(-1), free_mem_(0) {}
Line::Line(const Point& p_first, const Point& p_second) {
  int64_t dif_by_x = p_second.GetX() - p_first.GetX();
  int64_t dif_by_y = p_second.GetY() - p_first.GetY();
  y_coef_ = dif_by_x;
  x_coef_ = -dif_by_y;
  free_mem_ = -p_first.GetY() * dif_by_x + p_first.GetX() * dif_by_y;
  ToNormalizeCoefs();
  p_start_1_ = p_first;
  p_start_2_ = p_second;
}
Line::Line(const Line& lne) {
  x_coef_ = lne.x_coef_;
  y_coef_ = lne.y_coef_;
  free_mem_ = lne.free_mem_;
}
Line& Line::operator=(const Line& lne) {
  x_coef_ = lne.x_coef_;
  y_coef_ = lne.y_coef_;
  free_mem_ = lne.free_mem_;
  return *this;
}
//
// getters
int64_t Line::GetA() const { return x_coef_; }
int64_t Line::GetB() const { return y_coef_; }
int64_t Line::GetC() const { return free_mem_; }
//
// abstruckt methods
void Line::Move(const Vector& vec) {
  p_start_1_.Move(vec);
  p_start_2_.Move(vec);
  *this = Line(p_start_1_, p_start_2_);
}
bool Line::ContainsPoint(const Point& pnt) const {
  return (pnt.GetX() * x_coef_) + (pnt.GetY() * y_coef_) + free_mem_ == 0;
}
bool Line::CrossSegment(const Segment& sgmnt) const {
  Vector guide_vector(p_start_1_, p_start_2_);
  return (guide_vector ^ Vector(p_start_1_, sgmnt.GetA())) *
             (guide_vector ^ Vector(p_start_1_, sgmnt.GetB())) <=
         0;
}
IShape* Line::Clone() const { return new Line(*this); }
//
// comparators
bool Line::operator==(const Line& lne) const {
  return (x_coef_ == lne.x_coef_) && (y_coef_ == lne.y_coef_) &&
         (free_mem_ == lne.y_coef_);
}
bool Line::operator!=(const Line& lne) const { return !(*this == lne); }
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
  // com_divisor -> common divisor
  if (com_divisor == 0) {
    com_divisor = 1;
  }
  x_coef_ /= com_divisor;
  y_coef_ /= com_divisor;
  free_mem_ /= com_divisor;
}
//
//==============================
//=============Ray==============
// constractors
// (a_r -> another ray)
// (p_s -> point first)
// (p_s -> point second)
Ray::Ray() {
  p_start_ = Point();
  g_v_ = Vector();
}
Ray::Ray(const Point& p_f, const Point& p_s) : p_start_(p_f), g_v_(p_s - p_f) {
  ToNormolizeRay();
}
Ray::Ray(const Ray& a_r) : p_start_(a_r.p_start_), g_v_(a_r.g_v_) {}
Ray& Ray::operator=(const Ray& a_r) {
  p_start_ = a_r.p_start_;
  g_v_ = a_r.g_v_;
  return *this;
}
//
// getters
Point Ray::GetA() const { return p_start_; }
Vector Ray::GetVector() const { return g_v_; }
//
// abstract methods
void Ray::Move(const Vector& vec) { p_start_.Move(vec); }
bool Ray::ContainsPoint(const Point& pnt) const {
  return (Ray(p_start_, pnt).g_v_ == Vector()) ? true
                                               : (Ray(p_start_, pnt) == *this);
}
bool Ray::CrossSegment(const Segment& sgmnt) const {
  Point tmp_p = Point(p_start_);
  tmp_p.Move(g_v_);
  Line lne(p_start_, tmp_p);
  if (lne.CrossSegment(sgmnt)) {
    Line lne_seg(sgmnt.GetA(), sgmnt.GetB());
    int64_t det =
        Vector(lne.GetA(), lne.GetB()) ^ Vector(lne_seg.GetA(), lne_seg.GetB());
    Point p_inter;
    // (p_inter -> intersection point)
    if (det == 0) {
      if (Vector(p_start_, sgmnt.GetA()).Len() <
          Vector(p_start_, sgmnt.GetA()).Len()) {
        p_inter = sgmnt.GetA();
      } else {
        p_inter = sgmnt.GetB();
      }
    } else {
      p_inter = Point(-(Vector(lne.GetC(), lne.GetB()) ^
                        Vector(lne_seg.GetC(), lne_seg.GetB())) /
                          det,
                      -(Vector(lne.GetA(), lne.GetC()) ^
                        Vector(lne_seg.GetA(), lne_seg.GetC())) /
                          det);
    }
    return ContainsPoint(p_inter);
  }
  return false;
}  // пока хз
IShape* Ray::Clone() const { return new Ray(*this); }
//
// comparators
bool Ray::operator==(const Ray& a_r) const {
  return (p_start_ == a_r.p_start_) && (g_v_ == a_r.g_v_);
}
bool Ray::operator!=(const Ray& a_r) const { return !(*this == a_r); }
//
// necessery methods
void Ray::ToNormolizeRay() {
  // (com_d -> common divisor)
  int64_t com_d =
      std::__gcd((int64_t)abs(g_v_.GetX()), (int64_t)abs(g_v_.GetY()));
  com_d = (com_d == 0) ? 1 : com_d;
  g_v_ = Vector(g_v_.GetX() / com_d, g_v_.GetY() / com_d);
}
//
//==============================

//=============Circle===========
// constructors
Circle::Circle() : rad_(0) { cnt_ = Point(); }
Circle::Circle(const Point& pnt, const size_t& rad) : cnt_(pnt), rad_(rad) {}
// (crcl -> circle)
Circle::Circle(const Circle& crcl) : cnt_(crcl.cnt_), rad_(crcl.rad_) {}
Circle& Circle::operator=(const Circle& crcl) {
  cnt_ = crcl.cnt_;
  rad_ = crcl.rad_;
  return *this;
}
//
// getters
Point Circle::GetCentre() const { return cnt_; }
size_t Circle::GetRadius() const { return rad_; }
//
// abstract methods
void Circle::Move(const Vector& vec) { cnt_.Move(vec); }
bool Circle::ContainsPoint(const Point& pnt) const {
  return Vector(cnt_, pnt).Len() <= (rad_ * rad_);
}
bool Circle::CrossSegment(const Segment& sgmnt) const {
  size_t seg_len = Vector(sgmnt.GetA(), sgmnt.GetB()).Len();
  if (seg_len == 0) {
    return ContainsPoint(sgmnt.GetA());
  }
  Vector cta(cnt_, sgmnt.GetA());
  // (cta -> vector from center to point A) (ctb same meaning with cta)
  Vector ctb(cnt_, sgmnt.GetB());
  // (area -> Area of triangle which is built by center and segment)
  int64_t area = cta ^ ctb;
  area = abs(area);
  if (area == 0) {
    int64_t gcd_cta = std::__gcd(abs(cta.GetX()), abs(cta.GetY()));
    int64_t gcd_ctb = std::__gcd(abs(ctb.GetX()), abs(ctb.GetY()));
    gcd_cta = (gcd_cta == 0) ? 1 : gcd_cta;
    gcd_ctb = (gcd_ctb == 0) ? 1 : gcd_ctb;
    if (Vector(cta.GetX() / gcd_cta, cta.GetY() / gcd_cta) ==
        -Vector(ctb.GetX() / gcd_ctb, ctb.GetY() / gcd_ctb)) {
      return (std::max(cta.Len(), ctb.Len()) >= rad_ * rad_);
    }
    return (std::min(cta.Len(), ctb.Len()) <= rad_ * rad_);
  }  // if segment is parallel to the radius
  // (hot -> height of triangle)
  size_t hot = (area * area / seg_len);
  size_t len_cta = cta.Len();
  size_t len_ctb = ctb.Len();
  return (std::min(std::min(len_cta, len_ctb), hot) <= rad_ * rad_) &&
         (std::max(len_cta, len_ctb) >= rad_ * rad_);
}
IShape* Circle::Clone() const { return new Circle(*this); }
//
//==============================
