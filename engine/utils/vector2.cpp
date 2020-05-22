#include "engine/utils/vector2.h"
#include <cmath>
namespace utils
{

Vector2::Vector2(const float x, const float y) :
        x(x), y(y)
{
    //ctor
}
Vector2::Vector2(const Vector2& pOriginal) :
        x(pOriginal.x), y(pOriginal.y)
{

}

Vector2::Vector2() :
        x(0), y(0) {

}

Vector2::~Vector2()
{
    //dtor
}

int Vector2::distance(const Vector2& v2)
{
    return std::sqrt(
            std::pow(v2.getX() - getX(), 2) + std::pow(v2.getY() - getY(), 2));
}
Vector2 Vector2::operator +(const Vector2& other) {
    return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator -(const Vector2& other) {
    return Vector2(x - other.x, y - other.y);
}

Vector2 Vector2::operator *(const Vector2& other) {
    return Vector2(x * other.x, y * other.y);
}

Vector2 Vector2::operator *(float value) {
    return Vector2(x * value, y * value);
}

Vector2 Vector2::normalize() {
    float value = std::sqrt(std::pow(x, 2) + std::pow(y, 2));

    return Vector2(x / value, y / value);
}

Vector2 Vector2::floor()
{
    return Vector2(std::floor(x),std::floor(y));
}
bool Vector2::operator==(const Vector2 &other)
{
    return x == other.x && y == other.y;
}

//Vector2 utils::operator +(const Vector2& orig, const Vector2& other) {
//	return Vector2(orig.getX() + other.getX(), orig.getY() + other.getY());
//}
//
//Vector2 utils::operator -(const Vector2& orig, const Vector2& other) {
//	return Vector2(orig.getX() - other.getX(), orig.getY() - other.getY());
//}
//
//Vector2 utils::operator *(const Vector2& orig, const Vector2& other) {
//	return Vector2(orig.getX() * other.getX(), orig.getY() * other.getY());
//}

Vector2 operator *(const Vector2& orig, float value) {
    return Vector2(orig.getX() * value, orig.getY() * value);
}
Vector2 lerp(Vector2 start, Vector2 end, float percentage) {
    return start + ((end - start) * percentage);
}


} // namespace utils


