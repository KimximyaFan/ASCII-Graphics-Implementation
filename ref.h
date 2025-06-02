class Vector2 {
public:
    float x, y;

    Vector2(float x = 0, float y = 0) : x(x), y(y) {}

    // 덧셈
    Vector2 operator+(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }

    // 뺄셈
    Vector2 operator-(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }

    // 스칼라 곱
    Vector2 operator*(float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }

    // 스칼라 나눗셈
    Vector2 operator/(float scalar) const {
        return Vector2(x / scalar, y / scalar);
    }

    // 비교
    bool operator==(const Vector2& other) const {
        return x == other.x && y == other.y;
    }

    // 내적
    float dot(const Vector2& other) const {
        return x * other.x + y * other.y;
    }

    // 크기
    float length() const {
        return std::sqrt(x * x + y * y);
    }

    // 정규화
    Vector2 normalized() const {
        float len = length();
        return len != 0 ? *this / len : Vector2(0, 0);
    }
};