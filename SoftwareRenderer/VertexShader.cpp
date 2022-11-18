#include "VertexShader.h"



Matrix& VertexShader::modelToScreen(Matrix& matrixToCreate)
{
  Matrix transform = Matrix::translation(0.05f, 0.05f, -20.0f);
  Matrix rotate = Matrix::rotation(0.025f);
  Matrix scale = Matrix::scaling(100.0f, 100.0f, 100.0f);
  Matrix perspective = Matrix::perspective((float)M_PI / 1.5f, float(width_ / height_), 0.01f, 1000.0f);
  Matrix screen = Matrix::screenspace(float(width_), float(height_));

  matrixToCreate = screen * perspective * transform * rotate * scale;

  return matrixToCreate;
}

bool VertexShader::check_w_position(const Vector4& position)
{

  bool result = position.x < -position.w || position.x > position.w ||
    position.y < -position.w || position.y > position.w ||
    position.z < -position.w || position.z > position.w;
  return result;
}


void VertexShader::transform_triangle(RasterizerVertex<Vector4> vec4[3], Matrix& transform_matrix)
{
  Point3d points[3];
  Point3d dst[3];
  points[0] = { vec4[0].location.x, vec4[0].location.y, vec4[0].location.z };
  points[1] = { vec4[0].location.x, vec4[0].location.y, vec4[0].location.z };
  points[2] = { vec4[0].location.x, vec4[0].location.y, vec4[0].location.z };

  points[0].pntTransform(dst[0], transform_matrix, points[0]);
  points[1].pntTransform(dst[1], transform_matrix, points[1]);
  points[2].pntTransform(dst[2], transform_matrix, points[2]);

  vec4[0].location.transformBy(transform_matrix);
  vec4[1].location.transformBy(transform_matrix);
  vec4[2].location.transformBy(transform_matrix);

}

float VertexShader::calculateLightIntencity(const RasterizerVertex<Vector4> vec4[3], const Vector4& light_dir)
{
 

    Vector4 v0 = { vec4[2].location.x - vec4[0].location.x, vec4[2].location.y - vec4[0].location.y, vec4[2].location.z - vec4[0].location.z, 0 };
    Vector4 v1 = { vec4[1].location.x - vec4[0].location.x, vec4[1].location.y - vec4[0].location.y, vec4[1].location.z - vec4[0].location.z, 0 };
    Vector4 nrm;
    nrm.x = v0.y * v1.z - v0.z * v1.y;
    nrm.y = v0.z * v1.x - v0.x * v1.z;
    nrm.z = v0.x * v1.y - v0.y * v1.x;

    float normf = std::sqrt(nrm.x * nrm.x + nrm.y * nrm.y + nrm.z * nrm.z);
    nrm = { nrm.x * 1 / normf , nrm.y * 1 / normf, nrm.z * 1 / normf, 0 };
    float intensity = nrm[0] * light_dir.x + nrm[1] * light_dir.y + nrm[2] * light_dir.z;
    return intensity;
  
}


