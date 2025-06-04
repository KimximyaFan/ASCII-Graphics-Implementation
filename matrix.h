#pragma once

#include "vector.h"

class Mat4x4
{
public:

    float m[4][4] = {};

    Mat4x4 () {}

    Mat4x4 (float value);

    Mat4x4 (const float data[16]);

    Mat4x4 operator* (const Mat4x4& other) const ;
    Mat4x4& operator*=(const Mat4x4& other);

    Vec4 operator*(const Vec4& other) const;

    Mat4x4 operator+(const Mat4x4& other) const;
    Mat4x4& operator+=(const Mat4x4& other);

    Mat4x4 operator-(const Mat4x4& other) const;
    Mat4x4& operator-=(const Mat4x4& other);

    Mat4x4 operator*(float scalar) const;
    Mat4x4& operator*=(float scalar);

    Mat4x4 operator/(float scalar) const;
    Mat4x4& operator/=(float scalar);

    static Mat4x4 Identity();
    static Mat4x4 Zero();
    static Mat4x4 Translation(const Vec3& t);
    static Mat4x4 Scale(const Vec3& s);
    static Mat4x4 RotationX(float angle);
    static Mat4x4 RotationY(float angle);
    static Mat4x4 RotationZ(float angle);
    static Mat4x4 Perspective(float fov, float aspect, float near, float far);
    static Mat4x4 LookAt(const Vec3& eye, const Vec3& center, const Vec3& up);
    static Mat4x4 PerspectiveOffCenter(float x_min, float x_max, float y_min, float y_max, float z_near, float z_far);
};