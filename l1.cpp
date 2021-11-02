// l1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#define __STDC_LIB_EXT1__ 1
#include <iostream>
#include<vector>

#include "color.h"
#include "vec.h"
#include "hittable.h"
#include "material.h"
#include "Camera.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

Float hitSphere(const vec4& center, Float radius, const ray4& r)
{
	vec4 oc = r.origin - center;
	auto a = r.dir.square();
	auto b = 2.0f * r.dir.dot(oc);
	auto c = oc.square() - radius * radius;
	auto disc = b * b - 4 * a*c;
	if (disc < 0)
	{
		return -1.0f;
	}

	return -b - std::sqrt(disc)/(2.0f*a);
}

Colorf3 RayColor(ray4 const& r, const Hittable& world, int Depth)
{
	if (Depth <= 0)
	{
		return Colorf3{ 0.0f,0.0f,0.0f };
	}

	Float t = 0.0f;
	HitResult result;
	if ( world.hit(r,0.001f,INF, result))
	{
		ray4 scattered;
		Colorf3 attenuation;
		if (result.Material->scatter(r, result, attenuation, scattered))
		{
			return attenuation * RayColor(scattered, world, Depth - 1);
		}
		return Colorf3(0.0f, 0.0f, 0.0f);
	}
	vec4 unitDir = r.dir.normalize();
	t = 0.5f*(unitDir.y + 1.0f);
	return (1.0f - t)*Colorf3 { 1.0f, 1.0f, 1.0f }+t * Colorf3{ 0.5f, 0.7f, 1.0f };
}

int main()
{
	int width = 256;
	int height = 256;
	int Samples = 50;
	int Depth = 10;
	Float AspectRatio = width / height;

	auto origin = vec4{ 0.0f,0.0f,0.0f,0.0f };
	auto focalLenght = vec4{ 0.0f, 0.0f, 1.0f,0.0f };

	std::vector<Colori3> Data;
	Data.resize(width*height);

	Camera cam(vec4(-2, 2, 1), vec4(0, 0, -1), vec4(0, 1, 0), 90, AspectRatio);
	hittableList world;

	auto material_ground = std::make_shared<Lambertian>(Colorf3(0.8, 0.8, 0.0));
	auto material_center = std::make_shared<Lambertian>(Colorf3(0.1, 0.2, 0.5));
	auto material_left = std::make_shared<Dielectric>(1.5);
	auto material_right = std::make_shared<Metal>(Colorf3(0.8, 0.6, 0.2), 0.0);

	world.add(std::make_shared<Sphere>(vec4(0.0, -100.5, -1.0), 100.0, material_ground));
	world.add(std::make_shared<Sphere>(vec4(0.0, 0.0, -1.0), 0.5, material_center));
	world.add(std::make_shared<Sphere>(vec4(-1.0, 0.0, -1.0), 0.5, material_left));
	world.add(std::make_shared<Sphere>(vec4(-1.0, 0.0, -1.0), -0.45, material_left));
	world.add(std::make_shared<Sphere>(vec4(1.0, 0.0, -1.0), 0.5, material_right));

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			Colorf3 ColorF{ 0.0f,0.0f,0.0f };
			for (int k = 0; k < Samples; ++k)
			{
				auto u = Float(i+randomFloat()) / (height - 1);
				auto v = Float(j + randomFloat()) / (width - 1);
				ray4 ray = cam.GetRay(u, v);
				ColorF += RayColor(ray, world, Depth);
			}
			ColorF /= Samples;
			Data.at(j*width + i) = ColorF.toColori3();
		}
	}
	stbi_write_tga("l1.tga", width, height, 3, Data.data());
	std::cout << "Hello World!\n";
}