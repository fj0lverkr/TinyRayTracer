#include "graphics/Material.hpp"

Material::Material() : m_albedo(1.f, 0.f, 0.f, 0.f), m_diffuse_color(), m_specular_exponent(), m_refractive_index(1.f) {}
Material::Material(const Vec4f &a, const Vec3f &color, const float &spec, const float &refr) : m_albedo(a), m_diffuse_color(color), m_specular_exponent(spec), m_refractive_index(refr) {}
Vec4f Material::getAlbedo() const { return m_albedo; }
Vec3f Material::getDiffuseColor() const { return m_diffuse_color; }
float Material::getSpecularExponent() const { return m_specular_exponent; }
float Material::getRefractiveIndex() const { return m_refractive_index; }
void Material::setDiffuseColor(const Vec3f &color)
{
	m_diffuse_color = color;
}