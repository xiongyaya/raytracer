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


hittableList RandomScene(hittableList& world) {
	using namespace std;
	auto ground_material = make_shared<Lambertian>(Colorf3(0.5, 0.5, 0.5));
	world.add(make_shared<Sphere>(vec4(0, -1000, 0), 1000, ground_material));

	
	int count = 4;
	for (int a = -count; a < count; a++) {
		for (int b = -count; b < count; b++) {
			auto choose_mat = randomFloat();
			vec4 center(a + 0.9*randomFloat(), 0.2, b + 0.9*randomFloat());

			if ((center - vec4(4, 0.2, 0)).length() > 0.9) {
				shared_ptr<Material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = Colorf3::Random() * Colorf3::Random();
					auto Center1 = center + vec4(0.0, randomFloat(0, 0.5), 0);
					sphere_material = make_shared<Lambertian>(albedo);
					world.add(make_shared<MovingSphere>(center, Center1, 0.0,1.0, 0.2, sphere_material));
				}
				else if (choose_mat < 0.95) {
					// metal
					auto albedo = Colorf3::Random(0.5, 1);
					auto fuzz = randomFloat(0, 0.5);
					sphere_material = make_shared<Metal>(albedo, fuzz);
					world.add(make_shared<Sphere>(center, 0.2, sphere_material));
				}
				else {
					// glass
					sphere_material = make_shared<Dielectric>(1.5);
					world.add(make_shared<Sphere>(center, 0.2, sphere_material));
				}
			}
		}
	}

	
	auto material1 = make_shared<Dielectric>(1.5);
	world.add(make_shared<Sphere>(vec4(0, 1, 0),  1.0, material1));

	auto Center1 = vec4(-4, 1, 0) + vec4(0.0, randomFloat(0, 2), 0);
	shared_ptr<Material> material2 = make_shared<Lambertian>(Colorf3(0.4, 0.2, 0.1));
	world.add(make_shared<MovingSphere>(vec4(-4, 1, 0),Center1,0.0,1.0, 1.0, material2));

	auto material3 = make_shared<Metal>(Colorf3(0.7, 0.6, 0.5), 0.0);
	world.add(make_shared<Sphere>(vec4(4, 1, 0), 1.0, material3));

	return world;
}


int main()
{
	Float AspectRatio = 3.0 / 2.0;
	int width = 200;
	int height = width/AspectRatio;
	int Samples = 100;
	int Depth = 10;
	auto focalLenght = vec4{ 0.0f, 0.0f, 1.0f,0.0f };

	std::vector<Colori3> Data;
	Data.resize(width*height);

	Float Len = (vec4(-2, 2, 1) - vec4(0, 0, -1)).length();
	Camera cam(vec4(13, 2, 3), vec4(0, 0, 0), vec4(0, 1, 0), 20, AspectRatio,0.1, 10, 0.0,1.0);
	hittableList world;
	RandomScene(world);

	for (int i = 0; i < height ; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			Colorf3 ColorF{ 0.0f,0.0f,0.0f };
			for (int k = 0; k < Samples; ++k)
			{
				auto u = Float(j + randomFloat()) / (width - 1);
				auto v = Float(i + randomFloat()) / (height - 1);
				ray4 ray = cam.GetRay(u, v);
				ColorF += RayColor(ray, world, Depth);
			}
			ColorF /= Samples;
			Data.at(i*width +j) = ColorF.toColori3();
			std::cerr << "\rScanlines remaining: " << float(i) / height << ' ' << std::flush;
		}
	}
	stbi_write_tga("l1.tga", width, height, 3, Data.data());
	std::cout << "Hello World!\n";
}