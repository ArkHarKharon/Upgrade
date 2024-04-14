#include <iostream>
#include "TextureCache.hpp"
#include "ImageLoader.hpp"

namespace MyEngine
{

	TextureCache::TextureCache()
	{

	}

	TextureCache::~TextureCache()
	{

	}

	GLTexture TextureCache::get_texture(std::string filepath)
	{
		auto map_iterator = m_texture_map.find(filepath);

		if (map_iterator == m_texture_map.end())
		{
			GLTexture new_texture = ImageLoader::loadPNG(filepath);
			m_texture_map.insert(make_pair(filepath, new_texture));
			std::cout << "Использована кэшированная текстура!\n";
			return new_texture;
		}
		std::cout << "Использована текстура!";

		return map_iterator->second;
	}
}