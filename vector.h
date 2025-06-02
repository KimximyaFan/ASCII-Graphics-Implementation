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
};