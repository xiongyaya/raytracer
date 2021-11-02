#pragma once


#include"vec.h"
#include"color.h"

struct HitResult;
struct Material
{
	virtual bool scatter(ray4 const& ray, HitResult const& hitResult, Colorf3& Color, ray4& scatterRay) const = 0;
};


struct Lambertian: public Material
{
	Colorf3 albedo;

	Lambertian(Colorf3 const& InAlbedo) : albedo{ InAlbedo } {}

	virtual bool scatter(ray4 const& ray, HitResult const& hitResult, Colorf3& Attenuation, ray4& scatterRay) const override;
};



inline Float schlick(Float cosine, Float EtaiOverEtat)
{
	Float r0 = (1 - EtaiOverEtat) / (1 + EtaiOverEtat);
	r0 = r0 * r0;
	return r0 + (1 - r0)* std::pow(1 - cosine, 5);

}

struct Metal : public Material
{
	Colorf3 albedo;
	Float fuzz;

	Metal(Colorf3 const& InAlbedo, Float InFuzz) : albedo{ InAlbedo }, fuzz{ InFuzz < 1 ? InFuzz : 1 } {}
	virtual bool scatter(ray4 const& ray, HitResult const& hitResult, Colorf3& Color, ray4& scatterRay) const override;
};


struct Dielectric : public Material
{
	Float index;
	Dielectric(float InIndex) :index{ InIndex } {}
	virtual bool scatter(ray4 const& ray, HitResult const& hitResult, Colorf3& Color, ray4& scatterRay) const override;
};