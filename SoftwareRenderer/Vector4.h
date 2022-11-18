#pragma once
#include "Matrix.h"

struct Vector4
{
  float x, y, z, w;
  float& operator[] (int i) { return (&x)[i]; }

  Vector4()
  {
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    w = 0.0f;
  }

  Vector4(float x0, float y0, float z0, float w0)
  {
    x = x0;
    y = y0;
    z = z0;
    w = w0;
  }

  void transformBy(const Matrix& m)
  {
    *this = transform2(m, *this);
  };

  Vector4 transform2(const Matrix& m, const Vector4& a) const
  {
    //*this = M * (*this);
    Vector4 result;
    for (int i = 0; i < 4; i++)
    {
      float s = m.rc[i][0] * a.x + m.rc[i][1] * a.y +
        m.rc[i][2] * a.z + m.rc[i][3] * a.w;
      result[i] = s;
    };
    return result;
  }

  void operator /= (float value)
  {
    x = x / value;
    y = y / value;
    z = z / value,
    w = w / value;
  };
};

