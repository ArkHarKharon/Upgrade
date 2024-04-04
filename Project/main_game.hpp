#pragma once

#include <SDL.h>
#include <glew.h>


#include "Sprite.hpp"
#include "GLSL.hpp"
#include "GLTexture.hpp"


enum class GameState
{
	PLAY,
	EXIT,
};

class Game
{
	SDL_Window* m_window;
	GameState m_current_state;

	int m_window_width;
	int m_window_height;

	Sprite m_sprite{};
	GLSLProgram m_color_program;
	GLTexture m_tank_texture;

	float m_time;

	void init_shaders();
	void init_system(); 
	void process_input();
	void game_loop();
	void draw_game();

public:
	Game();
	~Game();

	void run();
};