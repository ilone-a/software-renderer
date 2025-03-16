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

    Vec2::Vec2(float x, float y) : x(x), y(y) {}

    float Vec2::min() const {
        return (x < y) ? x : y;
    }

    float Vec2::max() const {
        return (x > y) ? x : y;
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
}
