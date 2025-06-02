#include <cmath>

class Vec3
{
public:
    
    float x, y, z;

    Vec3 (float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

    Vec3 operator+ (const Vec3& other) const
    {
        return Vec3( x + other.x, y + other.y, z + other.z );
    }

    Vec3& operator+= (const Vec3& other)
    {
        x += other.x; y += other.y; z += other.z;
        return *this;
    }

    Vec3 operator- (const Vec3& other) const
    {
        return Vec3( x - other.x, y - other.y, z - other.z );
    }

    Vec3& operator-= (const Vec3& other)
    {
        x += other.x; y += other.y; z += other.z;
        return *this;
    }

    Vec3 operator* (float scalar) const
    {
        return Vec3(x * scalar, y * scalar);
    }

    Vec3& operator*= (float scalar)
    {
        x *= scalar; y*= scalar; z*= scalar;
        return *this;
    }

    Vec3 operator/ (float scalar) const
    {
        return Vec3(x / scalar, y / scalar);
    }

    Vec3& operator/= (float scalar)
    {
        x /= scalar; y /= scalar; z /= scalar;
        return *this;
    }

    bool operator== (const Vec3& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    float Dot (const Vec3& other) const
    {
        return x*other.x + y*other.y + z*other.z;
    }

    Vec3 Cross (const Vec3& other) const
    {
        return Vec3(y*other.z - z*other.y, z*other.x - x*other.z, x*other.y - y*other.x);
    } 

    float Length () const
    {
        return std::sqrt( x*x + y*y + z*z );
    }

    Vec3 Normalize ()
    {
        float len = Length();
        return len != 0 ? *this/len : Vec3(0, 0, 0);
    }
};

class Vec4
{
public:
    float x, y, z, w;

    Vec4 (float x = 0, float y = 0, float z = 0, float w = 0)
        : x(x), y(y), z(z), w(w) {}

    Vec4 operator+ (const Vec4& other) const
    {
        return Vec4(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    Vec4& operator+= (const Vec4& other)
    {
        x += other.x; y += other.y; z += other.z; w += other.w;
        return *this;
    }

    Vec4 operator- (const Vec4& other) const
    {
        return Vec4(x - other.x, y - other.y, z - other.z, w - other.w);
    }

    Vec4& operator-= (const Vec4& other)
    {
        x -= other.x; y -= other.y; z -= other.z; w -= other.w;
        return *this;
    }

    Vec4 operator* (float scalar) const
    {
        return Vec4(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    Vec4& operator*= (float scalar)
    {
        x *= scalar; y *= scalar; z *= scalar; w *= scalar;
        return *this;
    }

    Vec4 operator/ (float scalar) const
    {
        return Vec4(x / scalar, y / scalar, z / scalar, w / scalar);
    }

    Vec4& operator/= (float scalar)
    {
        x /= scalar; y /= scalar; z /= scalar; w /= scalar;
        return *this;
    }

    bool operator== (const Vec4& other) const
    {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }

    float Dot (const Vec4& other) const
    {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    float Length () const
    {
        return std::sqrt(x*x + y*y + z*z + w*w);
    }

    Vec4 Normalize ()
    {
        float len = Length();
        return len == 0 ? Vec4(0, 0, 0, 0) : *this / len;
    }
};
