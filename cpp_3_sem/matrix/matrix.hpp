#pragma once
#include <algorithm>
#include <vector>  // сделать новый класс для квадратных матриц

template <size_t N, size_t M, typename T = int64_t>
class Matrix {
 public:
  // constructors
  Matrix();
  Matrix(const std::vector<std::vector<T>>& tmp);
  Matrix(const T& elem);
  Matrix(const Matrix<N, M, T>& mtrx);
  Matrix<N, M, T>& operator=(const Matrix<N, M, T>& mtrx);
  ~Matrix();
  //
  // arithmetic operators
  Matrix<N, M, T>& operator+=(const Matrix<N, M, T>& mtrx);
  Matrix<N, M, T>& operator-=(const Matrix<N, M, T>& mtrx);
  Matrix<N, M, T>& operator*=(const T& elem);

  Matrix<N, M, T> operator+(const Matrix<N, M, T>& mtrx) const;
  Matrix<N, M, T> operator-(const Matrix<N, M, T>& mtrx) const;
  Matrix<N, M, T> operator*(const T& elem) const;
  template <size_t K>
  Matrix<N, K, T> operator*(const Matrix<M, K, T>& mtrx) const;
  //
  // access modifiers
  T& operator()(size_t row, size_t col);
  const T& operator()(size_t row, size_t col) const;
  //
  // other modificators
  Matrix<M, N, T> Transposed() const;
  //
  // comporation
  bool operator==(const Matrix<N, M, T>& mtrx) const;
  bool operator!=(const Matrix<N, M, T>& mtrx) const;
  //

 private:
  std::vector<std::vector<T>> body_;
};

//==================NOT=SQR========================
//               constructors
template <size_t N, size_t M, typename T>
Matrix<N, M, T>::Matrix() {
  body_.resize(N, std::vector<T>(M));
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>::Matrix(const T& elem) {
  body_.resize(N, std::vector<T>(M, elem));
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>::Matrix(const std::vector<std::vector<T>>& tmp) {
  body_ = tmp;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>::Matrix(const Matrix<N, M, T>& mtrx) {
  body_ = mtrx.body_;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>& Matrix<N, M, T>::operator=(const Matrix<N, M, T>& mtrx) {
  return *this = mtrx;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>::~Matrix() = default;
// -------------------------------------------

//            ariphmetic opecrations
template <size_t N, size_t M, typename T>
Matrix<N, M, T>& Matrix<N, M, T>::operator+=(const Matrix<N, M, T>& mtrx) {
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      body_[i][j] += mtrx.body_[i][j];
    }
  }
  return *this;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>& Matrix<N, M, T>::operator-=(const Matrix<N, M, T>& mtrx) {
  return *this += mtrx * -1;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>& Matrix<N, M, T>::operator*=(const T& elem) {
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      body_[i][j] *= elem;
    }
  }
  return *this;
}

template <size_t N, size_t M, typename T>
template <size_t K>
Matrix<N, K, T> Matrix<N, M, T>::operator*(const Matrix<M, K, T>& mtrx) const {
  Matrix<N, K, T> res_of_mult;
  Matrix<K, M, T> tr_mtrx = mtrx.Transposed();
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < K; ++j) {
      for (size_t k = 0; k < M; ++k) {
        res_of_mult(i, j) += body_[i][k] * tr_mtrx(j, k);
      }
    }
  }
  return res_of_mult;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T> Matrix<N, M, T>::operator+(const Matrix<N, M, T>& mtrx) const {
  return Matrix<N, M, T>(*this) += mtrx;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T> Matrix<N, M, T>::operator-(const Matrix<N, M, T>& mtrx) const {
  return Matrix<N, M, T>(*this) -= mtrx;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T> Matrix<N, M, T>::operator*(const T& elem) const {
  return Matrix<N, M, T>(*this) *= elem;
}
// -------------------------------------------

//              assess modifiers
template <size_t N, size_t M, typename T>
T& Matrix<N, M, T>::operator()(size_t row, size_t col) {
  return body_[row][col];
}

template <size_t N, size_t M, typename T>
const T& Matrix<N, M, T>::operator()(size_t row, size_t col) const {
  return body_[row][col];
}
// -------------------------------------------

//     other operators and modificators
template <size_t N, size_t M, typename T>
Matrix<M, N, T> Matrix<N, M, T>::Transposed() const {
  Matrix<M, N, T> mtrx;
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      mtrx(j, i) = body_[i][j];
    }
  }
  return mtrx;
}
// -------------------------------------------

//                comporation
template <size_t N, size_t M, typename T>
bool Matrix<N, M, T>::operator==(const Matrix<N, M, T>& mtrx) const {
  return body_ == mtrx.body_;
}

template <size_t N, size_t M, typename T>
bool Matrix<N, M, T>::operator!=(const Matrix<N, M, T>& mtrx) const {
  return *this != mtrx;
}
// -------------------------------------------
//=================================================

template <size_t N, typename T>
class Matrix<N, N, T> {
 public:
  // constructors
  Matrix();
  Matrix(const std::vector<std::vector<T>>& tmp);
  Matrix(const T& elem);
  Matrix(const Matrix<N, N, T>& mtrx);
  Matrix<N, N, T>& operator=(const Matrix<N, N, T>& mtrx);
  ~Matrix();
  //
  // arithmetic operators
  Matrix<N, N, T>& operator+=(const Matrix<N, N, T>& mtrx);
  Matrix<N, N, T>& operator-=(const Matrix<N, N, T>& mtrx);
  Matrix<N, N, T>& operator*=(const T& elem);

  Matrix<N, N, T> operator+(const Matrix<N, N, T>& mtrx) const;
  Matrix<N, N, T> operator-(const Matrix<N, N, T>& mtrx) const;
  Matrix<N, N, T> operator*(const T& elem) const;
  Matrix<N, N, T> operator*(const Matrix<N, N, T>& mtrx) const;
  //
  // access modifiers
  T& operator()(size_t row, size_t col);
  const T& operator()(size_t row, size_t col) const;
  //
  // other modificators
  Matrix<N, N, T> Transposed() const;
  T Trace() const;
  //
  // comporation
  bool operator==(const Matrix<N, N, T>& mtrx) const;
  bool operator!=(const Matrix<N, N, T>& mtrx) const;
  //

 private:
  std::vector<std::vector<T>> body_;
};

//===================SQR========================
//               constructors
template <size_t N, typename T>
Matrix<N, N, T>::Matrix() {
  body_.resize(N, std::vector<T>(N));
}

template <size_t N, typename T>
Matrix<N, N, T>::Matrix(const T& elem) {
  body_.resize(N, std::vector<T>(N, elem));
}

template <size_t N, typename T>
Matrix<N, N, T>::Matrix(const std::vector<std::vector<T>>& tmp) {
  body_ = tmp;
}

template <size_t N, typename T>
Matrix<N, N, T>::Matrix(const Matrix<N, N, T>& mtrx) {
  body_ = mtrx.body_;
}

template <size_t N, typename T>
Matrix<N, N, T>& Matrix<N, N, T>::operator=(const Matrix<N, N, T>& mtrx) {
  return *this = mtrx;
}

template <size_t N, typename T>
Matrix<N, N, T>::~Matrix() = default;
// -------------------------------------------

//            ariphmetic opecrations
template <size_t N, typename T>
Matrix<N, N, T>& Matrix<N, N, T>::operator+=(const Matrix<N, N, T>& mtrx) {
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < N; ++j) {
      body_[i][j] += mtrx.body_[i][j];
    }
  }
  return *this;
}

