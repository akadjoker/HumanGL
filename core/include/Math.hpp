
#pragma once
#include <cmath>
#include <vector>

const unsigned int MaxUInt32 = 0xFFFFFFFF;
const int MinInt32 = 0x80000000;
const int MaxInt32 = 0x7FFFFFFF;
const float Maxloat = 3.402823466e+38F;
const float MinPosFloat = 1.175494351e-38F;
const float MATH_FLOAT_SMALL = 1.0e-37f;
const float MATH_TOLERANCE = 2e-37f;
const float Pi = 3.141592654f;
const float TwoPi = 6.283185307f;
const float PiHalf = 1.570796327f;

const float dtor = 0.0174532925199432957692369076848861f;
const float rtod = 1 / dtor;

const float Epsilon = 0.000001f;
const float ZeroEpsilon = 32.0f * MinPosFloat; // Very small epsilon for checking against 0.0f

const float M_INFINITY = 1.0e30f;

template <typename T>
T Min(const T &a, const T &b) { return (a < b) ? a : b; }

template <typename T>
T Max(const T &a, const T &b) { return (a > b) ? a : b; }

#define powi(base, exp) (int)powf((float)(base), (float)(exp))

#define ToRadians(x) (float)(((x) * Pi / 180.0f))
#define ToDegrees(x) (float)(((x) * 180.0f / Pi))

inline float Sin(float a) { return sin(a * Pi / 180); }
inline float Cos(float a) { return cos(a * Pi / 180); }
inline float Tan(float a) { return tan(a * Pi / 180); }
inline float SinRad(float a) { return sin(a); }
inline float CosRad(float a) { return cos(a); }
inline float TanRad(float a) { return tan(a); }
inline float ASin(float a) { return asin(a) * 180 / Pi; }
inline float ACos(float a) { return acos(a) * 180 / Pi; }
inline float ATan(float a) { return atan(a) * 180 / Pi; }
inline float ATan2(float y, float x) { return atan2(y, x) * 180 / Pi; }
inline float ASinRad(float a) { return asin(a); }
inline float ACosRad(float a) { return acos(a); }
inline float ATanRad(float a) { return atan(a); }
inline float ATan2Rad(float y, float x) { return atan2(y, x); }
inline int Floor(float a) { return (int)(floor(a)); }
inline int Ceil(float a) { return (int)(ceil(a)); }
inline int Trunc(float a)
{
	if (a > 0)
		return Floor(a);
	else
		return Ceil(a);
}
inline int Round(float a)
{
	if (a < 0)
		return (int)(ceil(a - 0.5f));
	else
		return (int)(floor(a + 0.5f));
}
inline float Sqrt(float a)
{
	if (a > 0)
		return sqrt(a);
	else
		return 0;
}
inline float Abs(float a)
{
	if (a < 0)
		a = -a;
	return a;
}
inline int Mod(int a, int b)
{
	if (b == 0)
		return 0;
	return a % b;
}
inline float FMod(float a, float b)
{
	if (b == 0)
		return 0;
	return fmod(a, b);
}
inline float Pow(float a, float b) { return pow(a, b); }
inline int Sign(float a)
{
	if (a < 0)
		return -1;
	else if (a > 0)
		return 1;
	else
		return 0;
}
inline float Min(float a, float b) { return a < b ? a : b; }
inline float Max(float a, float b) { return a > b ? a : b; }
inline float Clamp(float a, float min, float max)
{
	if (a < min)
		a = min;
	else if (a > max)
		a = max;
	return a;
}
inline int Clamp(int a, int min, int max)
{
	if (a < min)
		a = min;
	else if (a > max)
		a = max;
	return a;
}

inline bool isnotzero(const float a, const float tolerance = Epsilon)
{
	return fabsf(a) > tolerance;
}

inline bool isequal(const float a, const float b, const float tolerance = Epsilon)
{
	if (fabs(b - a) <= tolerance)
		return true;
	else
		return false;
}



struct Vec2
{

	float x, y;

	Vec2();
	Vec2(float x, float y);

	// Getters/Setters
	float getX() const;
	float getY() const;
	void setX(float x);
	void setY(float y);

	// Operations
	Vec2 operator+(const Vec2 &other) const;
	Vec2 operator-(const Vec2 &other) const;
	Vec2 operator*(float scalar) const;
	float dot(const Vec2 &other) const;
	float length() const;
	Vec2 normalize() const;
};

struct Vec3
{

	static constexpr float EPSILON = 1e-6f;

	float x, y, z;

	Vec3();
	Vec3(float x, float y, float z);

	// Getters/Setters
	float getX() const;
	float getY() const;
	float getZ() const;
	void setX(float x);
	void setY(float y);
	void setZ(float z);

	// Operations
	Vec3 operator+(const Vec3 &other) const;
	Vec3 operator-(const Vec3 &other) const;
	Vec3 operator*(float scalar) const;
	float dot(const Vec3 &other) const;
	Vec3 cross(const Vec3 &other) const;
	float length() const;
	Vec3 normalize() const;

	Vec3 operator-() const
	{ // negação
		return Vec3(-x, -y, -z);
	}

