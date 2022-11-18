

#include "Renderer.h"
#include "RasterizerVertex.h"
#include <vector>


using namespace std;

struct FrameBufferTest;

class Matrix;
class DepthBuffer;
matrix3x3 matrix_interp;
typedef RasterizerVertex<Vector4> Vertex;

float randomcolor;
Vector4 vcolor;

Renderer::Renderer(SDL_Window* main_window, int width_, int height_)
{
  this->surface = SDL_GetWindowSurface(main_window);
  width = width_;
  height = height_;
  m_scissors = { {0.0f,0.0f }, {float(width), float(height)} };
  this->framebuffertest = new FrameBufferTest(width, height);
}

struct InterpolationContext
{
  row   coeff[3];
  int   x1, y1, x2, y2;
  bool intersect;

  //rectangle resultROI;

  void calculateCoefficients(const Vertex vec4[3])
  {
    coeff[0].m_vary.setFrom(vec4[0]);
    coeff[1].m_vary.setFrom(vec4[1]);
    coeff[2].m_vary.setFrom(vec4[2]);

    matrix3x3 matrix_interp2;
    bool inverted = makeInterpolationMatrix(matrix_interp2, vec4);
    computeCoeff(coeff[0], coeff[1], coeff[2], matrix_interp2);
  };

  bool  calculateROI(const Vertex vec4[3], float width, float height, Extents2f m_scissors)//calculate region of interests
  {
    float min_x = min3(vec4[0].location.x / vec4[0].location.w, vec4[1].location.x / vec4[1].location.w, vec4[2].location.x / vec4[2].location.w);
    float min_y = min3(vec4[0].location.y / vec4[0].location.w, vec4[1].location.y / vec4[1].location.w, vec4[2].location.y / vec4[2].location.w);
    float max_x = max3(vec4[0].location.x / vec4[0].location.w, vec4[1].location.x / vec4[1].location.w, vec4[2].location.x / vec4[2].location.w);
    float max_y = max3(vec4[0].location.y / vec4[0].location.w, vec4[1].location.y / vec4[1].location.w, vec4[2].location.y / vec4[2].location.w);

    //intersect = intersectRectangle(resultROI, { 0.0f, 0.0f, float(width), float(height) }, { min_x, min_y, max_x, max_y });

    min_x = fmax(min_x, m_scissors.minPoint().x);
    min_y = fmax(min_y, m_scissors.minPoint().y);
    max_x = fmin(max_x, m_scissors.maxPoint().x);
    max_y = fmin(max_y, m_scissors.maxPoint().y);


    //clipped float to int
    x1 = pixelIndex(min_x);
    y1 = pixelIndex(min_y);
    x2 = pixelIndex(max_x);
    y2 = pixelIndex(max_y);

    return (x1 < x2) && (y1 < y2);
  }
  /*
  bool intersectRectangle(rectangle& res, const rectangle& a, const rectangle& b)
  {
    res.l = fmax(a.l, b.l);
    res.t = fmax(a.t, b.t);
    res.r = fmin(a.r, b.r);
    res.b = fmin(a.b, b.b);
    return (res.l < res.r) && (res.t < res.b);
  }*/


  bool setup(const Vertex vec4[3], const Renderer& renderer0)
  {
    calculateCoefficients(vec4);
    return calculateROI(vec4, renderer0.width, renderer0.height, renderer0.m_scissors);
  }


  static  void computeCoeff(float& c0, float& c1, float& c2, const matrix3x3& im)
  {
    float v0 = c0, v1 = c1, v2 = c2;
    c0 = im.entry[0][0] * v0 + im.entry[0][1] * v1 + im.entry[0][2] * v2;
    c1 = im.entry[1][0] * v0 + im.entry[1][1] * v1 + im.entry[1][2] * v2;
    c2 = im.entry[2][0] * v0 + im.entry[2][1] * v1 + im.entry[2][2] * v2;
  }

  static  void computeCoeff(row& c0, row& c1, row& c2, const matrix3x3& m)
  {
    computeCoeff(c0.m_edge, c1.m_edge, c2.m_edge, m);
    computeCoeff(c0.m_vary, c1.m_vary, c2.m_vary, m);
  }

  static  void computeCoeff(edge& c0, edge& c1, edge& c2, const matrix3x3& m)
  {
    //edge
    c0 = { 1.0f, 1.0f, 0.0, 0.0 };
    c1 = { 1.0f, 0.0f, 1.0, 0.0 };
    c2 = { 1.0f, 0.0f, 0.0, 1.0 };

    c0[0] = m.entry[0][0] + m.entry[0][1] + m.entry[0][2];
    c1[0] = m.entry[1][0] + m.entry[1][1] + m.entry[1][2];
    c2[0] = m.entry[2][0] + m.entry[2][1] + m.entry[2][2];

    c0[1] = m.entry[0][0];
    c1[1] = m.entry[1][0];
    c2[1] = m.entry[2][0];

    c0[2] = m.entry[0][1];
    c1[2] = m.entry[1][1];
    c2[2] = m.entry[2][1];

    c0[3] = m.entry[0][2];
    c1[3] = m.entry[1][2];
    c2[3] = m.entry[2][2];
  }

  static  void computeCoeff(varyings& v0, varyings& v1, varyings& v2, const matrix3x3& m)
  {
    for (int i = 0; i < 4; ++i)//vary.length
    {
      computeCoeff(v0[i], v1[i], v2[i], matrix_interp);
    }
  }

