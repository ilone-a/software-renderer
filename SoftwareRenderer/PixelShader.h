#pragma once

#include "SDL.h"
#include <limits>
#include "Renderer.h"
#include "windows.h"

//#include "Vector4.h"
#include "RasterizerVertex.h"


struct row;



struct SIZEF { int cx; int cy; };
struct RGBA {
  BYTE r, g, b, a;
public:
  RGBA()
  {
    r = 0, g = 0, b = 0, a = 0;
  }
  RGBA(BYTE r, BYTE g, BYTE b, BYTE a)
  {
    this->r = r, this->g = g, this->b = b, this->a = a;
  }
};
typedef RGBA  COLOR;
typedef float DEPTH;
typedef BYTE  STENCIL;

struct shaderDescription
{
  bool stencil_test;
  bool depth_test;

  shaderDescription() { stencil_test = false, depth_test = false; }

  shaderDescription(const bool stencil, const bool depth)
  {
    stencil_test = stencil;
    depth_test = depth;
  }
};

struct CLEAR_DATA { 
  RGBA m_color; 
  DEPTH m_depth; 
#undef max //windows.h
  CLEAR_DATA()
  {
    m_color = { 0,0,0,0 };
    m_depth = std::numeric_limits<float>::max();
  }
  CLEAR_DATA(RGBA color_, float depth_)
  {
    m_color = color_, m_depth = depth_;
  }
};

struct FrameBufferTest
{
public:
  SIZEF m_size;
  COLOR* m_color;
  DEPTH* m_depth;
  STENCIL* m_stencil;
  float  maxvalue;
  time_t          time;


  FrameBufferTest(int w, int h)
  {
    m_size.cx = w, m_size.cy = h;
    m_color = new COLOR[w * h];
    m_depth = new DEPTH[w * h];
    m_stencil = new BYTE[w * h];
    clear({ { 0,0,0,0 }, 100000000.0f });
    //#undef max //windows.h
    maxvalue = std::numeric_limits<float>::max();
 // time =  time(NULL);

  }
  ~FrameBufferTest() {
    delete[] m_stencil; delete[] m_color;  delete[] m_depth;
  }
  void clear(const CLEAR_DATA &data) 
  {
    for (int i = 0; i < m_size.cx * m_size.cy; i++)
    {
      m_depth[i] = data.m_depth;

      //m_color[i] = data.m_color;
      m_color[i] = data.m_color;
      m_stencil[i] = 1;
    }
  }

  void get_color(int x, int y, COLOR& color0);

  bool stencilTest(int x, int y);

  bool depthTest(int x, int y, float depth);

  Vector4 expcolor(int x, int y, const int width, const int height) const

  {
    float posx = x / width * 4;
    float  posy = y / height * 4;


    for (int n = 1; n < 100; n++) {
      float i = float(n);
      posx += (0.7 / i * sin(i * posy * i + time / 10. + 0.2 * i) + 0.8, 0.4 / i * sin(i * posx * i + time / 10. + 0.2 * i) + 1.6);
      posy += (0.7 / i * sin(i * posy * i + time / 10. + 0.2 * i) + 0.8, 0.4 / i * sin(i * posx * i + time / 10. + 0.2 * i) + 1.6);

    }
    float colorr = 0.5 * sin(posx) + 0.5;

    Vector4 result = Vector4{ colorr, 0.0f, 0.0f, 1.0f };
    return result;

  }
};

