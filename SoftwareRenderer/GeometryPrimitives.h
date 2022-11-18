#pragma once
#include <iostream>
#include <assert.h> 
#include "Matrix.h"

template <typename ITEM, int LEN>
struct ArrayN
{
  typedef ITEM element;
  static const int length = LEN;

protected:
  element m_data[length];

public:

  void operator += (const ArrayN& a)
  {
    if (length == a.length)
    {
      for (int i = 0; i < length; i++)
      {
        this[i] = this[i] + a[i];
      }
    }
  }

  static bool checkIndex(int i) { return i >= 0 && i < length; }

  const element& at(int i) const
  {
    assert(checkIndex(i));
    return m_data[i];
  }

  element& at(int i)
  {
    assert(checkIndex(i));
    return m_data[i];
  }

  const element& operator[](int i) const
  {
    return at(i);
  }
  element& operator[](int i)
  {
    return at(i);
  }
};

//+= и setToXXX

template <typename DATA, typename ITEM>
struct DataOverlay
{
  static const int length = sizeof(DATA) / sizeof(ITEM); // формула неправильная, но смысл понятен
protected:
  union
  {
    ArrayN<ITEM, length> m_array;
    DATA m_data;
  };
};


struct MatrixRO
{
  float m_entry[4][4];
  float& at(int r, int c) { return m_entry[r][c]; }
};
struct MatrixCO
{
  float m_entry[4][4];
  float& at(int r, int c) { return m_entry[c][r]; }
};

/*
template <typename mtx>// MatrixRow or MatrixColumn
void transformPoint(Point& res, const mtx& m, const Point& src)
{
  res.x = m.at(0, 0) * src.x + m.at(0, 1) * src.y + m.at(0, 2) * src.z + m.at(0, 3);

};*/

struct Point3d
{
  float x, y, z;
  float& operator[] (int i) { return (&x)[i]; }

  Point3d()
  {
    x = 0.0f; y = 0.0f; z = 0.0f;
  }
  Point3d(float x0, float y0, float z0)
  {
    x = x0; y = y0; z = z0;
  }

  static void pntTransform(Point3d& dst, const Matrix& m, const Point3d& src)
  {
    float divider = m.rc[3][0] * src.x + m.rc[3][1] * src.y + m.rc[3][2] * src.z + m.rc[3][3];

    float x = (m.rc[0][0] * dst.x + m.rc[0][1] * dst.y + m.rc[0][2] * dst.z + m.rc[0][3]) / divider;
    float y = (m.rc[1][0] * dst.x + m.rc[1][1] * dst.y + m.rc[1][2] * dst.z + m.rc[1][3]) / divider;
    float z = (m.rc[2][0] * dst.x + m.rc[2][1] * dst.y + m.rc[2][2] * dst.z + m.rc[2][3]) / divider;

    dst.x = x; dst.y = y; dst.z = z;
  }
};

struct hPoint2d
{
  float x, y, w;
  float& operator[] (int i) { return (&x)[i]; }

  hPoint2d()
  {
    x = 0.0f; y = 0.0f; w = 0.0f;
  }
  hPoint2d(float x0, float y0, float z0)
  {
    x = x0; y = y0; w = z0;
  }
  void setToProduct(hPoint2d& hp, const Matrix& M, const Point3d& p)
  {
    hp.x = (M.rc[0][0] * p.x + M.rc[0][1] * p.y + M.rc[0][2] * p.z + M.rc[0][3]);
    hp.y = (M.rc[1][0] * p.x + M.rc[1][1] * p.y + M.rc[1][2] * p.z + M.rc[1][3]);
    //hp.z = (M[2][0]*p.x + M[2][1]*p.y + M[2][2]*p.z + M[2][3]);
    hp.w = (M.rc[3][0] * p.x + M.rc[3][1] * p.y + M.rc[3][2] * p.z + M.rc[3][3]);

  }
};


