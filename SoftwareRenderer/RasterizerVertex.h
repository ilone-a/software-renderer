#pragma once


#include <vector>
#include "Vector4.h"

template <typename VertexAttributes>
class RasterizerVertex
{
public:
  Vector4 location;
  VertexAttributes attr;
public:
  RasterizerVertex();
  RasterizerVertex(Vector4 location, VertexAttributes param);
};


template<typename VertexAttributes>
inline RasterizerVertex<VertexAttributes>::RasterizerVertex()
{
  location = Vector4(0.0f, 0.0f, 0.0f, 1.0f); //xyzw
}

template<typename VertexAttributes>
inline RasterizerVertex<VertexAttributes>::RasterizerVertex(Vector4 location, VertexAttributes param)
{
  this->location = location;
  attr = param;
}


