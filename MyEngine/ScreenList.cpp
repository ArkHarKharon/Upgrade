#include "ScreenList.hpp"

namespace MyEngine
{
	ScreenList::ScreenList(IMainGame* game) : m_game{game}
	{

	}
	ScreenList::~ScreenList()
	{
		destroy();
	}

	IGameScreen* ScreenList::move_next()
	{
		IGameScreen* current_screen = get_current_screen();

		if (current_screen->get_next_screen_index() != SCREEN_INDEX_NONE)
		{
			m_current_screen_index = current_screen->get_next_screen_index();
		}

		return get_current_screen();
	}

	IGameScreen* ScreenList::move_prev()
	{
		IGameScreen* current_screen = get_current_screen();

		if (current_screen->get_prev_screen_index() != SCREEN_INDEX_NONE)
		{
			m_current_screen_index = current_screen->get_prev_screen_index();
		}

		return get_current_screen();
	}

	void ScreenList::set_screen(int next_screen)
	{
		m_current_screen_index = next_screen;
	}

	void ScreenList::add_screen(IGameScreen* new_screen)
	{
		new_screen->set_screen_index(m_screens.size());

		m_screens.push_back(new_screen);
		new_screen->build();
		new_screen->set_parent_game(m_game);
	}

	IGameScreen* ScreenList::get_current_screen()
	{
		if (m_current_screen_index == SCREEN_INDEX_NONE)
			return nullptr;

		return m_screens.at(m_current_screen_index);
	}


	void ScreenList::destroy()
	{
		for (size_t i = 0; i < m_screens.size(); i++)
		{
			m_screens.at(i)->destroy();
		}
		m_screens.resize(0);
		m_current_screen_index = SCREEN_INDEX_NONE;
	}

}