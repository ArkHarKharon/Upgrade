#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>

#include <MyEngine/Sprite.hpp>
#include <MyEngine/GLSL.hpp>
#include <MyEngine/GLTexture.hpp>
#include <MyEngine/Camera2D.hpp>
#include <MyEngine/ImageLoader.hpp>
#include <MyEngine/Errors.hpp>
#include <MyEngine/Window.hpp>
#include <MyEngine/SpriteBatch.hpp>
#include <MyEngine/ResourceManager.hpp>
#include <MyEngine/InputManager.hpp>
#include <MyEngine/Timing.hpp>
#include <MyEngine/AudioManager.hpp>

#include "GameObjects.hpp"

#include <vector>
#include <iostream>
#include <string>
#include <array>
#include <Windows.h>


enum class GameState
{
    MAIN_MENU,
    UPGRADE_MENU,
    GAME,
    EXIT,
};

class Game
{
protected:
    // Управляющие переменные
    GameState m_current_state;    // Текущее сотояние игры из enum class GameState
    bool m_game_is_started;      // Началась ли игра
    float m_time;          // Кол-во кадров с начала игры



    // Объекты модулей приложения и связанные переменные
    MyEngine::Window m_window;        // Модуль окна игры
    float m_window_width;          // Ширина окна
    float m_window_height;          // Высота окна

    MyEngine::FPS_Limiter m_fps_limiter;  // Ограничитель FPS
    int m_fps;                // Текущее значение FPS
    int m_max_fps;              // Максимальное значение FPS

    MyEngine::Camera2D m_camera;      // Модуль камеры
    float m_cam_speed;            // Скорость камеры
    float m_cam_scale;            // Увеличение камеры

    MyEngine::GLSLProgram m_color_program;      // Модуль шейдеров
    MyEngine::SpriteBatch m_tank_sprite_batch;    // Модуль отрисовки танков
    MyEngine::InputManager m_input_manager;      // Менеджер ввода
    MyEngine::AudioManager m_audio_manager;      // Менеджер аудио




    // Игровые объекты
    std::vector <Level*> m_levels;      // Вектор карт уровней игры
    PlayerTank* m_player;          // Танк игрока
    std::vector<BotTank*> m_bots;      // Вектор танков ботов
    unsigned int m_bots_counter;      // Счетчик ботов
    std::vector<Tank*> m_tanks;        // Вектор всех танков игры
    GameManager m_game_manager;



    // Функции инициализации модулей приложения
    void init_system();      // Функция инициализации всех систем
    void init_shaders();    // Функция инициализации шейдеров
    void init_level();      // Функция инициализации уровня



    // Функция обработки ввода
    void process_input();



    // Функция начала игрового раунда
    void start_round();    // Функция начала нового раунда



    // Функция игровой петли
    void game_loop();



    // Функция отрисовки игры
    void draw_game();

public:
    // Конструкторы и деструкторы
    Game();
    ~Game();



    // Точка входа в приложение
    void run();
};
