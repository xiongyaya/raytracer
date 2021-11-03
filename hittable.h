#pragma once

#include"vec.h"
#include"material.h"   



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

struct MovingSphere : public Hittable
{
	vec4 Center0, Center1, Vec;
	Float Radius;
	Float Time0, Time1, TimeLengthInv;
	std::shared_ptr<Material> MatRef;

	MovingSphere() {}
	MovingSphere( vec4 const& InCenter0, vec4 const& InCenter1, Float InTime0, Float InTime1,
		Float InRadius, std::shared_ptr<Material>& InMatRef)
		: Center0{InCenter0}
		, Center1{InCenter1}
		, Time0{InTime0}
		, Time1{InTime1}
		, Radius{InRadius}
		, MatRef{InMatRef}
		, TimeLengthInv{1.0 / (InTime1-InTime0)}
		, Vec{InCenter1-InCenter0}
	{}

	virtual bool hit(const ray4& ray, Float tmin, Float tmax, HitResult& result) const override
	{
		vec4 TimeCenter = Center(ray.time);
		vec4 oc = ray.origin - TimeCenter;
		auto a = ray.dir.square();
		auto hb = oc.dot(ray.dir);
		auto c = oc.square() - Radius * Radius;
		auto disc = hb * hb - a * c;
		if (disc > 0.0f)
		{
			auto root = std::sqrt(disc);

			auto t = (-hb - root) / a;
			if (t < tmax && t > tmin)
			{
				result.t = t;
				result.point = ray.at(t);
				auto outwardNormal = (result.point - TimeCenter) / Radius;
				result.setFaceNormal(ray, outwardNormal);
				result.Material = MatRef;
				return true;
			}

			t = (-hb + root) / a;
			if (t < tmax && t > tmin)
			{
				result.t = t;
				result.point = ray.at(t);
				auto outwardNormal = (result.point - TimeCenter) / Radius;
				result.setFaceNormal(ray, outwardNormal);
				result.Material = MatRef;
				return true;
			}
		}
		return false;
	}

	vec4 Center(Float Time) const
	{
		return Center0 + (Time - Time0) * TimeLengthInv * Vec;
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


