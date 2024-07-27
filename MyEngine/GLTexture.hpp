#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>

namespace MyEngine
{
	struct GLTexture
	{
		GLuint id;
		int width;
		int height;
	};
}