#ifndef COLOR_H
#define COLOR_H
//==============================================================================================
// Originally written in 2020 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication
// along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==============================================================================================

#include "vec.h"

#include <iostream>


struct Colori3
{
	uint_fast8_t r;
	uint_fast8_t g;
	uint_fast8_t b;

};

struct Colorf3
{

	Float r{}, g{}, b{};
	Colorf3() {}
	Colorf3(Float InR, Float InG, Float InB)
		: r{InR}
		, g{InG}
		, b{InB}
	{}

	Colorf3 operator+(Colorf3 const& vec) const
	{
		return Colorf3{ vec.r + r, vec.g + g, vec.b + b };
	}

	Colorf3 operator-(Colorf3 const& vec) const
	{
		return Colorf3{ vec.r - r, vec.g - g, vec.b - b};
	}

	Colorf3 operator*(Colorf3 const& vec) const
	{
		return Colorf3{ vec.r * r, vec.g * g, vec.b * b };
	}

	friend Colorf3 operator*(Colorf3 const& vec, Float scale)
	{
		return Colorf3{ vec.r*scale, vec.g*scale, vec.b*scale};
	}

	friend Colorf3 operator*(Float scale, Colorf3 const& vec)
	{
		return Colorf3{ vec.r*scale, vec.g*scale, vec.b*scale};
	}


	void operator+=(Colorf3 const& other)
	{
		r += other.r;
		g += other.g;
		b += other.b;
	}

	void operator/=(int sample)
	{
		r /= (Float)sample;
		g /= (Float)sample;
		b /= (Float)sample;
	}



	Colori3 toColori3()
	{
		uint_fast8_t ir = static_cast<uint_fast8_t>(256 * clamp(std::sqrt(r), 0.0f, 0.999f));
		uint_fast8_t ig = static_cast<uint_fast8_t>(256 * clamp(std::sqrt(g), 0.0f, 0.999f));
		uint_fast8_t ib = static_cast<uint_fast8_t>(256 * clamp(std::sqrt(b), 0.0f, 0.999f));
		return Colori3{ ir, ig, ib };
	}

	static Colorf3 Random(Float min = 0.0, Float max=1.0)
	{
		return Colorf3{ randomFloat2(min,max),randomFloat2(min,max),randomFloat2(min,max) };
	}
};



#endif
