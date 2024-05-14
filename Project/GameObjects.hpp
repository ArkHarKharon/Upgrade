#pragma once

#include <SDL.h>
#include  <glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <MyEngine/ResourceManager.hpp>
#include <MyEngine/SpriteBatch.hpp>
#include <MyEngine/Errors.hpp>
#include <MyEngine/GLTexture.hpp>
#include <MyEngine/InputManager.hpp>
#include <MyEngine/Camera2D.hpp>

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
	float m_tile_size;

	MyEngine::SpriteBatch m_sprite_batch;

	std::vector<std::string> m_level_data;
	GLuint m_texture_id;

	glm::ivec2 m_start_position;
	glm::ivec2 m_enemy_position;

public:
	Level(const std::string& level_file);
	~Level();

	 glm::ivec2 const get_start_pos();
	 glm::ivec2 const get_enemy_pos();
	std::vector<std::string>& get_level_data();
	

	void draw();
};






class Tank 
{
protected:
	MyEngine::GLTexture m_texture;
	MyEngine::GLTexture m_turret_texture;

	bool m_control; //Можно ли управлять этим танком

	int m_hp;
	float m_speed;
	int m_damage;
	int m_fire_rate; // Через сколько кадров стреляем
	float m_projectile_speed;
	float m_accuracy; //Максимальный угол отклонения снарядов от башни

	int m_ammo_max;
	int m_ammo_current;
	int m_reload_time; //Кол-во кдров, которые занимает перезарядка

	float m_turret_angle;
	float m_turret_speed;

	glm::vec2 m_position;
	float m_angle;

	int m_tank_size;

	MyEngine::Camera2D m_camera;

	int m_frame_counter;
	int m_reload_frame_counter;

	void check_tile_pos(const std::vector<std::string>& level_data, std::vector<glm::vec2>& collide_tile_pos, float x, float y);
	void collide_with_tile(glm::vec2 tile_position);

	void fire(glm::vec2 barrel_pos, glm::vec2 direction, std::vector<Projectile>& bullets);
	bool reload();

	void move_1(MyEngine::InputManager& input_manager);
	void turret_rotate_1(MyEngine::InputManager& input_manager);
	void shoot_1(std::vector<Projectile>& bullets, MyEngine::InputManager& input_manager);

	void move_2(MyEngine::InputManager& input_manager);
	void turret_rotate_2(MyEngine::InputManager& input_manager);
	void shoot_2(std::vector<Projectile>& bullets, MyEngine::InputManager& input_manager);


	const struct Direction
	{
		float up = 0;
		float right = 4.71;
		float left = 1.57;
		float down = 3.14;
	}m_direction;

public:
	Tank();
	~Tank();

	void init(bool control, int hp, int damage, float speed, int ammo_max, int reload_time, float turret_speed, int fire_rate, float projectile_speed ,float accuracy,  glm::vec2 position, std::string tank_filepath, std::string turret_filepath);

	glm::vec2 get_position();
	int get_size();

	void draw(MyEngine::SpriteBatch& sprite_batch);

	bool update(MyEngine::InputManager input_manager, const std::vector<std::string>& level_data, std::vector <Tank*> tanks, std::vector<Projectile>& projectiles);

	void collide_with_level(const std::vector<std::string>& level_data);

	bool rotate(float dest_angle);

	bool is_controlable();

	void apply_damage(int damage);
};







class Projectile 
{
	float m_speed;
	glm::vec2 m_direction;
	glm::vec2 m_start_position;
	glm::vec2 m_position;
	int m_damage;

	int m_projectile_size;

	int m_lifetime;

public:
	Projectile(glm::vec2 position, float projectile_size, glm::vec2 direction, float speed, int damage);
	~Projectile();

	void draw(MyEngine::SpriteBatch& sprite_batch);

	bool update(std::vector<Tank*> tanks, const std::vector<std::string>& level_data);

	bool collide_with_tanks(std::vector<Tank*> tanks);
	bool collide_with_level(const std::vector<std::string>& level_data);
};




class GameManager
{
	int m_current_round;
	int m_times_blue_won;
	int m_times_red_won;

	bool m_need_new_round;
	
public:
	GameManager();

	std::pair<int, int> get_times_won();
	int get_current_round();

	bool get_need_round();
	void set_need_round(bool need_or_not);

	void start_new_session(std::vector <Tank*> tanks);
	void session_control();

	void increm_victory_score(bool control);
	void reload_message(bool control);
};



