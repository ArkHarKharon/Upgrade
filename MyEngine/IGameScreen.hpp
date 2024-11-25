#pragma once

#include <iostream>

const int SCREEN_INDEX_NONE = -1;

namespace MyEngine
{
	class IMainGame;

	enum class ScreenState
	{
		NONE, RUNNING, EXIT, CHANGE_NEXT, CHANGE_PREV
	};

	class IGameScreen
	{
	protected:
		int m_screen_index = SCREEN_INDEX_NONE;
		ScreenState m_current_state = ScreenState::NONE;
		IMainGame* m_game = nullptr;

	public:
		//friend class ScreenList;

		IGameScreen()
		{

		}

		virtual ~IGameScreen()
		{

		}

		//Вызываются в начале и в конце работы приложения
		virtual void build() = 0;
		virtual void destroy() = 0;

		//Вызываются когда окно становится и прекращает быть активным соответственно
		virtual void on_entry() = 0;
		virtual void on_exit() = 0;

		//Вызываются в игровой петле
		virtual void update() = 0;
		virtual void draw() = 0;

		virtual int get_next_screen_index() const = 0;
		virtual int get_prev_screen_index() const = 0;


		int get_screen_index() const
		{ 
			return m_screen_index; 
		}
		ScreenState get_current_state() const
		{
			return m_current_state;
		}

		void set_screen_index(int index)
		{
			m_screen_index = index;
		}
		void set_parent_game(IMainGame* game)
		{
			m_game = game;
		}
		void set_running()
		{
			m_current_state = ScreenState::RUNNING;
		}
	};
}
