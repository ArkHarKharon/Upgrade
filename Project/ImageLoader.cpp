#include <vector>

#include "ImageLoader.hpp"
#include "picoPNG.hpp"
#include "IOManager.hpp"
#include "Errors.hpp"

GLTexture ImageLoader::loadPNG(std::string filepath)
{
	GLTexture texture{};


	std::vector<unsigned char> output;
	std::vector<unsigned char> input;

	unsigned long width{}, height{};

	if (IOManager::read_file_to_buffer(filepath, input))
		fatal_error("PNG файл НЕ был загружен в буфер!");

	int error_code = decodePNG(output, width, height, &(input.at(0)), input.size());

	if (error_code)
		fatal_error("Ошибка decodePNG(): " + error_code);

	glGenTextures(1, &texture.id);

	glBindTexture(GL_TEXTURE_2D, texture.id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(output.at(0)));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	texture.width = width;
	texture, height = height;

	return texture;
}
