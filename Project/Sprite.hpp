#pragma once

#include <iostream>
#include <Windows.h>
#include <SDL.h>
#include <glew.h>
#include "main_game.hpp"

class Sprite
{
	float m_x{};
	float m_y{};
	float m_width{};
	float m_height{};

	GLuint m_vboID;

public:
	Sprite();
	~Sprite();

	void init(float x, float y, float width, float height);

	void draw();

};