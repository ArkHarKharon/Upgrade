#pragma once

#include <string>
#include "GLTexture.hpp"

namespace MyEngine
{
	class ImageLoader
	{
	public:
		static GLTexture loadPNG(std::string filepath);
	};
}