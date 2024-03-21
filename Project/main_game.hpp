#include <SDL.h>
#include <glew.h>

enum class GameState
{
	PLAY,
	EXIT,
};

class Game
{
	SDL_Window* m_window;
	GameState current_state;

	int m_WIDTH;
	int m_HEIGHT;


	void init_system(); 
	void process_input();
	void game_loop();
	void draw_game();

public:
	Game();
	~Game();

	void run();
};