#pragma once

#include <cmath>
#include <cstdio>



#include <cmath>

    namespace MathUtils
{
    struct Vec3 {
        float x, y, z;
        Vec3() = default;  // Конструктор по умолчанию
        Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    };

    struct Triangle {
        Vec3 v0, v1, v2;
    };

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
    void mat4_print(const Mat4& m);
}
