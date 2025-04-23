#ifndef VECTOR2
#define VECTOR2
#include <cmath>

struct Vector2 {
    float x, y;
    friend void operator+=(Vector2& a, const Vector2& b) { a.x += b.x; a.y += b.y; }
    friend void operator-=(Vector2& a, const Vector2& b) { a.x -= b.x; a.y -= b.y; }
    friend void operator*=(Vector2& a, const Vector2& b) { a.x *= b.x; a.y *= b.y; }
    friend void operator/=(Vector2& a, const Vector2& b) { a.x /= b.x; a.y /= b.y; }
    
    friend void operator+=(Vector2& a, const float& b) { a.x += b; a.y += b; }
    friend void operator-=(Vector2& a, const float& b) { a.x -= b; a.y -= b; }
    friend void operator*=(Vector2& a, const float& b) { a.x *= b; a.y *= b; }
    friend void operator/=(Vector2& a, const float& b) { a.x /= b; a.y /= b; }
    
    friend Vector2 operator+(const Vector2& a, const Vector2& b) { return { a.x + b.x, a.y + b.y}; }
    friend Vector2 operator-(const Vector2& a, const Vector2& b) { return { a.x - b.x, a.y - b.y}; }
    friend Vector2 operator*(const Vector2& a, const Vector2& b) { return { a.x * b.x, a.y * b.y}; }
    friend Vector2 operator/(const Vector2& a, const Vector2& b) { return { a.x / b.x, a.y / b.y}; }

    friend Vector2 operator+(const Vector2& a, const float& b) { return { a.x + b, a.y + b}; }
    friend Vector2 operator-(const Vector2& a, const float& b) { return { a.x - b, a.y - b}; }
    friend Vector2 operator*(const Vector2& a, const float& b) { return { a.x * b, a.y * b}; }
    friend Vector2 operator/(const Vector2& a, const float& b) { return { a.x / b, a.y / b}; }
    
    friend Vector2 operator^(const Vector2& a, const float& b) { return { powf(a.x, b), powf(a.y, b)}; }
    static float mag(Vector2 a) { return std::sqrtf(a.x * a.x + a.y * a.y); }
    static Vector2 normal(Vector2 a) { return a / mag(a); }
    float mag() { return std::sqrtf(x * x + y * y); }
    Vector2 normal() { return *this / mag(); }
};
#endif
