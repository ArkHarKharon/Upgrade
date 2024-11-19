#include "GameScreen.hpp"

#include <iostream>

GameScreen::GameScreen(MyEngine::Window* window) : m_window{window}
{
	std::cout << "GameScreen инициализирован!\n";
}


GameScreen::~GameScreen()
{

}

void GameScreen::build()
{

}
void GameScreen::destroy()
{

}

void GameScreen::on_entry()
{
	std::cout << "on_entry()\n";

}

void GameScreen::on_exit()
{
	std::cout << "on_ext()\n";

}

void GameScreen::update()
{
	std::cout << "Update()\n";
	check_input();
}

void GameScreen::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	std::cout << "Draw()\n";

}

int GameScreen::get_next_screen_index() const
{
	return SCREEN_INDEX_NONE;
}
int GameScreen::get_prev_screen_index() const
{
	return SCREEN_INDEX_NONE;
}

void  GameScreen::check_input()
{
	SDL_Event evnt;

	while (SDL_PollEvent(&evnt))
	{
		m_game->on_SDL_event(evnt);


	}
}

