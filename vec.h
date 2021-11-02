#pragma once

#include<cmath>
#include<vector>
#include<memory>

#define Float double

class vec4
{
public:
	Float x = 0.0f;
	Float y = 0.0f;
	Float z = 0.0f;
	Float w = 0.0f;


	vec4(Float inX = 0.0f, Float inY = 0.0f, Float inZ = 0.0f, Float inW = 0.0f)
		: x{ inX }
		, y{ inY }
		, z{ inZ }
		, w{inW}
	{}

	vec4 operator-() const
	{
		return vec4{ -x, -y, -z, w };
	}


	vec4 operator/(Float scale) const
	{
		return vec4{ x / scale, y / scale, z / scale, w / scale };
	}

	vec4 operator+(vec4 const& vec) const
	{
		return vec4{ vec.x + x, vec.y + y, vec.z + z, vec.w + w };
	}

	vec4 operator-(vec4 const& vec) const
	{
		return vec4{ x - vec.x, y - vec.y, z - vec.z, w - vec.w };
	}

	friend vec4 operator*(vec4 const& vec, Float scale)
	{
		return vec4{ vec.x*scale, vec.y*scale, vec.z*scale, vec.w*scale };
	}

	friend vec4 operator*(Float scale, vec4 const& vec)
	{
		return vec4{ vec.x*scale, vec.y*scale, vec.z*scale, vec.w*scale };
	}

	Float dot(vec4 const& vec) const
	{
		return x * vec.x + y * vec.y + z * vec.z;
	}

	vec4 Cross(vec4 const& vec) const 
	{
		return vec4{ y*vec.z - z * vec.y, z*vec.x - x * vec.z, x*vec.y - y * vec.x, 0.0 };
	}

	Float square() const
	{
		return dot(*this);
	}

	Float length() const
	{
		return std::sqrt(square());
	} 
	vec4 normalize() const 
	{
		Float len = length();
		return vec4{ x / len, y / len, z / len, 0.0f };
	}

	
};


struct ray4
{
public:
	vec4 origin{}, dir{};

	ray4() {}
	ray4(vec4 const& InOrigin, vec4 const& InDir)
		: origin{InOrigin}
		, dir{InDir.normalize()}
	{}

	vec4 at(Float t) const
	{
		return origin + t * dir;
	}
};

inline Float clamp(Float x, Float min, Float max)
{
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

inline vec4 reflect(vec4 const& origin, vec4 const& normal)
{
	return origin - 2 * origin.dot(normal)*normal;
}

inline vec4 refract(vec4 const& origin, vec4 const& normal, Float EtaiOverEtat)
{
	Float CosTheta = (-origin).dot(normal);
	vec4 Perp = EtaiOverEtat * (origin + CosTheta * normal);
	vec4 Para = -std::sqrt(std::fabs(1.0 - Perp.square()))*normal;
	return Perp + Para;
}

constexpr Float INF = std::numeric_limits<Float>::infinity();
constexpr Float PI = 3.141592653;

constexpr inline Float degreeToRadians(Float degree)
{
	return degree * PI / 180.0f;
}


#include<random>

inline Float randomFloat(Float min = 0.0f, Float max = 1.0f)
{
	static std::uniform_real_distribution<Float> Distribution(min, max);
	static std::random_device DefaultRD;
	static std::mt19937 generator(DefaultRD());
	return Distribution(generator);
}

inline Float randomFloat2(Float min = 0.0f, Float max = 1.0f)
{
	return min + (max - min) * rand() / (RAND_MAX + 1.0);
}



inline vec4 RandomVec3()
{
	return vec4(randomFloat(), randomFloat(), randomFloat());
}

inline vec4 RandomVec3(Float min, Float max)
{
	return vec4(randomFloat2(min, max), randomFloat2(min, max), randomFloat2(min, max));
}

inline vec4 RandomUnitSphere()
{
	while (true)
	{
		auto p = RandomVec3(-1.0f, 1.0f);
		return p.normalize();

		//if (p.square() >= 1.0) continue;
		//return p;
	}
}

inline vec4 RandomUnitHemiSphere(vec4 const& normal)
{
	auto p = RandomVec3(-1.0f, 1.0f).normalize();
	if (p.dot(normal) > 0.0f)
	{
		return p;
	}
	return -p;
}