	Vec3 &operator+=(const Vec3 &other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vec3 &operator-=(const Vec3 &other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	Vec3 &operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	bool operator==(const Vec3 &other) const
	{
		return std::abs(x - other.x) < EPSILON &&
			   std::abs(y - other.y) < EPSILON &&
			   std::abs(z - other.z) < EPSILON;
	}
};

struct Quat
{

	float w, x, y, z;

	Quat();
	Quat(float w, float x, float y, float z);
	Quat(float angle, const Vec3 &axis); // Rotation constructor

	// Getters/Setters
	float getW() const;
	float getX() const;
	float getY() const;
	float getZ() const;

	// Operations
	Quat operator*(const Quat &other) const;
	Quat conjugate() const;
	Quat normalize() const;
	float length() const;
	Vec3 rotate(const Vec3 &v) const;
};

struct Mat4
{
	float m[16]; // Column-major order

	Mat4(); // Identity matrix
	Mat4(const float *values);

	// Access
	float &at(int row, int col);
	float at(int row, int col) const;
	const float *data() const;

	// Basic operations
	Mat4 operator*(const Mat4 &other) const;
	Vec3 transform(const Vec3 &v) const;

	// Create transformation matrices
	static Mat4 Identity();
	static Mat4 Translate(const Vec3 &v);
	static Mat4 Rotate(const Quat &q);
	static Mat4 Scale(const Vec3 &v);
	static Mat4 Perspective(float fov, float aspect, float near, float far);
	static Mat4 LookAt(const Vec3 &eye, const Vec3 &center, const Vec3 &up);
	static Mat4 Ortho(float left, float right, float bottom, float top, float near, float far);
};

template <typename T>
struct Rect
{

	T x;
	T y;
	T width;
	T height;

	Rect() : x(0), y(0), width(0), height(0) {}
	Rect(T x, T y, T width, T height) : x(x), y(y), width(width), height(height) {}
	Rect(const Rect &rect) : x(rect.x), y(rect.y), width(rect.width), height(rect.height) {}

	void Set(T x, T y, T width, T height)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}

	void Merge(const Rect &rect)
	{
		T right = x + width;
		T bottom = y + height;
		T rectRight = rect.x + rect.width;
		T rectBottom = rect.y + rect.height;
		x = Min(x, rect.x);
		y = Min(y, rect.y);
		right = Max(right, rectRight);
		bottom = Max(bottom, rectBottom);
		width = right - x;
		height = bottom - y;
	}

	void Merge(const Vec2 &point)
	{
		T right = x + width;
		T bottom = y + height;
		x = Min(x, point.x);
		y = Min(y, point.y);
		right = Max(right, point.x);
		bottom = Max(bottom, point.y);
		width = right - x;
		height = bottom - y;
	}

	void Clear()
	{
		x = 0;
		y = 0;
		width = 0;
		height = 0;
	}

	Rect &operator=(const Rect &rect)
	{
		if (this == &rect)
			return *this;
		x = rect.x;
		y = rect.y;
		width = rect.width;
		height = rect.height;
		return *this;
	}
};

template <typename T>
struct Size
{
	T width;
	T height;

	Size() : width(0), height(0) {}
	Size(T w, T h) : width(w), height(h) {}
	Size(const Size &size) : width(size.width), height(size.height) {}

	Size &operator=(const Size &size)
	{
		if (this == &size)
			return *this;
		width = size.width;
		height = size.height;
		return *this;
	}
};

typedef Rect<int> IntRect;
typedef Rect<float> FloatRect;
typedef Size<int> IntSize;
typedef Size<float> FloatSize;



struct Rectangle
{
    Rectangle()
    {
        x = 0;
        y = 0;
        width = 0;
        height = 0;
    }
    Rectangle(float _x, float _y, float _width, float _height)
    {
        x = _x;
        y = _y;
        width = _width;
        height = _height;
    }
    void set(float _x, float _y, float _width, float _height)
    {
        x = _x;
        y = _y;
        width = _width;
        height = _height;
    }
    void Clip(const Rectangle &rect)
    {
        float x1 = x;
        float y1 = y;
        float x2 = x + width;
        float y2 = y + height;
        float rx1 = rect.x;
        float ry1 = rect.y;
        float rx2 = rect.x + rect.width;
        float ry2 = rect.y + rect.height;
        if (x1 < rx1) x1 = rx1;
        if (y1 < ry1) y1 = ry1;
        if (x2 > rx2) x2 = rx2;
        if (y2 > ry2) y2 = ry2;
        x = x1;
        y = y1;
        width = x2 - x1;
        height = y2 - y1;
    }
    bool Contains(int px, int py)
    {
        return (px >= x && py >= y && px < x + width && py < y + height);
    }
    bool Contains(float px, float py)
    {
        return (px >= x && py >= y && px < x + width && py < y + height);
    }

    bool Contains(const Vec2 &point)
    {
        return (point.x >= x && point.y >= y && point.x < x + width && point.y < y + height);
    }


    float x, y, width, height;
};


class MatrixStack
{
private:
	std::vector<Mat4> stack;

public:
	MatrixStack();

	void push();
	void pop();
	const Mat4 &top() const;
	void multiply(const Mat4 &m);

	void identity();

	void translate(const Vec3 &v);
	void translate(float x, float y, float z);

	void rotate(const Quat &q);
	void rotate(float angle, const Vec3 &axis);
	void rotateX(float a);
	void rotateY(float a);
	void rotateZ(float a);
	
	void scale(const Vec3 &v);
	void scale(float x, float y, float z);

	size_t size() const;
	void clear();
};