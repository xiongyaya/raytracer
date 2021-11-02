#pragma once

#define Float double

#include<cmath>

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
