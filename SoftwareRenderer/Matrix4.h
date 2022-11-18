#pragma once
class Matrix4
    // 4x4
{
public:

    Matrix4();
    Matrix4(float x00, float x01, float x02, float x03,
        float x10, float x11, float x12, float x13,
        float x20, float x21, float x22, float x23,
        float x30, float x31, float x32, float x33);

    float rc[4][4]; //get row get column

    //Mworld = scale*rotation*location

 static   Matrix4 create_translation_matrix(float x, float y, float z);
 static    Matrix4 create_rotation_x_matrix(float angle);
 static   Matrix4 create_rotation_y_matrix(float angle);
 static   Matrix4 create_scale_matrix(float x, float y, float z);

  static   Matrix4 create_camera_screenspace(float width, float height);
  static    Matrix4 create_camera_perspective(float fov, float aspect, float near, float far);

    Matrix4 multiply_m(const Matrix4& result) const;





};

