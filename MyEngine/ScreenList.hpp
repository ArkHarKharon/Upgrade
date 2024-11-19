#pragma once

#include <vector>

#include "IGameScreen.hpp"

namespace MyEngine
{
	class IMainGame;

	class ScreenList
	{
	protected:
		IMainGame* m_game = nullptr;
		std::vector<IGameScreen*> m_screens;
		int m_current_screen_index = SCREEN_INDEX_NONE;

	public:
		ScreenList(IMainGame* game);
		~ScreenList();

		IGameScreen* move_next();
		IGameScreen* move_prev();

		void set_screen(int next_screen);
		void add_screen(IGameScreen* new_screen);

		IGameScreen* get_current_screen();

		void destroy();
	};
}
