#ifndef GRAPHICS_SPHERE_HPP
#define GRAPHICS_SPHERE_HPP
#include "math/Vec.hpp"
#include "material.hpp"

struct Sphere
{
private:
	Vec3f m_center;
	float m_radius;
	Material m_material;

public:
	Sphere(const Vec3f &c, const float &r, const Material &m);
	bool ray_intersect(const Vec3f &origin, const Vec3f &direction, float &t0) const;
	Vec3f getCenter() const;
	float getRadius() const;
	Material getMaterial() const;
};

#endif // GRAPHICS_SPHERE_HPP