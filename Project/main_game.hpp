#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <MyEngine/Sprite.hpp>
#include <MyEngine/GLSL.hpp>
#include <MyEngine/GLTexture.hpp>
#include <MyEngine/Camera2D.hpp>
#include <MyEngine/ImageLoader.hpp>
#include <MyEngine/Errors.hpp>
#include <MyEngine/Window.hpp>
#include <MyEngine/SpriteBatch.hpp>
#include <MyEngine/ResourceManager.hpp>
#include <MyEngine/InputManager.hpp>
#include <MyEngine/Timing.hpp>

#include "GameObjects.hpp"

#include <vector>
#include <iostream>
#include <string>
#include <array>
#include <Windows.h>


enum class GameState
{
	MAIN_MENU,
	UPGRADE_MENU,
	GAME,
	EXIT,
};

class Game
{
	MyEngine::Window m_window;
	int m_window_width;
	int m_window_height;

	MyEngine::FPS_Limiter m_fps_limiter;
	int m_fps;
	int m_max_fps;

	MyEngine::Camera2D m_camera;
	float m_cam_speed;
	float m_cam_scale;

	GameState m_current_state;

	MyEngine::GLSLProgram m_color_program;

	MyEngine::SpriteBatch m_tank_sprite_batch;

	MyEngine::InputManager m_input_manager;

	std::vector<Projectile> m_projectiles;

	std::vector <Level*> m_levels;

	Tank* m_player1;
	Tank* m_player2;

	std::vector<Tank*> m_tanks;

	float m_time;

	bool m_game_is_started;

	void init_shaders();
	void init_system(); 
	void process_input();
	void game_loop();
	void draw_game();

	void init_level();
	void start_round();
public:
	Game();
	~Game();

	void run();
};