#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <MyEngine/ResourceManager.hpp>
#include <MyEngine/SpriteBatch.hpp>
#include <MyEngine/Errors.hpp>
#include <MyEngine/GLTexture.hpp>
#include <MyEngine/InputManager.hpp>
#include <MyEngine/Camera2D.hpp>
#include <MyEngine/AudioManager.hpp>
#include <MyEngine/Window.hpp>


#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <ctime>

class Projectile;


class Level
{
protected:

    // Информация об уровне
    float m_tile_size;                            // Размер тайла уровня
    glm::ivec2 m_start_position;                  // Координаты начальной позиции игрока (в тайлах)
    std::vector<glm::ivec2> m_enemy_positions;    // Вектор начальных позиций ботов (в тайлах)
    std::vector<glm::ivec2> m_knots;              // Вектор узлов ботов(в тайлах)



    // Объекты модулей приложения
    MyEngine::SpriteBatch m_sprite_batch;        // Модуль, отвечающий за отрисовку уровня



    // Дайнные из файла уровня
    std::vector<std::string> m_level_data;



    // id текстуры "подложки" уровня
    GLuint m_texture_id;


public:
    // Конструкторы и деструкторы
    Level(const std::string& level_file);
    ~Level();



    // Геттеры
    glm::ivec2 const get_start_pos();
    glm::ivec2 get_enemy_pos();
    std::vector<std::string>& get_level_data();
    int get_random_int(int min, int max);

    // Функция отрисовки уровня
    void draw();
};






class Tank
{
protected:
    // Управляющие переменные
    bool m_control;    //Является ли этот танк игроком



    //Игровые характеристики танка
    int m_hp;          // Текущий уровень HP
    int m_max_hp;        // Максимальный уровень HP
    float m_speed;        // Скорость танка
    float m_turret_speed;    // Скорость вращения башни танка
    int m_damage;        // Урон, наносимый танком
    int m_fire_rate;      // Через сколько кадров стреляем
    float m_projectile_speed;  // Скорость снаряда
    float m_accuracy;      // Максимальный угол отклонения снарядов от башни
    int m_ammo_max;        // Максимальный боезапас
    int m_ammo_current;      // Текущий боезапас
    int m_reload_time;      // Кол-во кдров, которые занимает перезарядка



    // Позиционирование танка
    float m_turret_angle;            // Угол наклона башни от вертикали (в радианах)
    glm::vec2 m_position;            // Текущая позиция танка
    glm::ivec2 m_tank_pos;            // Текущая позиция танка (в тайлах)
    float m_angle;                // Текущий угол наклона корруса танка от вертикали
    int m_tank_size;              // Размер танка



    // Объекты модулей приложения
    MyEngine::Camera2D m_camera;      // Модуль камеры



    // Текстуры
    MyEngine::GLTexture m_texture;      // Текстура корпуса танка
    MyEngine::GLTexture m_turret_texture;   // Текстура башни танка
    MyEngine::GLTexture m_hp_texture;    // Текстура полоски HP 
    MyEngine::GLTexture m_ammo_texture;    // Текстура полоски боезапаса



    // Звуковые эффекты
    MyEngine::SoundEffect m_shoot_effect;  // Звуковой эффект выстрела
    MyEngine::SoundEffect m_death_effect;   // Звуковой эффект при уничтожении танка



    // Счетчики кадров
    int m_frame_counter;      // Счетчик кадров для определения FPS
    int m_reload_frame_counter;    // Счетчик кадров для контроля времени перезарядки



    // Структура с узловыми значениями углов (для поворота танка)
    const struct Direction
    {
        float up = 0;
        float right = 4.71;
        float left = 1.57;
        float down = 3.14;
    }m_direction;

    // Вектор снарядов, связанных с танком
    std::vector<Projectile> m_projectiles;


    // Функции, отвечающие за коллизии танка
    void check_tile_pos(const std::vector<std::string>& level_data, std::vector<glm::vec2>& collide_tile_pos, float x, float y);
    void collide_with_tile(glm::vec2 tile_position);



    //Функции, отвечающие за стрельбу и перезарядку
    void fire(glm::vec2 barrel_pos, glm::vec2 direction);
    bool reload(MyEngine::InputManager& input_manager);


