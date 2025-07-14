#include "graphics/Sphere.hpp"

Sphere::Sphere(const Vec3f &c, const float &r, const Material &m) : m_center(c), m_radius(r), m_material(m) {}

bool Sphere::ray_intersect(const Vec3f &origin, const Vec3f &direction, float &t0) const
{
	Vec3f L = m_center - origin;
	float tca = L * direction;
	float d2 = L * L - tca * tca;
	if (d2 > m_radius * m_radius)
		return false;
	float thc = sqrtf(m_radius * m_radius - d2);
	t0 = tca - thc;
	float t1 = tca + thc;
	if (t0 < 0)
		t0 = t1;
	if (t0 < 0)
		return false;
	return true;
}

Vec3f Sphere::getCenter() const { return m_center; }
float Sphere::getRadius() const { return m_radius; }
Material Sphere::getMaterial() const { return m_material; }