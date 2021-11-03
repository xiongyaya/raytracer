#pragma once

#include"vec.h"

struct Camera
{
	vec4 origin;
	vec4 Horizontal;
	vec4 vertical;
	vec4 LowLeftCorner;
	Float LensRadius;

	vec4 Forward, Right, Up;


	Camera(vec4 const& LookFrom, vec4 const& LookAt, vec4 const& InUp,
		Float Fov, Float AspectRatio, Float Aperture, Float FocusDist)
		:origin{LookFrom}
	{
		Float Radian = degreeToRadians(Fov);
		Float H = std::tan(Radian / 2);
		Float  ViewportHeight = 2.0*H;
		Float  ViewportWidth  = ViewportHeight * AspectRatio;

		Forward = (LookFrom - LookAt ).normalize();
		Right = (InUp.Cross(Forward)).normalize();
		Up = Forward.Cross(Right);

		Horizontal = FocusDist*Right * ViewportWidth;
		vertical = FocusDist*ViewportHeight * Up;
		LowLeftCorner = origin - Horizontal / 2.0 - vertical / 2.0 - FocusDist*Forward;
		LensRadius = Aperture / 2;
	}

	ray4 GetRay(Float S, Float T)
	{
		 vec4 rd = LensRadius * RandomInUnitDisk();
		 vec4 offset = Right * rd.x + Up * rd.y;
		return ray4{ origin+offset, LowLeftCorner + S * Horizontal + T * vertical - origin - offset };
	}
};