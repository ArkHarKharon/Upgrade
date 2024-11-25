#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <MyEngine/IGameScreen.hpp>
#include <MyEngine/SpriteBatch.hpp>
#include <MyEngine/GLSL.hpp>
#include <MyEngine/Camera2D.hpp>
#include <MyEngine/GLTexture.hpp>
#include <MyEngine/Window.hpp>
#include <MyEngine/IMainGame.hpp>
#include <MyEngine/InputManager.hpp>
#include <MyEngine/Timing.hpp>
#include <MyEngine/GUI.hpp>
#include <MyEngine/Sprite.hpp>
#include <MyEngine/ImageLoader.hpp>
#include <MyEngine/Errors.hpp>
#include <MyEngine/ResourceManager.hpp>
#include <MyEngine/AudioManager.hpp>


#include "GameObjects.hpp"

#include <vector>
#include <iostream>
#include <string>
#include <array>
#include <Windows.h>



class GameplayScreen : public MyEngine::IGameScreen
{
private:
    // Управляющие переменные
    bool m_game_is_started;       // Началась ли игра
    float m_time;                 // Кол-во кадров с начала игры

    // Объекты модулей приложения и связанные переменные
    MyEngine::Window* m_window;        // Модуль окна игры
    float m_window_width;             // Ширина окна
    float m_window_height;            // Высота окна

    MyEngine::FPS_Limiter m_fps_limiter;  // Ограничитель FPS
    int m_fps;                  // Текущее значение FPS
    int m_max_fps;              // Максимальное значение FPS

    MyEngine::Camera2D m_camera;      // Модуль камеры
    float m_cam_speed;            // Скорость камеры
    float m_cam_scale;            // Увеличение камеры

    MyEngine::GLSLProgram m_color_program;       // Модуль шейдеров
    MyEngine::SpriteBatch m_tank_sprite_batch;   // Модуль отрисовки танков
    MyEngine::InputManager m_input_manager;      // Менеджер ввода
    MyEngine::AudioManager m_audio_manager;      // Менеджер аудио
    MyEngine::GUI m_gui;                         // Модуль графического интерфейса



    // Игровые объекты
    std::vector <Level*> m_levels;      // Вектор карт уровней игры
    PlayerTank* m_player;               // Танк игрока
    std::vector<BotTank*> m_bots;       // Вектор танков ботов
    unsigned int m_bots_counter;        // Счетчик ботов
    std::vector<Tank*> m_tanks;         // Вектор всех танков игры
    GameManager m_game_manager;

    // Функции инициализации модулей приложения
    void init_system();      // Функция инициализации всех систем
    void init_shaders();     // Функция инициализации шейдеров
    void init_level();       // Функция инициализации уровня
    void init_GUI();

    // Функция обработки ввода
    void process_input();


    // Функции GUI
    bool exit_clicked(const CEGUI::EventArgs& e);


public:
	GameplayScreen(MyEngine::Window* window);
	~GameplayScreen();

	virtual void build() override;
	virtual void destroy() override;

	virtual void on_entry() override;
	virtual void on_exit() override;

	virtual void update() override;
	virtual void draw() override;

	virtual int get_next_screen_index() const override;
	virtual int get_prev_screen_index() const override;
};