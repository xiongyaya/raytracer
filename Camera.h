#pragma once

#include"vec.h"

struct Camera
{
	vec4 origin;
	vec4 Horizontal;
	vec4 vertical;
	vec4 LowLeftCorner;

	Camera(vec4 const& LookFrom, vec4 const& LookAt, vec4 const& Up,
		Float Fov, Float AspectRatio)
		:origin{LookFrom}
	{
		Float Radian = degreeToRadians(Fov);
		Float H = std::tan(Radian / 2);
		Float ViewportHeight = 2.0*H;
		Float ViewportWidth = AspectRatio * ViewportHeight;

		auto CForward = (LookAt - LookFrom).normalize();
		auto CRight = (CForward.Cross(Up)).normalize();
		auto CUp = CRight.Cross(CForward);

		Horizontal = CRight * ViewportWidth;
		vertical = ViewportHeight * Up;
		LowLeftCorner = origin - Horizontal / 2.0 - vertical / 2.0 + CForward;
	}

	ray4 GetRay(Float S, Float T)
	{
		return ray4{ origin, LowLeftCorner + S * Horizontal + T * vertical - origin };
	}
};