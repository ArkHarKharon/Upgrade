#include "Window.hpp"
#include "Errors.hpp"

namespace MyEngine
{

	Window::Window()
	{

	}

	Window::~Window()
	{

	}

	int Window::create(std::string window_name, int screen_width, int screen_height, unsigned int flag)
	{
		Uint32 flags = SDL_WINDOW_OPENGL;

		if (flag & INVISIBLE)
			flags |= SDL_WINDOW_HIDDEN;

		else if (flag & FULLSCREEN)
			flags |= SDL_WINDOW_FULLSCREEN;

		else if (flag & BORDERLESS)
			flags |= SDL_WINDOW_BORDERLESS;

		m_sdl_window = SDL_CreateWindow(window_name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, flags);
		if (m_sdl_window == nullptr)
			fatal_error("Окно НЕ было открыто!");

		SDL_GLContext gl_context = SDL_GL_CreateContext(m_sdl_window);
		if (gl_context == nullptr)
			fatal_error("Контекст отрисовки OpenGl НЕ был создан!");

		GLenum error = glewInit();
		if (error != GLEW_OK)
			fatal_error("Glew НЕ был инициализирован!");


		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

		SDL_GL_SetSwapInterval(0);

		return 0;
	}

	int Window::get_width()
	{
		return m_screen_width;
	}

	int Window::get_height()
	{
		return m_screen_height;
	}

	void Window::swap_buffer()
	{
		SDL_GL_SwapWindow(m_sdl_window);

	}

}