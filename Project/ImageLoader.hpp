#pragma once

#include <string>
#include "GLTexture.hpp"

class ImageLoader
{
public:
	static GLTexture loadPNG(std::string filepath);
};