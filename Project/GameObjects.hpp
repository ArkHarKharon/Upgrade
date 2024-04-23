#pragma once

#include <SDL.h>
#include  <glew.h>
#include <glm/glm.hpp>

#include <MyEngine/ResourceManager.hpp>
#include <MyEngine/SpriteBatch.hpp>
#include <MyEngine/Errors.hpp>
#include <MyEngine/GLTexture.hpp>
#include <MyEngine/InputManager.hpp>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>


enum AnglePosition
{
	UP,
	DOWN,
	RIGHT,
	LEFT
};


class GameObject
{

};






class Level 
{
	float m_tile_size;

	MyEngine::SpriteBatch m_sprite_batch;
	MyEngine::ResourceManager* m_resource_manager;

	std::vector<std::string> m_level_data;
	GLuint m_texture_id;

	glm::ivec2 m_start_position;
	std::vector<glm::ivec2> m_enemy_start_position;

public:
	Level(const std::string& level_file);
	~Level();

	 glm::ivec2 const get_start_pos();
	const std::vector<glm::ivec2>&  get_enemies_pos();
	std::vector<std::string>& get_level_data();
	

	void draw();
};






class Tank 
{
protected:
	MyEngine::GLTexture m_texture;

	int m_hp;
	float m_speed;
	int damage;

	AnglePosition m_angle;
	glm::vec2 m_position;

	int m_tank_size;


public:
	Tank();
	~Tank(); //

	void init(float speed, glm::vec2 position,std::string filepath);

	void draw(MyEngine::SpriteBatch& sprite_batch);

	bool update(MyEngine::InputManager input_manager,
				const std::vector<std::string>& level_data,
				std::vector <Tank*> tanks);//

	void check_tile_pos(const std::vector<std::string>& level_data, 
		std::vector<glm::vec2>& collide_tile_pos,float x, float y);

	void collide_with_tile(glm::vec2 tile_position);

	void collide_with_level(const std::vector<std::string>& level_data);
};






class PlayerTank : public Tank
{
	
public:
	PlayerTank();
	~PlayerTank();

};






class EnemyTank : public Tank
{

};






class Projectile 
{
	float m_speed;
	glm::vec2 m_direction;
	glm::vec2 m_position;
	int m_lifetime;
public:
	Projectile(glm::vec2 position, glm::vec2 direction, float speed, int lifetime);
	~Projectile();

	void draw(MyEngine::SpriteBatch& sprite_batch);

	bool update();
};



