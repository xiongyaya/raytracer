// l1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#define __STDC_LIB_EXT1__ 1
#include <iostream>
#include<vector>

#include "color.h"
#include "vec.h"
#include "hittable.h"
#include "material.h"

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
	Float viewportHeight = 2;
	Float viewportWidth = viewportHeight * width / height;

	auto origin = vec4{ 0.0f,0.0f,0.0f,0.0f };
	auto horizontal = vec4{ viewportWidth, 0.0f, 0.0f, 0.0f };
	auto vertical = vec4{ 0.0f, viewportHeight, 0.0f,0.0f };
	auto focalLenght = vec4{ 0.0f, 0.0f, 1.0f,0.0f };
	auto lowLeftCorner = origin - horizontal / 2 - vertical / 2- focalLenght;

	std::vector<Colori3> Data;
	Data.resize(width*height);

	auto materialGround = std::make_shared<Lambertian>(Colorf3(0.8f, 0.8f, 0.0f));
	auto materialCenter = std::make_shared<Lambertian>(Colorf3(0.7f, 0.3f, 0.3f));
	auto materialLeft = std::make_shared<Dielectric>(1.5);
	auto materialRight = std::make_shared<Metal>(Colorf3(0.8f, 0.6f, 0.2f), 1.0);


	//hittableList world;
	//world.add(std::make_shared<Sphere>(vec4(0.0f, 0.0f, -1.0f), 0.5f));
	//world.add(std::make_shared<Sphere>(vec4(0.0f, -100.5f, -1.0f), 100.0f));

	hittableList world;
	world.add(std::make_shared<Sphere>(vec4(0.0f, -100.5f, -1.0f), 100.0f, materialGround));
	world.add(std::make_shared<Sphere>(vec4(0.0f, 0.0f, -1.0f), 0.5f, materialCenter));
	world.add(std::make_shared<Sphere>(vec4(-1.0f, 0.0f, -1.0f), 0.5f, materialLeft));
	world.add(std::make_shared<Sphere>(vec4(1.0f, 0.0f, -1.0f), 0.5f, materialRight));


	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			Colorf3 ColorF{ 0.0f,0.0f,0.0f };
			for (int k = 0; k < Samples; ++k)
			{
				auto u = Float(i+randomFloat()) / (height - 1);
				auto v = Float(j + randomFloat()) / (width - 1);
				ray4 r( origin, lowLeftCorner + u * horizontal + v * vertical );
				
				ColorF += RayColor(r, world, Depth);
			}
			ColorF /= Samples;
			Data.at(j*width + i) = ColorF.toColori3();
		}
	}
	stbi_write_tga("l1.tga", width, height, 3, Data.data());
	std::cout << "Hello World!\n";
}