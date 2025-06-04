#pragma once

#include <cmath>

class Vec3
{
public:
    float x, y, z;

    Vec3(float x = 0, float y = 0, float z = 0);

    Vec3 operator+(const Vec3& other) const;
    Vec3& operator+=(const Vec3& other);

    Vec3 operator-(const Vec3& other) const;
    Vec3& operator-=(const Vec3& other);

    Vec3 operator*(float scalar) const;
    Vec3& operator*=(float scalar);

    Vec3 operator/(float scalar) const;
    Vec3& operator/=(float scalar);

    bool operator==(const Vec3& other) const;

    float Dot(const Vec3& other) const;
    Vec3 Cross(const Vec3& other) const;

    float Length() const;
    Vec3 Normalize();
};

class Vec4
{
public:
    float x, y, z, w;

    Vec4(float x = 0, float y = 0, float z = 0, float w = 0);

    Vec4 operator+(const Vec4& other) const;
    Vec4& operator+=(const Vec4& other);

    Vec4 operator-(const Vec4& other) const;
    Vec4& operator-=(const Vec4& other);

    Vec4 operator*(float scalar) const;
    Vec4& operator*=(float scalar);

    Vec4 operator/(float scalar) const;
    Vec4& operator/=(float scalar);

    bool operator==(const Vec4& other) const;

    float Dot(const Vec4& other) const;
    float Length() const;
    Vec4 Normalize();
};
