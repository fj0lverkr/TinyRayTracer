#ifndef MATH_VEC_HPP
#define MATH_VEC_HPP
#include <cmath>

struct Vec3f
{
public:
	Vec3f();
	Vec3f(const float &x, const float &y, const float &z);
	float x, y, z;
	float &operator[](const int i);
	const float &operator[](const int i) const;
	Vec3f operator*(const float v) const;
	float operator*(const Vec3f &v) const;
	Vec3f operator+(const Vec3f &v) const;
	Vec3f operator-(const Vec3f &v) const;
	Vec3f operator-() const;
	float norm() const;
	Vec3f normalized() const;
};

struct Vec4f
{
public:
	Vec4f();
	Vec4f(const float &x, const float &y, const float &z, const float &w);
	float x, y, z, w;
	float &operator[](const int i);
	const float &operator[](const int i) const;
	Vec4f operator*(const float v) const;
	float operator*(const Vec4f &v) const;
	Vec4f operator+(const Vec4f &v) const;
	Vec4f operator-(const Vec4f &v) const;
	Vec4f operator-() const;
	float norm() const;
	Vec4f normalized() const;
};

#endif // MATH_VEC_HPP