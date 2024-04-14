#pragma once

#include <SDL.h>
#include <glew.h>

namespace MyEngine
{
	struct GLTexture
	{
		GLuint id;
		int width;
		int height;
	};
}