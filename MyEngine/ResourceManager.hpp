#pragma once
#include <string>
#include "TextureCache.hpp"


namespace MyEngine
{

	class ResourceManager
	{
		static TextureCache m_texture_cache;

	public:
		static GLTexture get_texture(std::string filepath);
	};
}