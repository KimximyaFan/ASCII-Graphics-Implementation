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
};