#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <MyEngine/IGameScreen.hpp>
#include <MyEngine/SpriteBatch.hpp>
#include <MyEngine/GLSL.hpp>
#include <MyEngine/Camera2D.hpp>
#include <MyEngine/GLTexture.hpp>
#include <MyEngine/Window.hpp>
#include <MyEngine/IMainGame.hpp>
#include <MyEngine/InputManager.hpp>



class GameScreen : public MyEngine::IGameScreen
{
private:
	MyEngine::SpriteBatch m_spriteBatch;
	MyEngine::GLSLProgram m_textureProgram;
	MyEngine::Camera2D m_camera;
	MyEngine::GLTexture m_texture;
	MyEngine::Window* m_window;

	void check_input();

public:
	GameScreen(MyEngine::Window* window);
	~GameScreen();

	virtual void build() override;
	virtual void destroy() override;

	virtual void on_entry() override;
	virtual void on_exit() override;

	virtual void update() override;
	virtual void draw() override;

	virtual int get_next_screen_index() const override;
	virtual int get_prev_screen_index() const override;
};