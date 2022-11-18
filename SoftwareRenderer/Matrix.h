#pragma once

enum GeConstruct { kTrivial };

class Matrix
{
  static const Matrix kIdentity;


public:
  float rc[4][4]; //get row get column
  Matrix() { this->setToIdentity(); }
  Matrix(GeConstruct) {};

  void setToIdentity();

  void setToTranslation(float x, float y, float z);
  void setToRotationX(float angle);
  void setToScaling(float x, float y, float z);

  void setToScreenspace(float width, float height);
  void setToPerspective(float fov, float aspect, float near, float far);

public:


  static Matrix translation(float x, float y, float z)
  {
    Matrix m(GeConstruct::kTrivial);
    m.setToTranslation(x, y, z); return m;
  };

  static Matrix rotation(float angle)
  {
    Matrix m(GeConstruct::kTrivial);
    m.setToRotationX(angle); return m;
  };
  static Matrix scaling(float x, float y, float z)
  {
    Matrix m(GeConstruct::kTrivial);
    m.setToScaling(x, y, z); return m;
  };

  static Matrix screenspace(float width, float height)
  {
    Matrix m(GeConstruct::kTrivial);
    m.setToScreenspace(width, height); return m;
  };

  static Matrix perspective(float fov, float aspect, float near, float far)
  {
    Matrix m(GeConstruct::kTrivial);
    m.setToPerspective(fov, aspect, near, far); return m;
  };

  /*
  template <typename T> inline size_t startOf(const T& a) { return (size_t)(&a); }

  template <typename T> inline size_t endOf(const T& a) { return startOf(a) + sizeof(a); }

  template <typename A, typename B> inline bool isAfter(const A& a, const B& b) { return startOf(b) >= endOf(a); }

  template <typename A, typename B> inline bool isNotOverlap(const A& a, const B& b)
  {   return isAfter(a, b) || isAfter(b, a); }

  template <typename A, typename B> inline bool isOverlap(const A& a, const B& b)
  { return !isNotOverlap(a, b); }*/




  Matrix operator * (const Matrix& m) const
  {
    Matrix res(GeConstruct::kTrivial);
    mtxMultiply(res, *this, m);
    return res;
  }

  static Matrix& mtxMultiply(Matrix& r, const Matrix& a, const Matrix& b)
  {
    if ((&r != &a) && (&r != &b))
      return mtxMultiplyNoOverlap(r, a, b);
    Matrix tmp(kTrivial);
    mtxMultiplyNoOverlap(tmp, a, b);
    return (r = tmp);
  }

  static Matrix& mtxMultiplyNoOverlap(Matrix& r, const Matrix& a, const Matrix& b)
  {
    for (int k = 0; k < 4; k++) {
      for (int j = 0; j < 4; j++) {
        float s = 0;
        for (int i = 0; i < 4; i++) {
          s += a.rc[k][i] * b.rc[i][j];
        }
        r.rc[k][j] = s;
      }
    }
    return r;
  }

};


