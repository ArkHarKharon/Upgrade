#include "Errors.hpp"
#include <string>
#include <iostream>
#include <SDL/SDL.h>
#include <GL/glew.h>
namespace MyEngine
{

	void fatal_error(std::string error_str)
	{
		std::cout << error_str << "\n";
		std::cout << "����� ����� ������� ����� �����...";
		int temp;
		std::cin >> temp;
		SDL_Quit();
	}
}