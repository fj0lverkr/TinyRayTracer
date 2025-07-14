#include "math/Vec.hpp"

Vec3f::Vec3f() : x(0.f), y(0.f), z(0.f) {}
Vec3f::Vec3f(const float &x, const float &y, const float &z) : x(x), y(y), z(z) {}
float &Vec3f::operator[](const int i) { return i == 0 ? x : (1 == i ? y : z); }
const float &Vec3f::operator[](const int i) const { return i == 0 ? x : (1 == i ? y : z); }
Vec3f Vec3f::operator*(const float v) const { return {x * v, y * v, z * v}; }
float Vec3f::operator*(const Vec3f &v) const { return x * v.x + y * v.y + z * v.z; }
Vec3f Vec3f::operator+(const Vec3f &v) const { return {x + v.x, y + v.y, z + v.z}; }
Vec3f Vec3f::operator-(const Vec3f &v) const { return {x - v.x, y - v.y, z - v.z}; }
Vec3f Vec3f::operator-() const { return {-x, -y, -z}; }
float Vec3f::norm() const { return std::sqrt(x * x + y * y + z * z); }
Vec3f Vec3f::normalized() const { return (*this) * (1.f / norm()); }

Vec4f::Vec4f() : x(0.f), y(0.f), z(0.f), w(0.f) {}
Vec4f::Vec4f(const float &x, const float &y, const float &z, const float &w) : x(x), y(y), z(z), w(w) {}
float &Vec4f::operator[](const int i) { return i == 0 ? x : (1 == i ? y : (2 == i ? z : w)); }
const float &Vec4f::operator[](const int i) const { return i == 0 ? x : (1 == i ? y : (2 == i ? z : w)); }
Vec4f Vec4f::operator*(const float v) const { return {x * v, y * v, z * v, w * v}; }
float Vec4f::operator*(const Vec4f &v) const { return x * v.x + y * v.y + z * v.z + w * v.w; }
Vec4f Vec4f::operator+(const Vec4f &v) const { return {x + v.x, y + v.y, z + v.z, w + v.w}; }
Vec4f Vec4f::operator-(const Vec4f &v) const { return {x - v.x, y - v.y, z - v.z, w - v.w}; }
Vec4f Vec4f::operator-() const { return {-x, -y, -z, -w}; }
float Vec4f::norm() const { return std::sqrt(x * x + y * y + z * z + w * w); }
Vec4f Vec4f::normalized() const { return (*this) * (1.f / norm()); }

Vec3f cross(const Vec3f v1, const Vec3f v2)
{
	return {v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x};
}