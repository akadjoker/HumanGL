#include "Color.hpp"
#include "Math.hpp"




const Color Color::WHITE;
const Color Color::GRAY(128, 128, 128);
const Color Color::BLACK(0, 0, 0);
const Color Color::RED(255, 0, 0);
const Color Color::GREEN(0, 255, 0);
const Color Color::BLUE(0, 0, 255);
const Color Color::CYAN(0, 255, 255);
const Color Color::MAGENTA(255, 0, 255);
const Color Color::YELLOW(255, 255, 0);
const Color Color::BLANK(0,0,0);


 Color Color::Lerp(const Color& rhs,float t) const
	{
		float invT = 1.0f - t;
		return Color(
			r * invT + rhs.r * t,
			g * invT + rhs.g * t,
			b * invT + rhs.b * t,
			a * invT + rhs.a * t
			);
	}


	u32 Color::ToUInt() const
	{
		unsigned r = (unsigned) Clamp(((int)(this->r * 255.0f)),0,255);
		unsigned g = (unsigned) Clamp(((int)(this->g * 255.0f)),0,255);
		unsigned b = (unsigned) Clamp(((int)(this->b * 255.0f)),0,255);
		unsigned a = (unsigned) Clamp(((int)(this->a * 255.0f)),0,255);
		return (a << 24) | (b << 16) | (g << 8) | r;
	}

    void Color::Set(u8 r, u8 g, u8 b, u8 a)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }