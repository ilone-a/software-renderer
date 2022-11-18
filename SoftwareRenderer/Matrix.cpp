#include "Matrix.h"
#include <iostream>



void Matrix::setToTranslation(float x, float y, float z)
{
  this->rc[0][0] = 1.0f;
  this->rc[0][1] = 0.0f;
  this->rc[0][2] = 0.0f;
  this->rc[0][3] = x;

  this->rc[1][0] = 0.0f;
  this->rc[1][1] = 1.0f;
  this->rc[1][2] = 0.0f;
  this->rc[1][3] = y;

  this->rc[2][0] = 0.0f;
  this->rc[2][1] = 0.0f;
  this->rc[2][2] = 1.0f;
  this->rc[2][3] = z;

  this->rc[3][0] = 0.0f;
  this->rc[3][1] = 0.0f;
  this->rc[3][2] = 0.0f;
  this->rc[3][3] = 1.0f;

}

void Matrix::setToRotationX(float angle)
{

  this->rc[0][0] = 1.0f;
  this->rc[0][1] = 0.0f;
  this->rc[0][2] = 0.0f;
  this->rc[0][3] = 0.0f;

  this->rc[1][0] = 0.0f;
  this->rc[1][1] = std::cos(angle);
  this->rc[1][2] = -std::sin(angle);
  this->rc[1][3] = 0.0f;

  this->rc[2][0] = 0.0f;
  this->rc[2][1] = std::sin(angle);
  this->rc[2][2] = std::cos(angle);
  this->rc[2][3] = 0.0f;

  this->rc[3][0] = 0.0f;
  this->rc[3][1] = 0.0f;
  this->rc[3][2] = 0.0f;
  this->rc[3][3] = 1.0f;
}

void Matrix::setToScaling(float x, float y, float z)
{
  this->rc[0][0] = x;
  this->rc[0][1] = 0.0f;
  this->rc[0][2] = 0.0f;
  this->rc[0][3] = 0.0f;

  this->rc[1][0] = 0.0f;
  this->rc[1][1] = y;
  this->rc[1][2] = 0.0f;
  this->rc[1][3] = 0.0f;

  this->rc[2][0] = 0.0f;
  this->rc[2][1] = 0.0f;
  this->rc[2][2] = z;
  this->rc[2][3] = 0.0f;

  this->rc[3][0] = 0.0f;
  this->rc[3][1] = 0.0f;
  this->rc[3][2] = 0.0f;
  this->rc[3][3] = 1.0f;

}

void Matrix::setToScreenspace(float width, float height)
{
  this->rc[0][0] = width / 2;
  this->rc[0][1] = 0.0f;
  this->rc[0][2] = 0.0f;
  this->rc[0][3] = width / 2;

  this->rc[1][0] = 0.0f;
  this->rc[1][1] = -height / 2;
  this->rc[1][2] = 0.0f;
  this->rc[1][3] = height / 2;

  this->rc[2][0] = 0.0f;
  this->rc[2][1] = 0.0f;
  this->rc[2][2] = 1.0f;
  this->rc[2][3] = 0.0f;

  this->rc[3][0] = 0.0f;
  this->rc[3][1] = 0.0f;
  this->rc[3][2] = 0.0f;
  this->rc[3][3] = 1.0f;
}

void Matrix::setToPerspective(float fov, float aspect, float near, float far)
{
  const float f = 1 / std::tan(fov / 2),
    A = (far + near) / (near - far),
    B = 2 * far * near / (near - far);


  this->rc[0][0] = f / aspect;
  this->rc[0][1] = 0.0f;
  this->rc[0][2] = 0.0f;
  this->rc[0][3] = 0.0f;

  this->rc[1][0] = 0.0f;
  this->rc[1][1] = f;
  this->rc[1][2] = 0.0f;
  this->rc[1][3] = 0.0f;

  this->rc[2][0] = 0.0f;
  this->rc[2][1] = 0.0f;
  this->rc[2][2] = A;
  this->rc[2][3] = B;

  this->rc[3][0] = 0.0f;
  this->rc[3][1] = 0.0f;
  this->rc[3][2] = -1.0f;
  this->rc[3][3] = 0.0f;

}

void Matrix::setToIdentity()
{
  this->rc[0][0] = 1.0f;
  this->rc[0][1] = 0.0f;
  this->rc[0][2] = 0.0f;
  this->rc[0][3] = 0.0f;

  this->rc[1][0] = 0.0f;
  this->rc[1][1] = 1.0f;
  this->rc[1][2] = 0.0f;
  this->rc[1][3] = 0.0f;

  this->rc[2][0] = 0.0f;
  this->rc[2][1] = 0.0f;
  this->rc[2][2] = 1.0f;
  this->rc[2][3] = 0.0f;

  this->rc[3][0] = 0.0f;
  this->rc[3][1] = 0.0f;
  this->rc[3][2] = 0.0f;
  this->rc[3][3] = 1.0f;
}
