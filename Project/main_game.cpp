#include "main_game.hpp"

Game::Game() :
    m_window_width{ (float)25 * 32 }, m_window_height{ (float)13 * 32 },
    m_current_state{ GameState::GAME },
    m_time{ 0.0f },
    m_fps{ 0 },
    m_max_fps{ 0 },
    m_cam_speed{ 0.2f },
    m_cam_scale{ 0.0f },
    m_game_is_started{ false }
{
    m_camera.init_camera(m_window_width, m_window_height);
}

Game::~Game()
{
    for (int i = 0; i < m_levels.size(); i++)
        delete m_levels.at(i);
}

void Game::run()
{
    init_system();
    game_loop();
}

void Game::init_shaders()
{
    m_color_program.compile_shaders("Shaders/vertex_shader.txt", "Shaders/fragment_shader.txt");
    m_color_program.add_attribute("vertexPosition");
    m_color_program.add_attribute("vertexColor");
    m_color_program.add_attribute("vertexUV");
    m_color_program.link_shaders();
}

void Game::init_system()
{
    setlocale(0, "");
    srand(time(0));

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); //


    m_window.create("Upgrade!", m_window_width, m_window_height, MyEngine::WindowFlag::FULLSCREEN);

    init_shaders();

    m_tank_sprite_batch.init();

    m_fps_limiter.init(m_max_fps);

    m_audio_manager.init();

    init_level();

    MyEngine::Music music = m_audio_manager.load_music("Data/Sound/theme.mp3");
    music.play();
    Mix_Volume(-1, 20);

    m_camera.set_position(glm::vec2(64 * 12.5f, 64 * 6.5f));
    m_camera.set_scale(0.5f);

    //CEGUI::OpenGL3Renderer& my_renderer = CEGUI::OpenGL3Renderer::bootstrapSystem();
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
            m_input_manager.set_mouse_coords(event.motion.x, event.motion.y);
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
    };
}

void Game::game_loop()
{
    while (m_current_state != GameState::EXIT)
    {
        process_input();

        if (m_input_manager.key_is_pressed(SDLK_HOME))
        {
            m_game_manager.end_screen(m_bots, m_tanks, m_levels, m_audio_manager);
        }

        m_game_manager.session_control(m_bots, m_tanks, m_levels, m_audio_manager, m_window);

        for (int i = 0; i < m_tanks.size(); i++)
        {
            if (m_tanks.at(i)->update(m_input_manager, m_levels.at(0)->get_level_data(), m_tanks))
            {
                if (m_tanks.at(i)->is_controlable())
                    m_game_manager.handle_player_death(m_bots, m_tanks, m_levels, m_audio_manager);

                else m_game_manager.increase_score();

                m_tanks.at(i) = m_tanks.back();
                m_tanks.pop_back();
            }

        }

        m_camera.update();

        draw_game();
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

    glm::mat4 cam_matrix = m_camera.get_cam_matrix();
    GLint p_location = m_color_program.get_uniform_location("P");
    glUniformMatrix4fv(p_location, 1, GL_FALSE, &(cam_matrix[0][0]));

    m_tank_sprite_batch.begin();

    m_levels.at(0)->draw();


    for (int i = 0; i < m_tanks.size(); i++)
    {
        m_tanks.at(i)->draw(m_tank_sprite_batch);
    }

    m_tank_sprite_batch.end();

    m_tank_sprite_batch.render_batch();

    m_color_program.unuse();

    m_window.swap_buffer();
}

void Game::init_level()
{
    m_levels.push_back(new Level("Data/Level.txt"));
}

