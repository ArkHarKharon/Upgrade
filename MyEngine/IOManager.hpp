#pragma once
#include <vector>
#include <string>

namespace MyEngine
{
	class IOManager
	{
	public:
		static bool read_file_to_buffer(std::string filepath, std::vector<unsigned char>& buffer);
	};
}