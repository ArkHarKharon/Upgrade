#pragma once
#include <map>
#include <string>

#include "GLTexture.hpp"

namespace MyEngine
{
	class TextureCache
	{
		std::map<std::string, GLTexture> m_texture_map;
	public:
		TextureCache();
		~TextureCache();

		GLTexture get_texture(std::string filepath);
	};
}