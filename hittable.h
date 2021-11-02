#pragma once

#include"vec.h"
#include"material.h"   
#include<cmath>
#include<vector>
#include<memory>


struct HitResult
{
	vec4 point;
	vec4 normal;
	Float t;
	bool frontFace;
	std::shared_ptr<Material> Material;

	inline void setFaceNormal(ray4 const& r, const vec4& outwardNormal)
	{
		frontFace = r.dir.dot(outwardNormal) < 0;
		normal = frontFace ? outwardNormal : -outwardNormal;
	}
};


struct Hittable
{
	virtual bool hit(const ray4& ray, Float tmin, Float tmax, HitResult& result) const =0;
};


struct Sphere :public Hittable
{
	vec4 center{};
	Float r{ 0.0f };
	std::shared_ptr<Material> mat;

	Sphere(vec4 const& InCenter, Float InRadius, std::shared_ptr<Material> InMat)
		: center{InCenter}
		, r{ InRadius }
		, mat{ InMat }
	{}

	virtual bool hit(const ray4& ray, Float tmin, Float tmax, HitResult& result) const override
	{
		vec4 oc = ray.origin - center;
		auto a = ray.dir.square();
		auto hb = oc.dot(ray.dir);
		auto c = oc.square() - r * r;
		auto disc = hb * hb - a * c;
		if (disc > 0.0f)
		{
			auto root = std::sqrt(disc);

			auto t = (-hb - root) / a;
			if (t < tmax && t > tmin)
			{
				result.t = t;
				result.point = ray.at(t);
				auto outwardNormal = (result.point - center) / r;
				result.setFaceNormal(ray, outwardNormal);
				result.Material = mat;
				return true;
			}

			t = (-hb + root) / a;
			if (t < tmax && t > tmin)
			{
				result.t = t;
				result.point = ray.at(t);
				auto outwardNormal = (result.point - center) / r;
				result.setFaceNormal(ray, outwardNormal);
				result.Material = mat;
				return true;
			}
		}
		return false;
	}
};


struct hittableList: public Hittable
{
	std::vector<std::shared_ptr<Hittable>> objects;

	void clear() { objects.clear(); }
	void add(std::shared_ptr<Hittable> const& object)
	{
		objects.push_back(object);
	}

	virtual bool hit(const ray4& ray, Float tmin, Float tmax, HitResult& result) const override
	{
		HitResult tempResult;
		bool hitAny = false;
		auto closeSoFar = tmax;
		for (const auto& object : objects)
		{
			if (object->hit(ray, tmin, closeSoFar, tempResult))
			{
					hitAny = true;
					closeSoFar = tempResult.t;
					result = tempResult;
			}
		}
		return hitAny;
	}

};

constexpr Float INF = std::numeric_limits<Float>::infinity();
constexpr Float PI = 3.141592653;

constexpr inline Float degreeToRadians(Float degree)
{
	return degree * PI / 180.0f;
}

#include<random>

inline Float randomFloat(Float min=0.0f, Float max=1.0f)
{
	static std::uniform_real_distribution<Float> Distribution(min,max);
	static std::random_device DefaultRD;
	static std::mt19937 generator(DefaultRD());
	return Distribution(generator);
}

inline Float randomFloat2(Float min = 0.0f, Float max = 1.0f)
{
	return min + (max-min) * rand() / (RAND_MAX+1.0);
}



inline vec4 RandomVec3()
{
	return vec4(randomFloat(), randomFloat(), randomFloat());
}

inline vec4 RandomVec3(Float min, Float max)
{
	return vec4(randomFloat2(min,max), randomFloat2(min, max), randomFloat2(min, max));
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