#include "main_game.hpp"


Game::Game() :
	m_window_width{ 800 }, m_window_height{ 800 },
	m_current_state{ GameState::GAME },
	m_time{ 0.0f },
	m_fps{ 0 },
	m_max_fps{ 0 },
	m_cam_speed{0.2f},
	m_cam_scale{0.0f}
{
	m_camera.init_camera(m_window_height, m_window_width);
}

Game::~Game()
{
	for (int i = 0; i < m_levels.size(); i++)
		delete m_levels.at(i);
}

void Game::run()
{
	init_system();
	init_level();
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

	m_window.create("Upgrade!", m_window_width, m_window_height, SDL_WINDOW_OPENGL);

	init_shaders();


	m_tank_sprite_batch.init();

	m_fps_limiter.init(m_max_fps);
}

void Game::process_input()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		glm::vec2 mouse_coords = m_camera.get_world_coords(m_input_manager.get_mouse_coords());
		glm::vec2 player_pos(0.0f);
		glm::vec2 direction = mouse_coords - player_pos;
		direction = glm::normalize(direction);


		switch (event.type)
		{
		case(SDL_QUIT):
			m_current_state = GameState::EXIT;
			break;

		case(SDL_MOUSEMOTION):
			m_input_manager.set_mouse_coords(event.motion.x, event.motion.y);
			break;

		case(SDL_MOUSEBUTTONDOWN):

			m_projectiles.emplace_back(player_pos, direction, 1.0f, 3000);
			break;

		//case(SDL_MOUSEBUTTONUP):
			//m_input_manager.release_key(event.button.button);
			//break;


		case(SDL_KEYDOWN):
			m_input_manager.press_key(event.key.keysym.sym);
			break;

		case(SDL_KEYUP):
			m_input_manager.release_key(event.key.keysym.sym);
			break;

		case(SDL_MOUSEWHEEL):
			if (event.wheel.y > 0)
				m_camera.set_scale(m_camera.get_scale() + 0.1f);
			else if (event.wheel.y < 0 and m_camera.get_scale() > 0.1f)
				m_camera.set_scale(m_camera.get_scale() - 0.1f);
			break;
		
		}
	};
	if (m_input_manager.key_is_pressed(SDLK_UP))
		m_camera.set_position(m_camera.get_position() + glm::vec2(0.0, m_cam_speed));
	
	if (m_input_manager.key_is_pressed(SDLK_DOWN))
		m_camera.set_position(m_camera.get_position() + glm::vec2(0.0, -m_cam_speed));

	if (m_input_manager.key_is_pressed(SDLK_LEFT))
		m_camera.set_position(m_camera.get_position() + glm::vec2(-m_cam_speed, 0.0));

	if (m_input_manager.key_is_pressed(SDLK_RIGHT))
		m_camera.set_position(m_camera.get_position() + glm::vec2(m_cam_speed, 0.0));

	

}

void Game::game_loop()
{
	while (m_current_state != GameState::EXIT)
	{
		m_fps_limiter.frame_begin();
		process_input();
		
		for (int i = 0; i < m_tanks.size(); i++)
			m_tanks.at(i)->update(m_input_manager,m_levels.at(0)->get_level_data(), m_tanks);

		m_time += 0.01;
		m_camera.update();

		for (int i = 0; i < m_projectiles.size();)
		{
			if (m_projectiles.at(i).update() == true)
			{
				m_projectiles.at(i) = m_projectiles.back();
				m_projectiles.pop_back();
			}
			else i++;
		}
		draw_game();

		m_fps = m_fps_limiter.frame_end();

		static int frame_counter = 0;
		frame_counter++;
		if (frame_counter == 10000)
		{
			std::cout << m_fps << std::endl;
			frame_counter = 0;
		}
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
	GLint p_location = m_color_program.get_uniform_location("P");
	glm::mat4 cam_matrix = m_camera.get_cam_matrix();
	glUniformMatrix4fv(p_location,1, GL_FALSE, &(cam_matrix[0][0]));

	m_tank_sprite_batch.begin();

	m_levels.at(0)->draw();


	for (int i = 0; i < m_tanks.size(); i++)
	{
		m_tanks.at(i)->draw(m_tank_sprite_batch);
	}





	m_tank_sprite_batch.end();

	m_tank_sprite_batch.render_batch();

	glBindTexture(GL_TEXTURE_2D, 0);

	m_color_program.unuse();

	m_window.swap_buffer();
}

void Game::init_level()
{
	m_levels.emplace_back(new Level("Data/Level.txt"));

	m_player = new PlayerTank();
	m_player->init(0.1f, m_levels.at(0)->get_start_pos(), "C:\\Users\\User\\Desktop\\Code\\Visual\\SDL\\SDL\\Project\\Data\\Textures\\tankBlue.png");

	m_tanks.push_back(m_player);
}
