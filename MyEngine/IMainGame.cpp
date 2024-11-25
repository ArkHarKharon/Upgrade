#include "IMainGame.hpp"
#include "Timing.hpp"
#include "ScreenList.hpp"
#include "IGameScreen.hpp"

namespace MyEngine
{
	
	IMainGame::IMainGame()
	{
		m_screen_list = new ScreenList(this);
	}

	IMainGame::~IMainGame()
	{

	}

	void IMainGame::run()
	{
		if (!init())
			return;

		FPS_Limiter m_limiter;
		m_limiter.set_max_fps(120.0f);

		m_is_running = true;

		while (m_is_running)
		{
			m_limiter.frame_begin();

			m_input_manager.update();
			update();
			
			if(m_is_running)
				draw();





			m_fps = m_limiter.frame_end();
			m_window.swap_buffer();

		}
	}

	bool IMainGame::init()
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		setlocale(0, "");

		if(!init_system())
			return false;

		on_init();
		add_screens();

		m_current_screen = m_screen_list->get_current_screen();
		m_current_screen->on_entry();
		m_current_screen->set_running();




		return true;
	}

	bool IMainGame::init_system()
	{

		m_window.create("Default", 1920, 1080, FULLSCREEN);

		return true;
	}

	void IMainGame::exit_game()
	{
		m_current_screen->on_exit();
		if (m_screen_list)
			m_screen_list->destroy();

		m_is_running = false;


	}

	void IMainGame::update()
	{
		if (m_current_screen)
		{
			switch (m_current_screen->get_current_state())
			{
			case ScreenState::RUNNING:
				m_current_screen->update();
				break;

			case(ScreenState::CHANGE_NEXT):
				m_current_screen->on_exit();
				m_current_screen = m_screen_list->move_next();

				if (m_current_screen)
				{
					m_current_screen->set_running();
					m_current_screen->on_entry();
				}
				break;

			case(ScreenState::CHANGE_PREV):
				m_current_screen->on_exit();
				m_current_screen = m_screen_list->move_prev();

				if (m_current_screen)
				{
					m_current_screen->set_running();
					m_current_screen->on_entry();
				}
				break;

			case(ScreenState::EXIT):
				exit_game();
				break;

			default:
				break;
			}
		}
		else
		{
			exit_game();
		}
	}

	void IMainGame::draw()
	{
		glViewport(0, 0,m_window.get_width(), m_window.get_height());

		if (m_current_screen and m_current_screen->get_current_state() == ScreenState::RUNNING)
		{
			m_current_screen->draw();
		}
	}

	void IMainGame::on_SDL_event(SDL_Event& event)
	{
		switch (event.type)
		{
		case(SDL_QUIT):
			exit_game();
			break;

		case(SDL_MOUSEMOTION):
			m_input_manager.set_mouse_coords((float)event.motion.x, (float)event.motion.y);
			break;

		case(SDL_MOUSEBUTTONDOWN):
			m_input_manager.press_key(event.button.button);
			break;

		case(SDL_MOUSEBUTTONUP):
			m_input_manager.release_key(event.button.button);
			break;

		case(SDL_KEYDOWN):
			m_input_manager.press_key(event.key.keysym.sym);
			break;

		case(SDL_KEYUP):
			m_input_manager.release_key(event.key.keysym.sym);
			break;
		}
	}

}