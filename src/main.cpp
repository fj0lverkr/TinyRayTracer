#define _USE_MATH_DEFINES
#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "math/Vec.hpp"
#include "graphics/Light.hpp"
#include "graphics/Material.hpp"
#include "graphics/Sphere.hpp"

using namespace std;

const Vec3f BGCOLOR = Vec3f(0.2f, 0.7f, 0.8f);
const Vec3f GREYCOLOR = Vec3f(0.4f, 0.4f, 0.3f);
const Vec3f REDCOLOR = Vec3f(0.3f, 0.1f, 0.1f);
const Vec3f WHITECOLOR = Vec3f(1.0f, 1.0f, 1.0f);
const Vec3f REFRACTCOLOR = Vec3f(0.6f, 0.7f, 0.8f);
const Vec3f SPECULARALEDOCOLOR = WHITECOLOR;

constexpr int WIDTH = 1024;
constexpr int HEIGHT = 768;

const string ENVMAP = "res/envmap.jpg";

const vector<Vec3f> COLORS{BGCOLOR, GREYCOLOR, REDCOLOR, WHITECOLOR};

enum Colors
{
	BG,
	GREY,
	RED,
	WHITE
};

const Material IVORY(Vec4f(0.6f, 0.3f, 0.1f, 0.f), COLORS[Colors::GREY], 50.f, 1.f);
const Material GLASS(Vec4f(0.f, 0.5f, 0.1f, 0.8f), REFRACTCOLOR, 125.f, 1.5f);
const Material REDRUBBER(Vec4f(0.9f, 0.1f, 0.f, 0.f), COLORS[Colors::RED], 10.f, 1.f);
const Material MIRROR(Vec4f(0.f, 10.f, 0.8f, 0.f), COLORS[Colors::WHITE], 1425.f, 1.f);

int envmap_width, envmap_height;
vector<Vec3f> envmap;

bool loadEnvMap(const string &path)
{
	int n = -1;
	unsigned char *pixmap = stbi_load(path.c_str(), &envmap_width, &envmap_height, &n, 0);
	if (!pixmap || 3 != n)
	{
		cout << pixmap << endl;
		cerr << "Error: can not load the environment map in path " + path + "!" << endl;
		return false;
	}
	envmap = vector<Vec3f>(envmap_width * envmap_height);
	for (int j = envmap_height - 1; j >= 0; j--)
	{
		for (int i = 0; i < envmap_width; i++)
		{
			envmap[i + j * envmap_width] = Vec3f(pixmap[(i + j * envmap_width) * 3 + 0], pixmap[(i + j * envmap_width) * 3 + 1], pixmap[(i + j * envmap_width) * 3 + 2]) * (1 / 255.);
		}
	}
	stbi_image_free(pixmap);
	return true;
}

Vec3f reflect(const Vec3f &I, const Vec3f &N)
{
	return I - N * 2.f * (I * N);
}

Vec3f refract(const Vec3f &I, const Vec3f &N, const float &refractive_index)
{ // Snell's law
	float cosi = -std::max(-1.f, std::min(1.f, I * N));
	float etai = 1, etat = refractive_index;
	Vec3f n = N;
	if (cosi < 0)
	{ // if the ray is inside the object, swap the indices and invert the normal to get the correct result
		cosi = -cosi;
		std::swap(etai, etat);
		n = -N;
	}
	float eta = etai / etat;
	float k = 1 - eta * eta * (1 - cosi * cosi);
	return k < 0
			   ? Vec3f(0, 0, 0)
			   : I * eta + n * (eta * cosi - sqrtf(k));
}

bool scene_intersect(const Vec3f &orig, const Vec3f &dir, const std::vector<Sphere> &spheres, Vec3f &hit, Vec3f &N, Material &material)
{
	float spheres_dist = numeric_limits<float>::max();
	float checkerboard_dist = numeric_limits<float>::max();
	float envmap_dist = numeric_limits<float>::max();

	for (size_t i = 0; i < spheres.size(); i++)
	{
		float dist_i;
		if (spheres[i].ray_intersect(orig, dir, dist_i) && dist_i < spheres_dist)
		{
			spheres_dist = dist_i;
			hit = orig + dir * dist_i;
			N = (hit - spheres[i].getCenter()).normalized();
			material = spheres[i].getMaterial();
		}
	}

	if (fabs(dir.y) > 1e-3)
	{
		float d = -(orig.y + 4) / dir.y; // the checkerboard plane has equation y = -4
		Vec3f pt = orig + dir * d;
		if (d > 0 && fabs(pt.x) < 10 && pt.z < -10 && pt.z > -30 && d < spheres_dist)
		{
			checkerboard_dist = d;
			hit = pt;
			N = Vec3f(0, 1, 0);
			material.setDiffuseColor((int(.5 * hit.x + 1000) + int(.5 * hit.z)) & 1 ? Vec3f(1, 1, 1) : Vec3f(1, .7, .3));
			material.setDiffuseColor(material.getDiffuseColor() * .3);
		}
	}

	return min(spheres_dist, checkerboard_dist) < 1000;
}

