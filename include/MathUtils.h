#pragma once

#include <cmath>
#include <cstdio>
#include <cstring>


    namespace MathUtils
{
        enum DefaultConstruct { kEmpty };
  
        class Matrix
        {
            static const Matrix kIdentity;
        public:
            float rc[4][4]; //get row get column
            Matrix() { this->setToIdentity(); }
            Matrix(DefaultConstruct) {};

            void setToIdentity();

            void setToTranslation(float x, float y, float z);
            void setToRotationX(float angle);
            void setToScaling(float x, float y, float z);

            void setToScreenspace(float width, float height);
            void setToPerspective(float fov, float aspect, float near, float far);
            Matrix operator *(const Matrix& other) const {
                Matrix result(kEmpty);
                memset(result.rc, 0, sizeof(result.rc));
                for (int i = 0; i < 4; ++i)
                    for (int j = 0; j < 4; ++j)
                        for (int k = 0; k < 4; ++k)
                            result.rc[i][j] += rc[i][k] * other.rc[k][j];
                return result;
            }



            static Matrix translation(float x, float y, float z)
            {
                Matrix m(DefaultConstruct::kEmpty);
                m.setToTranslation(x, y, z); return m;
            };

            static Matrix rotation(float angle)
            {
                Matrix m(DefaultConstruct::kEmpty);
                m.setToRotationX(angle); return m;
            };
            static Matrix scaling(float x, float y, float z)
            {
                Matrix m(DefaultConstruct::kEmpty);
                m.setToScaling(x, y, z); return m;
            };

            static Matrix screenspace(float width, float height)
            {
                Matrix m(DefaultConstruct::kEmpty);
                m.setToScreenspace(width, height); return m;
            };

            static Matrix perspective(float fov, float aspect, float near, float far)
            {
                Matrix m(DefaultConstruct::kEmpty);
                m.setToPerspective(fov, aspect, near, far); return m;
            };

            static Matrix& mtxMultiply(Matrix& r, const Matrix& a, const Matrix& b)
            {
                if ((&r != &a) && (&r != &b))
                    return mtxMultiplyNoOverlap(r, a, b);
                Matrix tmp(kEmpty);
                mtxMultiplyNoOverlap(tmp, a, b);
                return (r = tmp);
            }

            static Matrix& mtxMultiplyNoOverlap(Matrix& r, const Matrix& a, const Matrix& b)
            {
                for (int k = 0; k < 4; k++) {
                    for (int j = 0; j < 4; j++) {
                        float s = 0;
                        for (int i = 0; i < 4; i++) {
                            s += a.rc[k][i] * b.rc[i][j];
                        }
                        r.rc[k][j] = s;
                    }
                }
                return r;
            }

        };
        //

        struct Vector4
        {
            float x, y, z, w;
            float& operator[] (int i) { return (&x)[i]; }

            Vector4()
            {
                x = 0.0f;
                y = 0.0f;
                z = 0.0f;
                w = 0.0f;
            }

            Vector4(float x0, float y0, float z0, float w0)
            {
                x = x0;
                y = y0;
                z = z0;
                w = w0;
            }

            void transformBy(const Matrix& m)
            {
                *this = transform2(m, *this);
            };

            Vector4 transform2(const Matrix& m, const Vector4& a) const
            {
                //*this = M * (*this);
                Vector4 result;
                for (int i = 0; i < 4; i++)
                {
                    float s = m.rc[i][0] * a.x + m.rc[i][1] * a.y +
                        m.rc[i][2] * a.z + m.rc[i][3] * a.w;
                    result[i] = s;
                };
                return result;
            }

            void operator /= (float value)
            {
                x = x / value;
                y = y / value;
                z = z / value,
                    w = w / value;
            };

            Vector4 operator *(const MathUtils::Matrix& mat) const {
                Vector4 r;
                r.x = x * mat.rc[0][0] + y * mat.rc[0][1] + z * mat.rc[0][2] + w * mat.rc[0][3];
                r.y = x * mat.rc[1][0] + y * mat.rc[1][1] + z * mat.rc[1][2] + w * mat.rc[1][3];
                r.z = x * mat.rc[2][0] + y * mat.rc[2][1] + z * mat.rc[2][2] + w * mat.rc[2][3];
                r.w = x * mat.rc[3][0] + y * mat.rc[3][1] + z * mat.rc[3][2] + w * mat.rc[3][3];
                return r;
            }

        };////


        struct Point3d
        {
            float x, y, z;
            float& operator[] (int i) { return (&x)[i]; }

            Point3d()
            {
                x = 0.0f; y = 0.0f; z = 0.0f;
            }
            Point3d(float x0, float y0, float z0)
            {
                x = x0; y = y0; z = z0;
            }

            static void pntTransform(Point3d& dst, const Matrix& m, const Point3d& src)
            {
                float divider = m.rc[3][0] * src.x + m.rc[3][1] * src.y + m.rc[3][2] * src.z + m.rc[3][3];

                float x = (m.rc[0][0] * dst.x + m.rc[0][1] * dst.y + m.rc[0][2] * dst.z + m.rc[0][3]) / divider;
                float y = (m.rc[1][0] * dst.x + m.rc[1][1] * dst.y + m.rc[1][2] * dst.z + m.rc[1][3]) / divider;
                float z = (m.rc[2][0] * dst.x + m.rc[2][1] * dst.y + m.rc[2][2] * dst.z + m.rc[2][3]) / divider;

                dst.x = x; dst.y = y; dst.z = z;
            }
        };

    struct Vec3 {
        float x, y, z;
        Vec3() = default;  // 
        Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    };

    struct Triangle {
        Vec3 v0, v1, v2;
    };
    bool isPointInTriangle(const Vec3& p, const Vec3& v0, const Vec3& v1, const Vec3& v2);
    struct Mat4 {
        float m[4][4];

        static Mat4 identity();
    };

    struct Vec2 {
        float x, y;
        Vec2(float x = 0.0f, float y = 0.0f);
        float min() const;
        float max() const;
    };
    Vec3 lerp(const Vec3& a, const Vec3& b, float t); 
    Mat4 mat4_mul(const Mat4& a, const Mat4& b);
    Vec3 mat4_mul_vec3(const Mat4& m, const Vec3& v);
    Mat4 mat4_transpose(const Mat4& m);
    Mat4 mat4_lerp(const Mat4& a, const Mat4& b, float t);
    
}
