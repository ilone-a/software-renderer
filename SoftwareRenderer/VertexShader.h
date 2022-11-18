#pragma once
#include "Matrix.h"
#include "Vector4.h"
#include "Renderer.h"
#include "GeometryPrimitives.h"

//struct row;

class VertexShader
{
  int width_, height_;

public:
  VertexShader(int w, int h) { width_ = w, height_ = h; }
  Matrix& modelToScreen(Matrix& matrixToCreate);
  bool check_w_position(const Vector4& position);
  void transform_triangle(RasterizerVertex<Vector4> vec4[3], Matrix& transform_matrix);

  float calculateLightIntencity(const RasterizerVertex<Vector4> vec4[3], const Vector4& light_dir);
};

