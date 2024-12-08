
#include "Math.hpp"
#include <cmath>
#include <cstring>

Vec2::Vec2() : x(0.0f), y(0.0f) {}
Vec2::Vec2(float x, float y) : x(x), y(y) {}

float Vec2::getX() const { return x; }
float Vec2::getY() const { return y; }
void Vec2::setX(float newX) { x = newX; }
void Vec2::setY(float newY) { y = newY; }

Vec2 Vec2::operator+(const Vec2 &other) const
{
    return Vec2(x + other.x, y + other.y);
}

Vec2 Vec2::operator-(const Vec2 &other) const
{
    return Vec2(x - other.x, y - other.y);
}

Vec2 Vec2::operator*(float scalar) const
{
    return Vec2(x * scalar, y * scalar);
}

float Vec2::dot(const Vec2 &other) const
{
    return x * other.x + y * other.y;
}

float Vec2::length() const
{
    return std::sqrt(x * x + y * y);
}

Vec2 Vec2::normalize() const
{
    float len = length();
    if (len > 0)
    {
        return Vec2(x / len, y / len);
    }
    return *this;
}

// Vec3

Vec3::Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

float Vec3::getX() const { return x; }
float Vec3::getY() const { return y; }
float Vec3::getZ() const { return z; }
void Vec3::setX(float newX) { x = newX; }
void Vec3::setY(float newY) { y = newY; }
void Vec3::setZ(float newZ) { z = newZ; }

Vec3 Vec3::operator+(const Vec3 &other) const
{
    return Vec3(x + other.x, y + other.y, z + other.z);
}

Vec3 Vec3::operator-(const Vec3 &other) const
{
    return Vec3(x - other.x, y - other.y, z - other.z);
}

Vec3 Vec3::operator*(float scalar) const
{
    return Vec3(x * scalar, y * scalar, z * scalar);
}

float Vec3::dot(const Vec3 &other) const
{
    return x * other.x + y * other.y + z * other.z;
}

Vec3 Vec3::cross(const Vec3 &other) const
{
    return Vec3(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x);
}

float Vec3::length() const
{
    return std::sqrt(x * x + y * y + z * z);
}

Vec3 Vec3::normalize() const
{
    float len = length();
    if (len > EPSILON)
    {
        float invLen = 1.0f / len;
        return Vec3(x * invLen, y * invLen, z * invLen);
    }
    return *this;
}

// Quat
Quat::Quat() : w(1.0f), x(0.0f), y(0.0f), z(0.0f) {}
Quat::Quat(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}

Quat::Quat(float angle, const Vec3 &axis)
{
    float halfAngle = angle * 0.5f;
    float sinHalfAngle = std::sin(halfAngle);
    Vec3 normalizedAxis = axis.normalize();

    w = std::cos(halfAngle);
    x = normalizedAxis.getX() * sinHalfAngle;
    y = normalizedAxis.getY() * sinHalfAngle;
    z = normalizedAxis.getZ() * sinHalfAngle;
}

float Quat::getW() const { return w; }
float Quat::getX() const { return x; }
float Quat::getY() const { return y; }
float Quat::getZ() const { return z; }

Quat Quat::operator*(const Quat &other) const
{
    return Quat(
        w * other.w - x * other.x - y * other.y - z * other.z,
        w * other.x + x * other.w + y * other.z - z * other.y,
        w * other.y - x * other.z + y * other.w + z * other.x,
        w * other.z + x * other.y - y * other.x + z * other.w);
}

Quat Quat::conjugate() const
{
    return Quat(w, -x, -y, -z);
}

float Quat::length() const
{
    return std::sqrt(w * w + x * x + y * y + z * z);
}

Quat Quat::normalize() const
{
    float len = length();
    if (len > 0)
    {
        float invLen = 1.0f / len;
        return Quat(w * invLen, x * invLen, y * invLen, z * invLen);
    }
    return *this;
}

Vec3 Quat::rotate(const Vec3 &v) const
{
    Quat p(0, v.getX(), v.getY(), v.getZ());
    Quat rotated = *this * p * conjugate();
    return Vec3(rotated.x, rotated.y, rotated.z);
}

// Mat4
Mat4::Mat4()
{
    // Initialize as identity matrix
    std::memset(m, 0, sizeof(m));
    m[0] = m[5] = m[10] = m[15] = 1.0f;
}

Mat4::Mat4(const float *values)
{
    std::memcpy(m, values, sizeof(m));
}

float &Mat4::at(int row, int col)
{
    return m[col * 4 + row];
}

float Mat4::at(int row, int col) const
{
    return m[col * 4 + row];
}

const float *Mat4::data() const
{
    return m;
}

Mat4 Mat4::operator*(const Mat4 &other) const
{
    Mat4 result;
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            result.at(row, col) = 0;
            for (int k = 0; k < 4; k++)
            {
                result.at(row, col) += at(row, k) * other.at(k, col);
            }
        }
    }
    return result;
}

Vec3 Mat4::transform(const Vec3 &v) const
{
    float w = m[3] * v.getX() + m[7] * v.getY() + m[11] * v.getZ() + m[15];
    const float EPSILON = 1e-6f;
    w = (std::abs(w) > EPSILON) ? 1.0f / w : 1.0f;
    return Vec3(
        (m[0] * v.getX() + m[4] * v.getY() + m[8] * v.getZ() + m[12]) * w,
        (m[1] * v.getX() + m[5] * v.getY() + m[9] * v.getZ() + m[13]) * w,
        (m[2] * v.getX() + m[6] * v.getY() + m[10] * v.getZ() + m[14]) * w);
}