    // Функции, содержащие алгоритмы, определяющие поведение танка
    void virtual move(MyEngine::InputManager& input_manager, const std::vector<std::string>& level_data) = 0;
    void virtual turret_rotate(MyEngine::InputManager& input_manager, Tank* player) = 0;
    void virtual shoot(MyEngine::InputManager& input_manager, std::vector <Tank*> tanks, const std::vector<std::string>& level_data) = 0;

public:
    // Конструкторы и деструкторы
    Tank(bool control, int hp, int damage, float speed, int ammo_max,
        int reload_time, float turret_speed, int fire_rate, float projectile_speed,
        float accuracy, glm::vec2 start_position, std::string tank_filepath,
        std::string turret_filepath, std::string hp_filepath, std::string ammo_filepath,
        MyEngine::SoundEffect fire_effect, MyEngine::SoundEffect death_effect);

    Tank();
    ~Tank();



    // Функция инициализации переменных танка
    void init(bool control, int hp, int damage, float speed, int ammo_max,
        int reload_time, float turret_speed, int fire_rate, float projectile_speed,
        float accuracy, glm::vec2 position, std::string tank_filepath,
        std::string turret_filepath, std::string hp_filepath, std::string ammo_filepath,
        MyEngine::SoundEffect fire_effect, MyEngine::SoundEffect death_effect);



    // Функция покадрового обновления танка (возвращает true, если танк уничтожен)
    bool virtual update(MyEngine::InputManager input_manager, const std::vector<std::string>& level_data, std::vector <Tank*> tanks) = 0;



    // Функция, отвечающая за отрисовку танка
    void draw(MyEngine::SpriteBatch& sprite_batch);



    // Функция, отвечающая за коллизию танка и уровня
    void collide_with_level(const std::vector<std::string>& level_data);



    // Функция, отвечающая за плавный поворот танка
    bool rotate(float dest_angle);



    // Геттеры
    bool is_controlable();
    glm::vec2 get_position();
    int get_size();



    // Сеттеры
    void apply_damage(int damage);
};





class PlayerTank : public Tank
{
protected:
    // Функции, содержащие алгоритмы, определяющие поведение танка
    void virtual move(MyEngine::InputManager& input_manager, const std::vector<std::string>& level_data) override;
    void virtual turret_rotate(MyEngine::InputManager& input_manager, Tank* player) override;
    void virtual shoot(MyEngine::InputManager& input_manager, std::vector <Tank*> tanks, const std::vector<std::string>& level_data) override;


public:
    PlayerTank() {};
    PlayerTank(bool control, int hp, int damage, float speed, int ammo_max,
        int reload_time, float turret_speed, int fire_rate, float projectile_speed,
        float accuracy, glm::vec2 start_position, std::string tank_filepath,
        std::string turret_filepath, std::string hp_filepath, std::string ammo_filepath,
        MyEngine::SoundEffect fire_effect, MyEngine::SoundEffect death_effect) :

        Tank(control, hp, damage, speed, ammo_max, reload_time, turret_speed, fire_rate, projectile_speed,
            accuracy, start_position, tank_filepath, turret_filepath, hp_filepath, ammo_filepath, fire_effect, death_effect) {}


    // Функция покадрового обновления танка (возвращает true, если танк уничтожен)
    bool virtual update(MyEngine::InputManager input_manager, const std::vector<std::string>& level_data, std::vector <Tank*> tanks) override;

};






class BotTank : public Tank
{
protected:

    // Переменные, отвечающие за направление движения бота
    int m_current_direction;          // Текущее направление движения бота по emun Directions
    std::vector<int> m_possible_directions;    // Вектор возможных направлений движения бота по enum Directions



    // Переменные, отвечающие за стрельбу ботов
    bool m_test_in_progress;          // Пристрелочный выстрел в процессе




    // Enum для перемещения ботов
    enum DIRECTIONS
    {
        NONE, UP, DOWN, LEFT, RIGHT
    };


    // Функции, содержащие алгоритмы, определяющие поведение танка
    void virtual move(MyEngine::InputManager& input_manager, const std::vector<std::string>& level_data) override;
    void virtual turret_rotate(MyEngine::InputManager& input_manager, Tank* player) override;
    void virtual shoot(MyEngine::InputManager& input_manager, std::vector <Tank*> tanks, const std::vector<std::string>& level_data) override;
    bool test_shot(glm::vec2 barrel_pos, glm::vec2 direction, std::vector <Tank*> tanks, const std::vector<std::string>& level_data);

public:
    // Конструкторы и деструкторы
    BotTank() {}
    BotTank(bool control, int hp, int damage, float speed, int ammo_max,
        int reload_time, float turret_speed, int fire_rate, float projectile_speed,
        float accuracy, glm::vec2 start_position, std::string tank_filepath,
        std::string turret_filepath, std::string hp_filepath, std::string ammo_filepath,
        MyEngine::SoundEffect fire_effect, MyEngine::SoundEffect death_effect) :

