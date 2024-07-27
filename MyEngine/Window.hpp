#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <string>

namespace MyEngine
{
	enum WindowFlag
	{	

		INVISIBLE = 0x1,
		FULLSCREEN = 0x2,
		BORDERLESS = 0x4,
	};


	class Window
	{
		SDL_Window* m_sdl_window;
		int m_screen_width;
		int m_screen_height;
	public:
		Window();
		~Window();

		int create(std::string window_name, int screen_width, int screen_height, unsigned int flag = SDL_WINDOW_OPENGL);
		int get_width();
		int get_height();

		void swap_buffer();
	};
}