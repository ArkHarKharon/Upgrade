#include "Errors.hpp"
#include <string>
#include <iostream>
#include <SDL.h>
#include <glew.h>

void fatal_error(std::string error_str)
{
	std::cout << error_str << "\n";
	std::cout << "����� ����� ������� ����� �����...";
	int temp;
	std::cin >> temp;
	SDL_Quit();
}
