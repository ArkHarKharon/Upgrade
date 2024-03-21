#include <iostream>
#include <string>
#include <SDL.h>
#include "main_game.hpp"
#include <glew.h>

void fatal_error(std::string error_str)
{
	std::cout << error_str << "\n";
	std::cout << "Нажми любую клавишу чтобы выйти...";
	int temp;
	std::cin >> temp;
	SDL_Quit();
}

Game::Game()
{
	m_window = nullptr;
	m_WIDTH = 1024;
	m_HEIGHT = 768;
	current_state = GameState::PLAY;
}

Game::~Game()
{

}

void Game::run()
{
	init_system();

	game_loop();
}

void Game::init_system()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	m_window = SDL_CreateWindow("HUESOS", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_WIDTH, m_HEIGHT, SDL_WINDOW_OPENGL);
	if (m_window == nullptr)
		fatal_error("Окно НЕ было открыто!");

	SDL_GLContext gl_context = SDL_GL_CreateContext(m_window);
	if (gl_context == nullptr)
		fatal_error("Контекст отрисовки OpenGl НЕ был создан!");

	GLenum error = glewInit();
	if (error != GLEW_OK)
		fatal_error("Glew НЕ был инициализирован!");

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
}

void Game::process_input()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case(SDL_QUIT):
			current_state = GameState::EXIT;
			break;

		case(SDL_MOUSEMOTION):
			std::cout << event.motion.x << " " << event.motion.y << "\n";
			break;

		case(SDL_MOUSEBUTTONDOWN):
			std::cout << event.button.timestamp << "\n";
			std::cout << event.button.x << " " << event.button.x << "\n";
			break;

		}
	};
}

void Game::game_loop()
{
	while (current_state != GameState::EXIT)
	{
		process_input();
		draw_game();
	}
}

void Game::draw_game()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisableClientState(GL_COLOR_ARRAY);

	glBegin(GL_TRIANGLES);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(0,0); 
	glVertex2f(0, 500);
	glVertex2f(500, 500);

	glEnd();

	SDL_GL_SwapWindow(m_window);
}

