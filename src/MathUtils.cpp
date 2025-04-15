#pragma once
#include "../include/MathUtils.h"
#include <cmath>


namespace MathUtils {

    Mat4 Mat4::identity() {
        Mat4 result = {};
        for (int i = 0; i < 4; ++i) {
            result.m[i][i] = 1.0f;
        }
        return result;
    }
    bool isPointInTriangle(const Vec3& p, const Vec3& v0, const Vec3& v1, const Vec3& v2) {
        float denom = (v1.y - v2.y) * (v0.x - v2.x) + (v2.x - v1.x) * (v0.y - v2.y);
        if (std::abs(denom) < 1e-5f) return false;

        float a = ((v1.y - v2.y) * (p.x - v2.x) + (v2.x - v1.x) * (p.y - v2.y)) / denom;
        float b = ((v2.y - v0.y) * (p.x - v2.x) + (v0.x - v2.x) * (p.y - v2.y)) / denom;
        float c = 1.0f - a - b;

        return a >= -0.01f && b >= -0.01f && c >= -0.01f;
    }
    Vec2::Vec2(float x, float y) : x(x), y(y) {}

    float Vec2::min() const {
        return (x < y) ? x : y;
    }

    float Vec2::max() const {
        return (x > y) ? x : y;
    }
    Vec3 lerp(const Vec3& a, const Vec3& b, float t)
    {
        return {
            a.x * (1.0f - t) + b.x * t,
            a.y * (1.0f - t) + b.y * t,
            a.z * (1.0f - t) + b.z * t
        };
    }
    Mat4 mat4_mul(const Mat4& a, const Mat4& b) {
        Mat4 result = {};
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result.m[i][j] = 0.0f;
                for (int k = 0; k < 4; ++k) {
                    result.m[i][j] += a.m[i][k] * b.m[k][j];
                }
            }
        }
        return result;
    }

    Vec3 mat4_mul_vec3(const Mat4& m, const Vec3& v) {
        return {
            m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3],
            m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3],
            m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3]
        };
    }

    Mat4 mat4_lerp(const Mat4& a, const Mat4& b, float t) {
        Mat4 result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result.m[i][j] = a.m[i][j] * (1.0f - t) + b.m[i][j] * t;
            }
        }
        return result;
    }

    Mat4 mat4_transpose(const Mat4& m) {
        Mat4 result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result.m[i][j] = m.m[j][i];
            }
        }
        return result;
    }

    void mat4_print(const Mat4& m) {
        for (int i = 0; i < 4; ++i) {
            printf("| %.3f %.3f %.3f %.3f |\n", m.m[i][0], m.m[i][1], m.m[i][2], m.m[i][3]);
        }
    }


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

}
