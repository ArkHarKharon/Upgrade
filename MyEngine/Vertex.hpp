#pragma once

namespace MyEngine
{
	struct Position
	{
		float x;
		float y;
	};

	struct Color
	{
		unsigned char red;
		unsigned char green;
		unsigned char blue;
		unsigned char alpha;
	};

	struct UV
	{
		float u;
		float v;
	};

	struct Vertex
	{
		Position position;
		Color color;
		UV uv;

		void set_position(float x, float y)
		{
			position.x = x;
			position.y = y;
		}

		void set_color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
		{
			color.red = red;
			color.green = green;
			color.blue = blue;
			color.alpha = alpha;
		}

		void set_UV(float u, float v)
		{
			uv.u = u;
			uv.v = v;
		}
	};
}