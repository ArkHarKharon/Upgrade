#pragma once

#include <SDL.h>
#include <glew.h>

#include <MyEngine/Sprite.hpp>
#include <MyEngine/GLSL.hpp>
#include <MyEngine/GLTexture.hpp>
#include <MyEngine/Camera2D.hpp>
#include <MyEngine/ImageLoader.hpp>
#include <MyEngine/Errors.hpp>
#include <MyEngine/Window.hpp>

#include <vector>
#include <iostream>
#include <string>
#include <array>
#include <Windows.h>


enum class GameState
{
	PLAY,
	EXIT,
};

class Game
{
	MyEngine::Window m_window;

	GameState m_current_state;

	int m_window_width;
	int m_window_height;

	std::vector<MyEngine::Sprite> m_sprites;
	MyEngine::GLSLProgram m_color_program;

	float m_time;

	float m_fps;
	float m_max_fps;
	float m_frame_time;

	MyEngine::Camera2D m_camera;

	void init_shaders();
	void init_system(); 
	void process_input();
	void game_loop();
	void draw_game();
	void fps_counter(int samples_num);

public:
	Game();
	~Game();

	void run();
};