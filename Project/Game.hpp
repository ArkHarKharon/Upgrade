#pragma once

#include <MyEngine/IMainGame.hpp>
#include <MyEngine/ScreenList.hpp>

#include "GameScreen.hpp"

class GameApp : public MyEngine::IMainGame
{
private:
	GameScreen* m_gameplay_screen = nullptr;

public:
	GameApp();
	~GameApp();

	virtual void on_init() override;
	virtual void add_screens() override;
	virtual void on_exit() override;

};