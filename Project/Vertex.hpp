#pragma once

struct Vertex
{
	struct Position
	{
		float x;
		float y;
	}position;

	struct Color
	{
		unsigned char red;
		unsigned char green;
		unsigned char blue;
		unsigned char alpha;
	}color;
};