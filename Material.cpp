#include "material.h"
#include "hittable.h"

bool Lambertian::scatter(ray4 const& ray, HitResult const& hitResult, Colorf3& Attenuation, ray4& scatterRay) const
{
	scatterRay.origin = hitResult.point;
	scatterRay.dir = RandomUnitHemiSphere(hitResult.normal);
	scatterRay.time = ray.time;
	Attenuation = albedo;
	return true;
}

bool Metal::scatter(ray4 const& ray, HitResult const& hitResult, Colorf3& Color, ray4& scatterRay) const
{
	scatterRay.origin = hitResult.point;
	vec4 reflectVec = reflect(ray.dir, hitResult.normal);
	scatterRay.dir = reflectVec + fuzz * RandomUnitHemiSphere(hitResult.normal);

	Color = albedo;
	return hitResult.normal.dot(scatterRay.dir) > 0;
}

bool Dielectric::scatter(ray4 const& ray, HitResult const& hitResult, Colorf3& Color, ray4& scatterRay) const
{
	Float EtaiOverEtat = hitResult.frontFace ? 1.0 / index : index;
	
	
	Color = Colorf3(1.0, 1.0, 1.0);
	vec4 normalizeDir = -ray.dir.normalize();
	Float cosTheta = std::fmin(normalizeDir.dot( hitResult.normal), 1.0);
	Float sinTheta = std::sqrt(1 - cosTheta * cosTheta);
	if (EtaiOverEtat * sinTheta > 1.0)
	{
		scatterRay.origin = hitResult.point;
		scatterRay.dir = reflect(normalizeDir,hitResult.normal);
		return true;
	}
	Float prob = schlick(cosTheta, EtaiOverEtat);
	if (randomFloat() < prob)
	{
		scatterRay.origin = hitResult.point;
		scatterRay.dir = reflect(normalizeDir, hitResult.normal);
		return true;
	}
	vec4 refractVec = refract(normalizeDir, hitResult.normal, EtaiOverEtat);
	scatterRay.origin = hitResult.point;
	scatterRay.dir = refractVec;
	
	return true;
}