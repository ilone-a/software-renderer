#include "Vector4.h"

//xyzw vector


Vector4::Vector4()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 1.0f;
}

Vector4::Vector4(float x_coord, float y_coord, float z_coord, float w)
{
	x = x_coord;
	y = y_coord;
	z = z_coord;
	this->w = w;

}



Vector4 Vector4::multiply_v(const Matrix4& matrix)
{
    Vector4 result;
    float vector_tmp[4] = { this->x, this->y, this->z, this->w };

    float result_tmp[4] = { 
        matrix.rc[0][0]*vector_tmp[0]+ matrix.rc[0][1] * vector_tmp[1]+ matrix.rc[0][2] * vector_tmp[2] + matrix.rc[0][3] * vector_tmp[3],
         matrix.rc[1][0] * vector_tmp[0] + matrix.rc[1][1] * vector_tmp[1] + matrix.rc[1][2] * vector_tmp[2] + matrix.rc[1][3] * vector_tmp[3],
        matrix.rc[2][0] * vector_tmp[0] + matrix.rc[2][1] * vector_tmp[1] + matrix.rc[2][2] * vector_tmp[2] + matrix.rc[2][3] * vector_tmp[3],
         matrix.rc[3][0] * vector_tmp[0] + matrix.rc[3][1] * vector_tmp[1] + matrix.rc[3][2] * vector_tmp[2] + matrix.rc[3][3] * vector_tmp[3] };
 
    result.x = result_tmp[0];
    result.y = result_tmp[1];
    result.z = result_tmp[2];
    result.w = result_tmp[3];

	return result;

}

Vector4 Vector4::multiply_ñ(float f)
{
    return Vector4(this->x *f, this->y *f, this->z *f, this->w *f);
}

Vector4 Vector4::divide_v(float value)
{
   
    return Vector4{ this->x / value, this->y / value, this->z / value, this->w / value };
}