Vec3f renderEnvMap(const Vec3f &dir)
{
	Vec3f d = dir.normalized();
	int x = max(0, min(envmap_width - 1, static_cast<int>((atan2(d.z, d.x) / (2 * M_PI) + .5) * envmap_width)));
	int y = max(0, min(envmap_height - 1, static_cast<int>(acos(d.y) / M_PI * envmap_height)));

	return envmap[(x + y * envmap_width)];
}

Vec3f cast_ray(const Vec3f &orig, const Vec3f &dir, const vector<Sphere> &spheres, const vector<Light> &lights, size_t depth = 0)
{
	Vec3f point, N;
	Material material;

	if (depth > 4 || !scene_intersect(orig, dir, spheres, point, N, material))
	{
		return renderEnvMap(dir);
	}

	Vec3f reflect_dir = reflect(dir, N).normalized();
	Vec3f refract_dir = refract(dir, N, material.getRefractiveIndex()).normalized();
	Vec3f reflect_orig = reflect_dir * N < 0 ? point - N * 1e-3 : point + N * 1e-3; // offset the original point to avoid occlusion by the object itself
	Vec3f refract_orig = refract_dir * N < 0 ? point - N * 1e-3 : point + N * 1e-3;
	Vec3f reflect_color = cast_ray(reflect_orig, reflect_dir, spheres, lights, depth + 1);
	Vec3f refract_color = cast_ray(refract_orig, refract_dir, spheres, lights, depth + 1);

	float diffuse_light_intensity = 0, specular_light_intensity = 0;

	for (size_t i = 0; i < lights.size(); i++)
	{
		Vec3f light_dir = (lights[i].getPosition() - point).normalized();
		float light_distance = (lights[i].getPosition() - point).norm();
		Vec3f shadow_orig = light_dir * N < 0 ? point - N * 1e-3 : point + N * 1e-3; // checking if the point lies in the shadow of the lights[i]
		Vec3f shadow_pt, shadow_N;
		Material tmpmaterial;
		if (scene_intersect(shadow_orig, light_dir, spheres, shadow_pt, shadow_N, tmpmaterial) && (shadow_pt - shadow_orig).norm() < light_distance)
			continue;
		diffuse_light_intensity += lights[i].getIntensity() * std::max(0.f, light_dir * N);
		specular_light_intensity += powf(std::max(0.f, reflect(light_dir, N) * dir), material.getSpecularExponent()) * lights[i].getIntensity();
	}

	return material.getDiffuseColor() * diffuse_light_intensity * material.getAlbedo()[0] + SPECULARALEDOCOLOR * specular_light_intensity * material.getAlbedo()[1] + reflect_color * material.getAlbedo()[2] + refract_color * material.getAlbedo()[3];
}

void render(const vector<Sphere> &spheres, const vector<Light> &lights)
{
	const int width = WIDTH;
	const int height = HEIGHT;
	const float fov = M_PI / 3.;
	vector<Vec3f> framebuffer(width * height);

	for (size_t j = 0; j < height; j++)
	{
		for (size_t k = 0; k < width; k++)
		{
			float x = (2 * (k + 0.5) / (float)width - 1) * tan(fov / 2.) * width / (float)height;
			float y = -(2 * (j + 0.5) / (float)height - 1) * tan(fov / 2.);
			Vec3f dir = Vec3f(x, y, -1).normalized();
			Vec3f envmap_pixel = envmap[k + j * envmap_width];
			framebuffer[k + j * width] = cast_ray(Vec3f(0.f, 0.f, 0.f), dir, spheres, lights);
		}
	}

	ofstream ofs;
	ofs.open("./out.ppm", ofstream::out | ofstream::binary);
	ofs << "P6\n"
		<< width << " " << height << "\n255\n";
	for (size_t i = 0; i < height * width; ++i)
	{
		Vec3f &c = framebuffer[i];
		float max = std::max(c[0], std::max(c[1], c[2]));
		if (max > 1)
			c = c * (1. / max);
		for (size_t j = 0; j < 3; j++)
		{
			ofs << (char)(255 * std::max(0.f, std::min(1.f, framebuffer[i][j])));
		}
	}
	ofs.close();
}

int main()
{
	cout << "Loading environment map..." << endl;
	if (!loadEnvMap(ENVMAP))
	{
		return -1;
	}

	cout << "Setting up spheres..." << endl;
	vector<Sphere> spheres;
	vector<Light> lights;
	spheres.push_back(Sphere(Vec3f(-3, 0, -16), 2, IVORY));
	spheres.push_back(Sphere(Vec3f(-1.0, -1.5, -12), 2, GLASS));
	spheres.push_back(Sphere(Vec3f(1.5, -0.5, -18), 3, REDRUBBER));
	spheres.push_back(Sphere(Vec3f(7, 5, -18), 4, MIRROR));

	cout << "Setting up lights..." << endl;
	lights.push_back(Light(Vec3f(-20, 20, 20), 1.5));
	lights.push_back(Light(Vec3f(30, 50, -25), 1.8));
	lights.push_back(Light(Vec3f(30, 20, 30), 1.7));

	cout << "Starting rendering..." << endl;
	render(spheres, lights);
	cout << "Rendering finished..." << endl;
	return 0;
}