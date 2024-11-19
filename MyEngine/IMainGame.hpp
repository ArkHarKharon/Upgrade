#pragma once

#include "Window.hpp"
#include "InputManager.hpp"

namespace MyEngine
{

	class ScreenList;
	class IGameScreen;

	class IMainGame
	{
	protected:
		ScreenList* m_screen_list = nullptr;
		IGameScreen* m_current_screen = nullptr;
		bool m_is_running = false;
		float m_fps = 0.0f;
		Window m_window;
		InputManager m_input_manager;

		bool init();
		bool init_system();

		virtual void update();
		virtual void draw();


	public:
		IMainGame();
		virtual ~IMainGame();

		void run();
		void exit_game();
		
		virtual void on_init() = 0;
		virtual void add_screens() = 0;
		virtual void on_exit() = 0;


		const float get_fps() const
		{
			return m_fps;
		}

		void on_SDL_event(SDL_Event& event);

	};
}
