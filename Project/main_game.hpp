#include <SDL.h>
#include <glew.h>

class Game
{
	int m_HEIGHT;
	int m_WIDTH;

	SDL_Window* m_window;

public:
	void run();
	void init_system();
};