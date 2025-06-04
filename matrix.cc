#include "matrix.h"

Mat4x4::Mat4x4() {}

Mat4x4::Mat4x4(float value) 
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m[i][j] = value;
}

Mat4x4::Mat4x4(const float data[16]) 
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m[i][j] = data[i * 4 + j];
}

Mat4x4 Mat4x4::operator*(const Mat4x4& other) const 
{
    Mat4x4 result;

    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            for (int k = 0; k < 4; k++)
                result.m[row][col] += m[row][k] * other.m[k][col];

    return result;
}

Mat4x4& Mat4x4::operator*=(const Mat4x4& other) 
{
    Mat4x4 result;

    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            for (int k = 0; k < 4; k++)
                result.m[row][col] += m[row][k] * other.m[k][col];

    *this = result;
    return *this;
}

Vec4 Mat4x4::operator*(const Vec4& other) const 
{
    Vec4 result;

    result.x = m[0][0]*other.x + m[0][1]*other.y + m[0][2]*other.z + m[0][3]*other.w;
    result.y = m[1][0]*other.x + m[1][1]*other.y + m[1][2]*other.z + m[1][3]*other.w;
    result.z = m[2][0]*other.x + m[2][1]*other.y + m[2][2]*other.z + m[2][3]*other.w;
    result.w = m[3][0]*other.x + m[3][1]*other.y + m[3][2]*other.z + m[3][3]*other.w;

    return result;
}

Mat4x4 Mat4x4::operator+(const Mat4x4& other) const 
{
    Mat4x4 result;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            result.m[i][j] = m[i][j] + other.m[i][j];

    return result;
}

Mat4x4& Mat4x4::operator+=(const Mat4x4& other) 
{
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            m[i][j] += other.m[i][j];

    return *this;
}

Mat4x4 Mat4x4::operator-(const Mat4x4& other) const 
{
    Mat4x4 result;

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            result.m[i][j] = m[i][j] - other.m[i][j];

    return result;
}

Mat4x4& Mat4x4::operator-=(const Mat4x4& other) 
{
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            m[i][j] -= other.m[i][j];

    return *this;
}

Mat4x4 Mat4x4::operator*(float scalar) const 
{
    Mat4x4 result;

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            result.m[i][j] = m[i][j] * scalar;

    return result;
}

Mat4x4& Mat4x4::operator*=(float scalar) 
{
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            m[i][j] *= scalar;

    return *this;
}

Mat4x4 Mat4x4::operator/(float scalar) const 
{
    Mat4x4 result;

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            result.m[i][j] = m[i][j] / scalar;

    return result;
}

Mat4x4& Mat4x4::operator/=(float scalar) 
{
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            m[i][j] /= scalar;

    return *this;
}

Mat4x4 Mat4x4::Identity()
{
    Mat4x4 mat;

    for (int i = 0; i < 4; ++i)
        mat.m[i][i] = 1.0f;

    return mat;
}

Mat4x4 Mat4x4::Zero()
{
    return Mat4x4();
}

/*
    1  0  0  x
    0  1  0  y
    0  0  1  z
    0  0  0  1
*/
Mat4x4 Mat4x4::Translation(const Vec3& t)
{
    Mat4x4 mat = Identity();
    mat.m[0][3] = t.x;
    mat.m[1][3] = t.y;
    mat.m[2][3] = t.z;
    return mat;
}

/*
    x  0  0  0
    0  y  0  0
    0  0  z  0
    0  0  0  1
*/
Mat4x4 Mat4x4::Scale(const Vec3& s)
{
    Mat4x4 mat = Identity();
    mat.m[0][0] = s.x;
    mat.m[1][1] = s.y;
    mat.m[2][2] = s.z;
    return mat;
}

/*
    1  0  0  0
    0  c -s  0
    0  s  c  0
    0  0  0  1
*/
Mat4x4 Mat4x4::RotationX(float angle)
{
    Mat4x4 mat = Identity();
    float c = std::cos(angle), s = std::sin(angle);
    mat.m[1][1] = c;
    mat.m[1][2] = -s;
    mat.m[2][1] = s;
    mat.m[2][2] = c;
    return mat;
}

/*
     c  0  s  0
     0  1  0  0
    -s  0  c  0
     0  0  0  1
*/
Mat4x4 Mat4x4::RotationY(float angle) {
    Mat4x4 mat = Identity();
    float c = std::cos(angle), s = std::sin(angle);
    mat.m[0][0] = c;
    mat.m[0][2] = s;
    mat.m[2][0] = -s;
    mat.m[2][2] = c;
    return mat;
}

/*
    c -s  0  0
    s  c  0  0
    0  0  1  0
    0  0  0  1
*/
Mat4x4 Mat4x4::RotationZ(float angle) {
    Mat4x4 mat = Identity();
    float c = std::cos(angle), s = std::sin(angle);
    mat.m[0][0] = c;
    mat.m[0][1] = -s;
    mat.m[1][0] = s;
    mat.m[1][1] = c;
    return mat;
}