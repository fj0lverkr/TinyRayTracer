#include "graphics/Light.hpp"

Light::Light(const Vec3f &p, const float &i) : m_position(p), m_intensity(i) {}
Vec3f Light::getPosition() const { return m_position; }
float Light::getIntensity() const { return m_intensity; }