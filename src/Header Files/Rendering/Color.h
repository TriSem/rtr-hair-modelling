#pragma once

namespace Rendering
{
	struct Color
	{
		Color() :
			red(0),
			blue(0),
			green(0),
			alpha(255)
		{

		}

		Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) :
			red(red),
			green(green),
			blue(blue),
			alpha(alpha)
		{
		}

		uint8_t red;
		uint8_t green;
		uint8_t blue;
		uint8_t alpha;
	};
}