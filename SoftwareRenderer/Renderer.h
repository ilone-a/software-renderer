#pragma once

#include <math.h>
#include <iostream>
#include "SDL.h"
#include "Vector4.h"
#include "Matrix.h"
#include "RasterizerVertex.h"
#include "PixelShader.h"

#include "GeometryPrimitives.h"


struct FrameBufferTest;

struct Point2f
{
  float x, y;
  Point2f() { x = 0, y = 0; }
  Point2f(float x_, float y_) { x = x_, y = y_; }
};

struct Extents2f
{
  const Point2f& minPoint() const { return m_min; }
  Point2f& minPoint() { return m_min; }
  const Point2f& maxPoint() const { return m_max; }
  Point2f& maxPoint() { return m_max; }

  Extents2f() { m_min = { 0,0 }, m_max = { 0,0 }; }
  Extents2f(Point2f m_min_, Point2f m_max_) {
    m_min = m_min_;
    m_max = m_max_;
  }

protected:
  Point2f m_min;
  Point2f m_max;
};

struct matrix3x3 { float entry[3][3]; };

__forceinline float max3(float x, float y, float z)
{
  return x > y ? (x > z ? x : z)
    : (y > z ? y : z);
}
__forceinline float min3(float x, float y, float z)
{
  return x < y ? (x < z ? x : z)
    : (y < z ? y : z);
}


struct edge
{
  union {
    struct { float inverseW, e0, e1, e2; };
    float c[4];
  };
  float& operator[] (int i) { return c[i]; }

  bool  isHit() const { return e0 > 0 && e1 > 0 && e2 > 0; }

  edge& operator+=(const edge& e)
  {
    c[0] += e.c[0];
    c[1] += e.c[1];
    c[2] += e.c[2];
    c[3] += e.c[3];
    return *this;
  }
};


struct varyings
{
  union {
    struct { float va0, va1, va2, va3; };
    float c[4];
  };
  float& operator[] (int i) { return c[i]; }
  void  operator = (const Vector4& val)
  {
    va0 = val.x; va1 = val.y; va2 = val.z; va3 = val.w;
  };
  void setFrom(const RasterizerVertex<Vector4>& v)
  {
    *this = v.location; // operator=(const& vector4)
  }
};

struct row
{
  edge     m_edge;
  varyings m_vary;
};

//struct rectangle { float l, t, r, b; };

class Renderer

{
public:

  matrix3x3& matrix_inv(matrix3x3& m, bool& inv);
  Extents2f m_scissors;


  Renderer(SDL_Window* main_window, int width, int height);

  void clear();
  void render_triangle(const RasterizerVertex<Vector4> vec4[3]);
  void put_pixel_to_surface(int x, int y, int r, int g, int b);

  FrameBufferTest* framebuffertest;

  float evaluate(float& c0, float& c1, float& c2, float x, float y)
  {
    return c0 * x + c1 * y + c2;
  };



  void applyPerspectiveCorrection(row& dst, const row& fragment)
  {
    dst.m_edge = fragment.m_edge; //

    dst.m_vary = { fragment.m_vary.va0 / fragment.m_edge.inverseW,
    fragment.m_vary.va1 / fragment.m_edge.inverseW,
    fragment.m_vary.va2 / fragment.m_edge.inverseW,
    fragment.m_vary.va3 / fragment.m_edge.inverseW };
  }

  void applyPixelFunctor(int x, int y, const row& uncorrected_fragment);

  float calculateDepthValue(varyings vary, Vector4 barycentric)
  {
    float result = vary.va0 * barycentric.x + vary.va1 * barycentric.y + vary.va2 * barycentric.z;
    return result;
  }


  SDL_Surface* surface;
  int width, height;
};

