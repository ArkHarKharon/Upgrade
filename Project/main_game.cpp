#include "main_game.hpp"


Game::Game() :
	m_window_width{800 }, m_window_height{800 },
	m_current_state{ GameState::PLAY },
	m_time{ 0.0f },
	m_fps{ 0 },
	m_frame_time{ 0 },
	m_max_fps{0.0f }
{
	m_camera.init_camera(m_window_height, m_window_width);

}

Game::~Game()
{

}

void Game::run()
{
	init_system();

	m_sprites.resize(2);
	m_sprites.at(0).init(100.0f,100.0f,100.0f,100.0f, "Textures/tankBlue.png");
	m_sprites.at(1).init(10000.0f,250.0f,250.0f,250.0f, "Textures/tankRed.png");

	//m_tank_texture = ImageLoader::loadPNG("Textures/tankBlue.png");

	game_loop();

}

void Game::init_shaders()
{
	m_color_program.compile_shaders("Shaders/vertex_shader.txt","Shaders/fragment_shader.txt");
	m_color_program.add_attribute("vertexPosition");
	m_color_program.add_attribute("vertexColor");
	m_color_program.add_attribute("vertexUV");
	m_color_program.link_shaders();
}

void Game::init_system()
{
	setlocale(0, "");

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	m_window.create("CHLENOZAVR", m_window_width, m_window_height, 0);

	init_shaders();
}

void Game::process_input()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case(SDL_QUIT):
			m_current_state = GameState::EXIT;
			break;

		case(SDL_MOUSEMOTION):
			//std::cout << event.motion.x << " " << event.motion.y << "\n";
			break;

		case(SDL_MOUSEBUTTONDOWN):
			//std::cout << event.button.timestamp << "\n";
			//std::cout << event.button.x << " " << event.button.x << "\n";
			break;

		case(SDL_KEYDOWN):
			switch (event.key.keysym.sym)
			{
			case(SDLK_w):
				m_camera.set_position(m_camera.get_position() + glm::vec2(0.0, -20.0));
				break;

			case(SDLK_s):
				m_camera.set_position(m_camera.get_position() + glm::vec2(0.0, +20.0));
				break;

			case(SDLK_a):
				m_camera.set_position(m_camera.get_position() + glm::vec2(20.0, 0.0));
				break;
			
			case(SDLK_d):
				m_camera.set_position(m_camera.get_position() + glm::vec2(-20.0, 0.0));
				break;
			}
			break;

		case(SDL_MOUSEWHEEL):
			if (event.wheel.y > 0)
				m_camera.set_scale(m_camera.get_scale() + 0.1);
			else if (event.wheel.y < 0 and m_camera.get_scale() > 0)
				m_camera.set_scale(m_camera.get_scale() - 0.1);
			break;
		
		}
	};
}

void Game::game_loop()
{
	while (m_current_state != GameState::EXIT)
	{
		float start_ticks = SDL_GetTicks();

		process_input();
		m_time += 0.01;
		m_camera.update();

		draw_game();

		fps_counter(10);
		std::cout << m_fps << std::endl;


		float frame_ticks = SDL_GetTicks() - start_ticks;
		if (1000.0f / m_max_fps > frame_ticks)
			SDL_Delay(1000.0f / m_max_fps - frame_ticks);

	}
}

void Game::draw_game()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_color_program.use();

	glActiveTexture(GL_TEXTURE0);
	GLint texture_location = m_color_program.get_uniform_location("tank_texture");
	glUniform1i(texture_location, 0);

	//GLint time_location = m_color_program.get_uniform_location("time");
	//glUniform1f(time_location, m_time);

	GLint p_location = m_color_program.get_uniform_location("P");
	glm::mat4 cam_matrix = m_camera.get_cam_matrix();

	glUniformMatrix4fv(p_location,1, GL_FALSE, &(cam_matrix[0][0]));


	for(int i = 0; i < m_sprites.size(); i++)
		m_sprites.at(i).draw();

	glBindTexture(GL_TEXTURE_2D, 0);
	m_color_program.unuse();

	m_window.swap_buffer();
}

void Game::fps_counter(int samples_num)
{
	
	static std::vector<float> frame_times;
	frame_times.resize(samples_num);
	static int current_frame = 0;

	static float prev_ticks = SDL_GetTicks();

	float current_ticks;
	current_ticks = SDL_GetTicks(); //

	m_frame_time = current_ticks - prev_ticks;
	frame_times.at(current_frame % samples_num) = m_frame_time;

	prev_ticks = current_ticks;

	int count;
	current_frame++;

	if (current_frame < samples_num)
		count = current_frame;
	else count = samples_num;

	float averange_frame_time = 0;
	for (int i = 0; i < count; i++)
		averange_frame_time += frame_times.at(i);

	averange_frame_time /= count;
	if (averange_frame_time)
		m_fps = 1000.0f / averange_frame_time;
	else m_fps = 60.0f;

}