template <size_t N, typename T>
Matrix<N, N, T>& Matrix<N, N, T>::operator-=(const Matrix<N, N, T>& mtrx) {
  return *this += mtrx * -1;
}

template <size_t N, typename T>
Matrix<N, N, T>& Matrix<N, N, T>::operator*=(const T& elem) {
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < N; ++j) {
      body_[i][j] *= elem;
    }
  }
  return *this;
}

template <size_t N, typename T>
Matrix<N, N, T> Matrix<N, N, T>::operator*(const Matrix<N, N, T>& mtrx) const {
  Matrix<N, N, T> res_of_mult;
  Matrix<N, N, T> tr_mtrx = mtrx.Transposed();
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < N; ++j) {
      for (size_t k = 0; k < N; ++k) {
        res_of_mult(i, j) += body_[i][k] * tr_mtrx(j, k);
      }
    }
  }
  return res_of_mult;
}

template <size_t N, typename T>
Matrix<N, N, T> Matrix<N, N, T>::operator+(const Matrix<N, N, T>& mtrx) const {
  return Matrix<N, N, T>(*this) += mtrx;
}

template <size_t N, typename T>
Matrix<N, N, T> Matrix<N, N, T>::operator-(const Matrix<N, N, T>& mtrx) const {
  return Matrix<N, N, T>(*this) -= mtrx;
}

template <size_t N, typename T>
Matrix<N, N, T> Matrix<N, N, T>::operator*(const T& elem) const {
  return Matrix<N, N, T>(*this) *= elem;
}
// -------------------------------------------

//              assess modifiers
template <size_t N, typename T>
T& Matrix<N, N, T>::operator()(size_t row, size_t col) {
  return body_[row][col];
}

template <size_t N, typename T>
const T& Matrix<N, N, T>::operator()(size_t row, size_t col) const {
  return body_[row][col];
}
// -------------------------------------------

//     other operators and modificators
template <size_t N, typename T>
Matrix<N, N, T> Matrix<N, N, T>::Transposed() const {
  Matrix<N, N, T> mtrx;
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < N; ++j) {
      mtrx(j, i) = body_[i][j];
    }
  }
  return mtrx;
}

template <size_t N, typename T>
T Matrix<N, N, T>::Trace() const {
  T res_of_trace = 0;
  for (size_t i = 0; i < N; ++i) {
    res_of_trace += body_[i][i];
  }
  return res_of_trace;
}
// -------------------------------------------

//                comporation
template <size_t N, typename T>
bool Matrix<N, N, T>::operator==(const Matrix<N, N, T>& mtrx) const {
  return body_ == mtrx.body_;
}

template <size_t N, typename T>
bool Matrix<N, N, T>::operator!=(const Matrix<N, N, T>& mtrx) const {
  return *this != mtrx;
}
// -------------------------------------------
//=================================================
