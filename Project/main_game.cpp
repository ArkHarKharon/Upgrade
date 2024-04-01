#include <iostream>
#include <string>

#include <SDL.h>
#include <glew.h>

#include "main_game.hpp"
#include "Sprite.hpp"
#include "Errors.hpp"
#include "GLSL.hpp"


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

	m_sprite.init(-0.5f, -0.5f, 2.0f, 2.0f);

	game_loop();
}

void Game::init_shaders()
{
	m_color_program.compile_shaders("Shaders/vertex_shader.txt","Shaders/fragment_shader.txt");
	m_color_program.add_attribute("vertexPosition");
	m_color_program.add_attribute("vertexColor");
	m_color_program.link_shaders();
}

void Game::init_system()
{
	setlocale(0, "");

	SDL_Init(SDL_INIT_EVERYTHING);
	m_window = SDL_CreateWindow("CHLENOZAVR", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_WIDTH, m_HEIGHT, SDL_WINDOW_OPENGL);
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

	init_shaders();
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
			//std::cout << event.button.timestamp << "\n";
			//std::cout << event.button.x << " " << event.button.x << "\n";
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

	m_color_program.use();

	m_sprite.draw();

	m_color_program.unuse();

	SDL_GL_SwapWindow(m_window);
}
