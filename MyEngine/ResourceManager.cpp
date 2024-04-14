#include "ResourceManager.hpp"

namespace MyEngine
{

	TextureCache ResourceManager::m_texture_cache;

	GLTexture ResourceManager::get_texture(std::string filepath)
	{
		return m_texture_cache.get_texture(filepath);
	}
}