#include <iostream>
#include <Windows.h>
#include <SDL.h>
#include <glew.h>
#include "main_game.hpp"


int main(int argc, char** argv)
{
	Game main_game{};
	main_game.run();

	return 0;
}