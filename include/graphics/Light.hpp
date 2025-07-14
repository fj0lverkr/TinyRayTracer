#ifndef GRAPHICS_LIGHT_HPP
#define GRAPHICS_LIGHT_HPP
#include "math/Vec.hpp"

struct Light
{
private:
	Vec3f m_position{};
	float m_intensity{};

public:
	Light(const Vec3f &p, const float &i);
	Vec3f getPosition() const;
	float getIntensity() const;
};

#endif // GRAPHICS_LIGHT_HPP