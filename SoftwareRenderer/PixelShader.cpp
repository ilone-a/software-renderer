#include "PixelShader.h"

bool FrameBufferTest::stencilTest(int x, int y)
{
    int index = y * m_size.cx + x;
    if (m_stencil[index] == 1)
    {
      return true;
    }
    else { return false; }
}


bool FrameBufferTest::depthTest(int x, int y, float depth)
{
 // bool result = true;
  bool result = false;
  int depth_index = (y * m_size.cx) + x;
  if (m_depth[depth_index] > depth)
  {
    int colorint = int(depth* 256/500000);//EDIT
    m_depth[depth_index] = depth;
    m_color[depth_index] = { byte(colorint), byte(colorint), byte(colorint),byte(colorint) };
    result = true;
  }
  return result;
};

void FrameBufferTest::get_color(int x, int y, COLOR& color0)
{
    int index = y * m_size.cx + x;
    color0 = m_color[index];
}
