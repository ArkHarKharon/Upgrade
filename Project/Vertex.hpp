#pragma once

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

struct Vertex
{
	Position position;
	Color color;
};