Mat4 Mat4::Translate(const Vec3 &v)
{
    Mat4 result;
    result.at(0, 3) = v.getX();
    result.at(1, 3) = v.getY();
    result.at(2, 3) = v.getZ();
    return result;
}

Mat4 Mat4::Rotate(const Quat &q)
{
    Mat4 result;
    float xx = q.getX() * q.getX();
    float xy = q.getX() * q.getY();
    float xz = q.getX() * q.getZ();
    float xw = q.getX() * q.getW();
    float yy = q.getY() * q.getY();
    float yz = q.getY() * q.getZ();
    float yw = q.getY() * q.getW();
    float zz = q.getZ() * q.getZ();
    float zw = q.getZ() * q.getW();

    result.at(0, 0) = 1 - 2 * (yy + zz);
    result.at(0, 1) = 2 * (xy - zw);
    result.at(0, 2) = 2 * (xz + yw);
    result.at(1, 0) = 2 * (xy + zw);
    result.at(1, 1) = 1 - 2 * (xx + zz);
    result.at(1, 2) = 2 * (yz - xw);
    result.at(2, 0) = 2 * (xz - yw);
    result.at(2, 1) = 2 * (yz + xw);
    result.at(2, 2) = 1 - 2 * (xx + yy);

    return result;
}

Mat4 Mat4::Scale(const Vec3 &v)
{
    Mat4 result;
    result.at(0, 0) = v.getX();
    result.at(1, 1) = v.getY();
    result.at(2, 2) = v.getZ();
    return result;
}

Mat4 Mat4::Perspective(float fovRadians, float aspect, float near, float far)
{
    Mat4 result;
    float tanHalfFov = std::tan(fovRadians / 2.0f);

    result.at(0, 0) = 1.0f / (aspect * tanHalfFov);
    result.at(1, 1) = 1.0f / tanHalfFov;
    result.at(2, 2) = -(far + near) / (far - near);
    result.at(2, 3) = -(2.0f * far * near) / (far - near);
    result.at(3, 2) = -1.0f;
    result.at(3, 3) = 0.0f;

    return result;
}

Mat4 Mat4::LookAt(const Vec3 &eye, const Vec3 &center, const Vec3 &up)
{
    Vec3 f = (center - eye).normalize();
    Vec3 s = f.cross(up).normalize();
    Vec3 u = s.cross(f);

    Mat4 result;
    result.at(0, 0) = s.getX();
    result.at(0, 1) = s.getY();
    result.at(0, 2) = s.getZ();
    result.at(0, 3) = -s.dot(eye);

    result.at(1, 0) = u.getX();
    result.at(1, 1) = u.getY();
    result.at(1, 2) = u.getZ();
    result.at(1, 3) = -u.dot(eye);

    result.at(2, 0) = -f.getX();
    result.at(2, 1) = -f.getY();
    result.at(2, 2) = -f.getZ();
    result.at(2, 3) = f.dot(eye);

    result.at(3, 0) = 0.0f;
    result.at(3, 1) = 0.0f;
    result.at(3, 2) = 0.0f;
    result.at(3, 3) = 1.0f;

    return result;
}

Mat4 Mat4::Ortho(float left, float right, float bottom, float top, float near, float far)
{
    Mat4 mat;
    mat.m[0] = 2.0f / (right - left);
    mat.m[5] = 2.0f / (top - bottom);
    mat.m[10] = -2.0f / (far - near);
    mat.m[12] = -(right + left) / (right - left);
    mat.m[13] = -(top + bottom) / (top - bottom);
    mat.m[14] = -(far + near) / (far - near);
    mat.m[15] = 1.0f;
    return mat;
}
Mat4 Mat4::Identity()
{
    Mat4 mat;
    mat.m[0] = 1.0f;
    mat.m[5] = 1.0f;
    mat.m[10] = 1.0f;
    mat.m[15] = 1.0f;
    return mat;
}

MatrixStack::MatrixStack()
{
    stack.push_back(Mat4::Identity());
}

void MatrixStack::push()
{
    stack.push_back(stack.back());
}

void MatrixStack::pop()
{
    if (stack.size() > 1)
    { // Always keep at least one matrix
        stack.pop_back();
    }
}

// Get current matrix
const Mat4 &MatrixStack::top() const
{
    return stack.back();
}

// Multiply current matrix by another matrix
void MatrixStack::multiply(const Mat4 &m)
{
    stack.back() = stack.back() * m;
}

// Clear stack and reset to identity
void MatrixStack::identity()
{
    stack.clear();
    stack.push_back(Mat4::Identity());
}


void MatrixStack::translate(const Vec3 &v)
{
    multiply(Mat4::Translate(v));
}

void MatrixStack::translate(float x, float y, float z)
{
    translate(Vec3(x,y,z));
}

void MatrixStack::rotate(const Quat &q)
{
    multiply(Mat4::Rotate(q));
}

void MatrixStack::rotate(float angle, const Vec3 &axis)
{
    multiply(Mat4::Rotate(Quat(angle, axis)));
}


void MatrixStack::rotateX(float a)
{
    rotate(a,Vec3(1.0f,0.0f,0.0f));
}


void MatrixStack::rotateY(float a)
{
    rotate(a,Vec3(0.0f,1.0f,0.0f));

}
void MatrixStack::rotateZ(float a)
{
    rotate(a,Vec3(0.0f,0.0f,1.0f));

}

void MatrixStack::scale(const Vec3 &v)
{
    multiply(Mat4::Scale(v));
}
void MatrixStack::scale(float x, float y, float z)
{
    scale(Vec3(x,y,z));
}
	

size_t MatrixStack::size() const
{
    return stack.size();
}

void MatrixStack::clear()
{
    identity();
}
