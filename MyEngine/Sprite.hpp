#pragma once

#include <iostream>
#include <string>
#include <Windows.h>
#include <SDL/SDL.h>
#include <GL/glew.h>

#include "GLTexture.hpp"

namespace MyEngine
{
	class Sprite
	{
		float m_x{};
		float m_y{};
		float m_width{};
		float m_height{};

		GLuint m_vboID;

		GLTexture m_texture;

	public:
		Sprite();
		~Sprite();

		void init(float x, float y, float width, float height, std::string filepath);

		void draw();

	};
}