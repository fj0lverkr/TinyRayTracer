#ifndef GRAPHICS_MATERIAL_HPP
#define GRAPHICS_MATERIAL_HPP
#include "math/Vec.hpp"

struct Material
{
private:
	Vec4f m_albedo;
	Vec3f m_diffuse_color;
	float m_specular_exponent;
	float m_refractive_index;

public:
	Material();
	Material(const Vec4f &a, const Vec3f &color, const float &spec, const float &refr);
	Vec4f getAlbedo() const;
	Vec3f getDiffuseColor() const;
	float getSpecularExponent() const;
	float getRefractiveIndex() const;
	void setDiffuseColor(const Vec3f &color);
};

#endif // GRAPHICS_MATERIAL_HPP