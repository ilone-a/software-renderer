#include "Matrix4.h"
#include "Math.h"
#include <iostream>


Matrix4::Matrix4()
{
    this->rc[0][0] = 0.0f;
    this->rc[0][1] = 0.0f;
    this->rc[0][2] = 0.0f;
    this->rc[0][3] = 0.0f;

    this->rc[1][0] = 0.0f;
    this->rc[1][1] = 0.0f;
    this->rc[1][2] = 0.0f;
    this->rc[1][3] = 0.0f;


    this->rc[2][0] = 0.0f;
    this->rc[2][1] = 0.0f;
    this->rc[2][2] = 0.0f;
    this->rc[2][3] = 0.0f;


    this->rc[3][0] = 0.0f;
    this->rc[3][1] = 0.0f;
    this->rc[3][2] = 0.0f;
    this->rc[3][3] = 0.0f;
}

Matrix4::Matrix4(float x00, float x01, float x02, float x03, float x10, float x11, float x12, float x13, float x20, float x21, float x22, float x23, float x30, float x31, float x32, float x33)
{
    this->rc[0][0] = x00;
    this->rc[0][1] = x01;
    this->rc[0][2] = x02;
    this->rc[0][3] = x03;

    this->rc[1][0] = x10;
    this->rc[1][1] = x11;
    this->rc[1][2] = x12;
    this->rc[1][3] = x13;


    this->rc[2][0] = x20;
    this->rc[2][1] = x21;
    this->rc[2][2] = x22;
    this->rc[2][3] = x23;


    this->rc[3][0] = x30;
    this->rc[3][1] = x31;
    this->rc[3][2] = x32;
    this->rc[3][3] = x33;
}

//multiply matrix
  Matrix4 Matrix4::multiply_m(const Matrix4& m) const
{
    Matrix4 result;
    for (int k = 0; k < 4; k++) {
        for (int j = 0; j < 4; j++) {
            for (int i = 0; i < 4; i++) {
                result.rc[k][j] += this->rc[k][i] * m.rc[i][j];
            }
        }
    }
    return result;
}




//transforms
 Matrix4 Matrix4::create_translation_matrix(float  x, float  y, float  z) 
{
     Matrix4 result;
     result.rc[0][0] = 1.0f;
     result.rc[1][1] = 1.0f;
     result.rc[2][2] = 1.0f;
     result.rc[3][3] = 1.0f;

     result.rc[0][3] = x;
     result.rc[1][3] = y;
     result.rc[2][3] = z;

    return result;

}

 Matrix4 Matrix4::create_rotation_x_matrix(float angle)
 {

     Matrix4 result;
     result.rc[0][0] = 1.0f;
     result.rc[1][1] = std::cos(angle);  result.rc[1][2] = -std::sin(angle);
     result.rc[2][1] = std::sin(angle);  result.rc[2][2] = std::cos(angle);
     result.rc[3][3] = 1.0f;

     return result;
 }


 Matrix4 Matrix4::create_rotation_y_matrix(float angle)
 {
     Matrix4 result;

     result.rc[0][0] = std::cos(angle);  result.rc[0][2] = std::sin(angle);
     result.rc[1][1] = 1.0f;
     result.rc[2][0] = -std::sin(angle);  result.rc[2][2] = std::cos(angle);
     result.rc[3][3] = 1.0f;

     return result;
 }


 Matrix4 Matrix4::create_scale_matrix(float x, float y, float z)
{

     Matrix4 result;
     result.rc[0][0] = x;
     result.rc[1][1] = y;
     result.rc[2][2] = z;
     result.rc[3][3] = 1.0f;


     return result;

}


//
  Matrix4 Matrix4::create_camera_screenspace(float width, float height)
{
    Matrix4 result;
    result.rc[0][0] = -width / 2; 
    result.rc[0][1] = 0.0f;
    result.rc[0][2] = 0.0f;
    result.rc[0][3] = width / 2;

    result.rc[1][0] = 0.0f;
    result.rc[1][1] = -height / 2; 
    result.rc[1][2] = 0.0f;
    result.rc[1][3] = height / 2;

    result.rc[2][0] = 0.0f;
    result.rc[2][1] = 0.0f;
    result.rc[2][2] = 1.0f;
    result.rc[2][3] = 0.0f;

    result.rc[2][0] = 0.0f;
    result.rc[2][1] = 0.0f;
    result.rc[3][2] = 0.0f;
    result.rc[3][3] = 1.0f;
    return result;
}

 Matrix4 Matrix4::create_camera_perspective(float fov, float aspect, float near, float far)
{
     //
     Matrix4 result;

     const float f = 1 / std::tan(fov / 2), 
         A = (far + near) / (near - far),
         B = 2 * far * near / (near - far);
   //  Matrix4 result;
     result.rc[0][0] = f/aspect;
     result.rc[1][1] = f;
     result.rc[2][2] = A; result.rc[2][3] = B;
     result.rc[3][2] = -1.0f;

     return result;

}


