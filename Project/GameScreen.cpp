#include "GameScreen.hpp"

#include <iostream>

GameplayScreen::GameplayScreen(MyEngine::Window* window) :  m_window{window}

{
    
}

GameplayScreen::~GameplayScreen()
{

}

void GameplayScreen::build()
{
    m_time = 0.0f;
    m_fps = 0.0f;
    m_max_fps = 0.0f;
    m_cam_speed = 0.0f;
    m_cam_scale = 0.0f;
    m_game_is_started = false;
    m_window_height = m_window->get_height();
    m_window_width = m_window->get_width();

    m_camera.init_camera(m_window_width, m_window_height);
}
void GameplayScreen::destroy()
{

}

void GameplayScreen::on_entry()
{
    init_system();
}

void GameplayScreen::on_exit()
{

}

void GameplayScreen::update()
{
    m_camera.update();

	process_input();
    m_game_manager.session_control(m_bots, m_tanks, m_levels, m_audio_manager, *m_window);

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

   
}



void GameplayScreen::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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

    //m_gui.draw();


}

int GameplayScreen::get_next_screen_index() const
{
	return SCREEN_INDEX_NONE;
}
int GameplayScreen::get_prev_screen_index() const
{
	return SCREEN_INDEX_NONE;
}

void  GameplayScreen::process_input()
{
	SDL_Event evnt;

	while (SDL_PollEvent(&evnt))
	{
		m_game->on_SDL_event(evnt);


	}
}


void GameplayScreen::init_system()
{
    setlocale(0, "");
    system("chcp1251");
    system("cls");
    srand(time(0));

    //init_GUI();
    init_shaders();

    m_tank_sprite_batch.init();

    m_fps_limiter.init(m_max_fps);

    m_audio_manager.init();

    init_level();

    MyEngine::Music music = m_audio_manager.load_music("Data/Sound/theme.mp3");
    music.play();
    Mix_Volume(-1, 20);

    m_camera.set_position(glm::vec2(m_window_width + 20, m_window_height));
    m_camera.set_scale(0.49f);
}

void GameplayScreen::init_shaders()
{
    m_color_program.compile_shaders("Shaders/vertex_shader.txt", "Shaders/fragment_shader.txt");
    m_color_program.add_attribute("vertexPosition");
    m_color_program.add_attribute("vertexColor");
    m_color_program.add_attribute("vertexUV");
    m_color_program.link_shaders();

}
void GameplayScreen::init_level()
{
    m_levels.push_back(new Level("Data/Level.txt"));
    m_levels.push_back(new Level("Data/Level1.txt"));

}

//void GameplayScreen::init_GUI()
//{
//    m_gui.init("C:/Users/ArkHarKharon/source/repos/ArkHarKharon/Upgrade/GUI");
//    m_gui.load_scheme("TaharezLook.scheme");
//    m_gui.set_mouse_cursor("TaharezLook/MouseArrow");
//    m_gui.show_mouse_cursor();
//    SDL_ShowCursor(0);
//    m_gui.set_font("DejaVuSans-10");
//
//    CEGUI::PushButton* button = static_cast<CEGUI::PushButton*>(m_gui.create_widget("TaharezLook/Button", glm::vec4(0.5f, 0.5f, 0.1f, 0.05f), glm::vec4(0.0f), "button"));
//    button->setText("EXIT SUKAAAAA!!!!");
//
//    button->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameplayScreen::exit_clicked, this));
//}

//bool GameplayScreen::exit_clicked(const CEGUI::EventArgs& e)
//{
//    m_current_state = MyEngine::ScreenState::NONE;
//
//    return true;
//}
