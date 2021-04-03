#ifndef UTILS_VECTOR2_H
#define UTILS_VECTOR2_H

namespace utils {

bool areSame(float a, float b);

class Vector2 {
public:
    /** Default constructor */
    Vector2(const float x, const float y);
    Vector2();
    /** copy constructor */
    Vector2(const Vector2& pOriginal);
    /** Default destructor */
    virtual ~Vector2();
    float getX() const
    {
        return x;
    }
    float getY() const
    {
        return y;
    }
    float distance(const Vector2& v2);
    Vector2 operator+(const Vector2& other);
    Vector2 operator-(const Vector2& other);
    Vector2 operator*(const Vector2& other);
    Vector2 operator*(float value);
    bool operator==(const Vector2& other);
    bool operator!=(const Vector2& other);

    Vector2 normalize();
    Vector2 floor();

protected:
private:
    float x, y;
};
//Vector2 operator+(const Vector2 &orig, const Vector2 &other);
//Vector2 operator-(const Vector2 &orig, const Vector2 &other);
//Vector2 operator*(const Vector2 &orig, const Vector2 &other);
Vector2 operator*(const Vector2& orig, float value);
Vector2 lerp(Vector2 start, Vector2 end, float percentage);
} // namespace utils

#endif // UTILS_VECTOR2_H