        Tank(control, hp, damage, speed, ammo_max, reload_time, turret_speed, fire_rate, projectile_speed,
            accuracy, start_position, tank_filepath, turret_filepath, hp_filepath, ammo_filepath, fire_effect, death_effect), m_test_in_progress{ false } {}


    // Функция покадрового обновления танка (возвращает true, если танк уничтожен)
    bool virtual update(MyEngine::InputManager input_manager, const std::vector<std::string>& level_data, std::vector <Tank*> tanks) override;

    // Геттеры и сеттеры



};






class Projectile
{
protected:
    // Управляющие переменные
    bool m_is_bot_sender;    // Является ли бот отправителем снаряда
    bool m_is_test_projectile;  // Является ли снаряд пристрелочным



    // Характеристики сняряда
    float m_speed;          // Скорость снаряда
    glm::vec2 m_direction;      // Точка, в направлении которой летит снаряд
    glm::vec2 m_start_position;    // Точка, откуда был выпущен снаряд
    glm::vec2 m_position;      // Текущая позиция снаряда
    int m_damage;          // Урон, который нанесёт снаряд при попадании
    int m_projectile_size;      // Размер снаряда
    int m_lifetime;          // Текущее время жизни снаряда

public:
    // Конструкторы и деструкторы
    Projectile(glm::vec2 position, float projectile_size, glm::vec2 direction, float speed, int damage, bool bot_sender, bool test = false);
    ~Projectile();



    // Функция покадрового обновления снаряда (возвращает true, если снаряд уничтожен) 
    bool update(std::vector<Tank*> tanks, const std::vector<std::string>& level_data);



    // Фукция покадрового обновления сняряда
    void draw(MyEngine::SpriteBatch& sprite_batch);



    // Функции, отвечающие за коллизию снаряда
    bool collide_with_tanks(std::vector<Tank*> tanks);
    bool collide_with_level(const std::vector<std::string>& level_data);

    // Геттеры и сеттеры
    int get_lifetime() { return m_lifetime; }
    bool is_test() { return m_is_test_projectile; }
};





class GameManager
{
private:
    unsigned int m_frame_count;

    bool m_is_initialized;
    bool m_is_session_started;
    bool m_is_round_started;

    bool m_is_player_dead;

    short int m_max_player_lives;
    short int m_current_player_lives;

    unsigned int m_score;
    short int m_difficulty;
    unsigned int m_bots_killed;

    float m_bot_strengh_scale;

    void start_round(std::vector<BotTank*>& bots, std::vector<Tank*>& tanks, std::vector <Level*>& levels, MyEngine::AudioManager& audio_manager, bool bot_spawn = true);
    void start_session(std::vector<BotTank*>& bots, std::vector<Tank*>& tanks, std::vector <Level*>& levels, MyEngine::AudioManager& audio_manager);
    void end_game(std::vector<BotTank*>& bots, std::vector<Tank*>& tanks, std::vector <Level*>& levels, MyEngine::AudioManager& audio_manager);

    void bot_number_control(std::vector<BotTank*>& bots, std::vector<Tank*>& tanks, std::vector <Level*>& levels, MyEngine::AudioManager& audio_manager);
    void spawn_bot(std::vector<BotTank*>& bots, std::vector<Tank*>& tanks, std::vector <Level*>& levels, MyEngine::AudioManager& audio_manager);

public:
    GameManager();


    enum Difficulty
    {
        DEBUG1, EASY, NORMAL, HARD, IMPOSIBLE
    };



    void session_control(std::vector<BotTank*>& bots, std::vector<Tank*>& tanks, std::vector <Level*>& levels, MyEngine::AudioManager& audio_manager, MyEngine::Window& window);

    void session_control_init_setting();

    void increase_score();
    void handle_player_death(std::vector<BotTank*>& bots, std::vector<Tank*>& tanks, std::vector <Level*>& levels, MyEngine::AudioManager& audio_manager);
    void end_screen(std::vector<BotTank*>& bots, std::vector<Tank*>& tanks, std::vector <Level*>& levels, MyEngine::AudioManager& audio_manager);


};
