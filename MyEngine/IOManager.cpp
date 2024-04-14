#include "IOManager.hpp"
#include "Errors.hpp"
#include <fstream>

namespace MyEngine
{

	bool IOManager::read_file_to_buffer(std::string filepath, std::vector<unsigned char>& buffer)
	{
		std::ifstream file(filepath, std::ios::binary);
		if (file.fail())
		{
			fatal_error("Файл по адресу " + filepath + " НЕ открыт!");
			return false;
		}

		//Определяю размер файла
		file.seekg(0, std::ios::end);
		int file_size = file.tellg();
		file.seekg(0, std::ios::beg);

		//Убираю заголовочные биты
		file_size -= file.tellg();

		buffer.resize(file_size);
		file.read((char*)&(buffer.at(0)), file_size);
		file.close();

		return true;
	}
}