#include <SDL.h>
#include "main_game.hpp"
#include <glew.h>


	void Game::run()
	{
		init_system();
	}

	void Game::init_system()
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		m_window = SDL_CreateWindow("HUESOS", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_OPENGL);

	}