  bool makeInterpolationMatrix(matrix3x3& m, const Vertex t[3])
  {
    //check vertices
    for (int i = 0; i < 3; i++)
    {
      m.entry[i][0] = t[i].location.x;
      m.entry[i][1] = t[i].location.y;
      m.entry[i][2] = t[i].location.w;
    }
    bool inverted = false;
    matrix_inv(m, inverted);
    return inverted;
  }
  matrix3x3& matrix_inv(matrix3x3& m, bool& inv)
  {
    float det = m.entry[0][0] * (m.entry[1][1] * m.entry[2][2] - m.entry[2][1] * m.entry[1][2]) - m.entry[0][1] * (m.entry[1][0] * m.entry[2][2] - m.entry[1][2] * m.entry[2][0]) +
      m.entry[0][2] * (m.entry[1][0] * m.entry[2][1] - m.entry[1][1] * m.entry[2][0]);
    if (det == 0) { inv = false; return m; }
    float invdet = 1 / det;
    matrix3x3 temp = m;

    //inverse
    m.entry[0][0] = (temp.entry[1][1] * temp.entry[2][2] - temp.entry[2][1] * temp.entry[1][2]) * invdet;
    m.entry[0][1] = (temp.entry[0][2] * temp.entry[2][1] - temp.entry[0][1] * temp.entry[2][2]) * invdet;
    m.entry[0][2] = (temp.entry[0][1] * temp.entry[1][2] - temp.entry[0][2] * temp.entry[1][1]) * invdet;
    m.entry[1][0] = (temp.entry[1][2] * temp.entry[2][0] - temp.entry[1][0] * temp.entry[2][2]) * invdet;
    m.entry[1][1] = (temp.entry[0][0] * temp.entry[2][2] - temp.entry[0][2] * temp.entry[2][0]) * invdet;
    m.entry[1][2] = (temp.entry[1][0] * temp.entry[0][2] - temp.entry[0][0] * temp.entry[1][2]) * invdet;
    m.entry[2][0] = (temp.entry[1][0] * temp.entry[2][1] - temp.entry[2][0] * temp.entry[1][1]) * invdet;
    m.entry[2][1] = (temp.entry[2][0] * temp.entry[0][1] - temp.entry[0][0] * temp.entry[2][1]) * invdet;
    m.entry[2][2] = (temp.entry[0][0] * temp.entry[1][1] - temp.entry[1][0] * temp.entry[0][1]) * invdet;

    inv = true;
    return m;
  }

  inline int pixelIndex(float x) { return (int)ceilf(x - 0.5f); }
};




void Renderer::clear()
{
  SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0));
  //this->zbuffer->clearzbuffer();
   //CLEAR_DATA clear = { { 0,0,0,0 }, 100500.0f };
  framebuffertest->clear({ { 0,0,0,0 }, 1000000.0f });
}

void Renderer::render_triangle(const RasterizerVertex<Vector4> vec4[3])

{
  randomcolor = rand() % 256;
  Vertex vec1[3];
  vec1[0] = vec4[0]; vec1[1] = vec4[1]; vec1[2] = vec4[2];

  InterpolationContext ctx;
  if (!ctx.setup(vec1, *this))
  {
    return;
  }

 //test
  //DataOverlay < float[3], float > arraytest;

  //set something to varyings
  ctx.coeff[0].m_vary = vec1[0].location;
  ctx.coeff[1].m_vary = vec1[1].location;
  ctx.coeff[2].m_vary = vec1[2].location;

  row scanline, fragment;

  for (int i = 0; i < 4; i++)
  {
    scanline.m_edge[i] = evaluate(ctx.coeff[0].m_edge[i], ctx.coeff[1].m_edge[i], ctx.coeff[2].m_edge[i], (ctx.x1 + 0.5f), (ctx.y1 + 0.5f));
    scanline.m_vary[i] = evaluate(ctx.coeff[0].m_vary[i], ctx.coeff[1].m_vary[i], ctx.coeff[2].m_vary[i], (ctx.x1 + 0.5f), (ctx.y1 + 0.5f));
  }

  //fill depth buffer
  for (int y = ctx.y1; y < ctx.y2; ++y)
  {
    fragment = scanline; //начальный фрагмент сканлайна
    for (int x = ctx.x1; x < ctx.x2; ++x)
    {
      if (fragment.m_edge.isHit())
      {
        applyPixelFunctor(x, y, fragment);
      }
      fragment.m_edge += ctx.coeff[0].m_edge; // сдвинули по X
    }
    scanline.m_edge += ctx.coeff[1].m_edge; //Y
  }


}


void Renderer::put_pixel_to_surface(int x, int y, int r, int g, int b)
{
  //convert pixels to Unt32 
  Uint32* scanline_pixels = (Uint32*)(((char*)surface->pixels) + y * surface->pitch);
  scanline_pixels[x] = SDL_MapRGB(surface->format, r, g, b);
}

void Renderer::applyPixelFunctor(int x, int y, const row& uncorrected_frag)
{
  row dst = uncorrected_frag;
  bool resultdepth = false;
  bool resultstencil = false;


  shaderDescription scenario;
  scenario.depth_test = true; scenario.stencil_test = true;

  if (scenario.depth_test == true)
  {
    resultdepth = framebuffertest->depthTest(x, y, dst.m_vary.va3);
  }
  if (scenario.stencil_test == true)
  {
    resultstencil = framebuffertest->stencilTest(x, y);
  }
  {
    if (scenario.depth_test == resultdepth && scenario.stencil_test == resultstencil)
    {
     vcolor = framebuffertest->expcolor(x, y, width, height);
      this->put_pixel_to_surface(x, y, randomcolor, randomcolor, randomcolor);
      //this->put_pixel_to_surface(x, y, vcolor.x*256, vcolor.y*256, vcolor.z*256);
    }
  }
}


