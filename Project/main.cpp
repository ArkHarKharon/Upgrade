#include <iostream>
#include <Windows.h>
#include <SDL.h>
#include <glew.h>
#include "main_game.hpp"


int main(int argc, char** argv)
{
	Game main_game{};
	main_game.run();

	std::cout << "Enter any key to exit!";
	int a; 
	std::cin >> a;
	return 0;
}