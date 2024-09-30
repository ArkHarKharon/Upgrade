#include "GameObjects.hpp"

const int TILE_SIZE = 64;


Projectile::Projectile(glm::vec2 position, float projectile_size ,glm::vec2 direction, float speed, int damage) : m_lifetime{0}
{
	m_position = position;
	m_start_position = m_position;
	m_speed = speed;
	m_direction = direction;
	m_damage = damage;
	m_projectile_size = projectile_size;
}

Projectile::~Projectile()
{

}

void Projectile::draw(MyEngine::SpriteBatch& sprite_batch)
{
	MyEngine::Color color{0,255,0,255};
	glm::vec4 uv_pos(0.0f, 0.0f, 1.0f, 1.0f);
	glm::vec4 pos_size = glm::vec4(m_position.x, m_position.y, m_projectile_size, m_projectile_size);
	MyEngine::GLTexture bullet_texture = MyEngine::ResourceManager::get_texture("Data/Textures/bulletBlue.png");

	sprite_batch.draw(pos_size, uv_pos, bullet_texture.id, 1.0f, color);
}

bool Projectile::update(std::vector<Tank*> tanks, const std::vector<std::string>& level_data)
{
	m_position += m_direction * m_speed;
	m_lifetime++;

	if (collide_with_tanks(tanks))
		return true;

	if (collide_with_level(level_data) ) 
		return true;

	return false;
}

bool Projectile::collide_with_tanks(std::vector<Tank*> tanks)
{
	for (int i = 0; i < tanks.size(); i++)
	{
		glm::vec2 tank_pos = glm::vec2(tanks.at(i)->get_position().x + tanks.at(i)->get_size()/2, tanks.at(i)->get_position().y + tanks.at(i)->get_size() / 2);
		glm::vec2 projectile_pos = glm::vec2(m_position.x + m_projectile_size/2, m_position.y + m_projectile_size/2);

		int min_distance = (tanks.at(i)->get_size() / 2) + (m_projectile_size / 2);

		float dist_x = glm::abs(tank_pos.x - projectile_pos.x);
		float dist_y = glm::abs(tank_pos.y - projectile_pos.y);

		if (dist_x < min_distance and dist_y < min_distance)
		{
			if(m_lifetime > 150)
				tanks.at(i)->apply_damage(m_damage);
			return true;
		}
	}

	return false;
}

bool Projectile::collide_with_level(const std::vector<std::string>& level_data)
{
	glm::ivec2 grid_pos(floor(m_position.x / TILE_SIZE), floor(m_position.y / TILE_SIZE));

	if (level_data.at(grid_pos.y).at(grid_pos.x) == 'w' or level_data.at(grid_pos.y).at(grid_pos.x) == 'b')
		return true;

	else 
		return false;

}








Level::Level(const std::string& level_file) : m_tile_size{ TILE_SIZE }
{
	std::ifstream file;
	file.open(level_file);

	if (file.fail())
		MyEngine::fatal_error("���� " + level_file + " �� ������!");

	std::string temp;
	while (std::getline(file, temp))
		m_level_data.push_back(temp);

	m_sprite_batch.init();
	m_sprite_batch.begin();

	glm::vec4 uv_rect(0.0f,0.0f,1.0f, 1.0f);
	MyEngine::Color color{ 255,255,255,255 };

	glm::vec4 grass_dest_rect(0.0f, 0.0f, m_level_data.at(0).size() * TILE_SIZE, m_level_data.size() * TILE_SIZE);
	m_sprite_batch.draw(grass_dest_rect, uv_rect, MyEngine::ResourceManager::get_texture("Data/Textures/grass.png").id, 0.0f,color);

	for (int y = 0; y < m_level_data.size(); y++)
	{
		for (int x = 0; x < m_level_data.at(y).size(); x++)
		{
			char tile = m_level_data.at(y).at(x);
			glm::vec4 dest_rect(x * m_tile_size, y * m_tile_size, m_tile_size, m_tile_size);

			switch (tile)
			{
			case('b'):
				m_sprite_batch.draw(dest_rect, uv_rect,
					MyEngine::ResourceManager::get_texture("Data/Textures/blackBrick.png").id,
					0.0f, color);
				break;

			case('e'):
				m_enemy_positions.push_back(glm::ivec2(x * m_tile_size, y * m_tile_size));
				break;

			case(' '):
				break;

			case('z'):
				m_level_data.at(y).at(x) = ' ';
				break;

			case('s'):
				m_start_position = glm::ivec2(x * m_tile_size, y * m_tile_size);
				break;

			case('w'):
				m_sprite_batch.draw(dest_rect, uv_rect,
					MyEngine::ResourceManager::get_texture("Data/Textures/sandBrick.png").id,
					0.0f, color);
				break;

			default:
				std::cout << "�������� ������ � ����� ������: " << tile << "(" << x << ", " << y << ")\n";
				break;
			}
		}
	}

	m_sprite_batch.end();
}

Level::~Level()
{

}

void Level::draw()
{
	m_sprite_batch.render_batch();
}

const glm::ivec2 Level::get_start_pos()
{
	return m_start_position;
}

int Level::get_random_int(int min, int max)
{
	static std::mt19937 random_engine(time(0));
	std::uniform_real_distribution<float> pos_number(min, max);

	return pos_number(random_engine);
}

glm::ivec2 Level::get_enemy_pos()
{
	int current_pos_number = get_random_int(0, m_enemy_positions.size() - 1);
	std::cout << current_pos_number;
	return m_enemy_positions.at(current_pos_number);
}

std::vector<std::string>& Level::get_level_data()
{
	return m_level_data;
}








Tank::Tank() : m_tank_size{ 64 }, m_angle{ 0 }, m_turret_angle{ 0.0f }, m_possible_directions{}
{

}

Tank::~Tank()
{
	
}

bool Tank::is_controlable()
{
	return m_control;
}

void Tank::init(bool control, int hp, int damage, float speed, int ammo_max, int reload_time, float turret_speed, int fire_rate,float projectile_speed, float accuracy ,glm::vec2 start_position,std::string tank_filepath, std::string turret_filepath)
{
	m_control = control;
	m_hp = hp;
	m_damage = damage;
	m_speed = speed;
	m_ammo_max = ammo_max;
	m_ammo_current = ammo_max;
	m_reload_time = reload_time;
	m_turret_speed = turret_speed;
	m_fire_rate = fire_rate;
	m_projectile_speed = projectile_speed;
	m_accuracy = accuracy;
	m_position = start_position;
	m_texture.id = MyEngine::ResourceManager::get_texture(tank_filepath).id;
	m_turret_texture.id = MyEngine::ResourceManager::get_texture(turret_filepath).id;
	m_frame_counter = 0;
	m_reload_frame_counter = 0;
}

bool Tank::update(MyEngine::InputManager input_manager, const std::vector<std::string>& level_data, std::vector <Tank*> tanks, std::vector<Projectile>& projectiles)
{
	m_frame_counter++;

	if (is_controlable())
	{
		move_1(input_manager);
		turret_rotate_1(input_manager);
		shoot_1(projectiles, input_manager);
		
	}

	else if(!is_controlable())
	{
		m_tank_pos = glm::ivec2((m_position.x) / m_tank_size, (m_position.y)/ m_tank_size);

		float error = 0.05;

		if ((level_data.at(m_tank_pos.y).at(m_tank_pos.x) == 'e' or level_data.at(m_tank_pos.y).at(m_tank_pos.x) == 's'))
		{
			glm::vec2 curr_knot_pos = glm::vec2(m_tank_pos.x * m_tank_size + m_tank_size / 2, m_tank_pos.y * m_tank_size + m_tank_size / 2);
			if ((curr_knot_pos.x + error > m_position.x + m_tank_size / 2 and curr_knot_pos.y + error > m_position.y + m_tank_size / 2) and (curr_knot_pos.x - error < m_position.x + m_tank_size / 2 and curr_knot_pos.y - error < m_position.y + m_tank_size / 2))
			{
				m_possible_directions.clear();
				m_possible_directions.resize(0);
				int directions_number = 0;

				if ((level_data.at(m_tank_pos.y + 1).at(m_tank_pos.x) != 'w') and (level_data.at(m_tank_pos.y + 1).at(m_tank_pos.x) != 'b'))
				{
					m_possible_directions.push_back(UP);
					directions_number++;
				}
					

				if ((level_data.at(m_tank_pos.y - 1).at(m_tank_pos.x) != 'w') and (level_data.at(m_tank_pos.y - 1).at(m_tank_pos.x) != 'b'))
				{
					m_possible_directions.push_back(DOWN);
					directions_number++;
				}
					

				if ((level_data.at(m_tank_pos.y).at(m_tank_pos.x + 1) != 'w') and (level_data.at(m_tank_pos.y).at(m_tank_pos.x + 1) != 'b'))
				{
					m_possible_directions.push_back(RIGHT);
					directions_number++;
				}
				if ((level_data.at(m_tank_pos.y).at(m_tank_pos.x - 1) != 'w') and (level_data.at(m_tank_pos.y ).at(m_tank_pos.x - 1) != 'b'))
				{
					m_possible_directions.push_back(LEFT);
					directions_number++;
				}

				static std::mt19937 random_engine(time(0));
				std::uniform_real_distribution<float> pos_number(0, directions_number );

				m_current_direction = m_possible_directions.at(pos_number(random_engine));

				switch (m_current_direction)
				{
				case(UP):
					m_position.y += m_speed;
					break;
				case(DOWN):
					m_position.y -= m_speed;
					break;
				case(RIGHT):
					m_position.x += m_speed;
					break;
				case(LEFT):
					m_position.x -= m_speed;
				}

				move_2(input_manager, level_data);
				turret_rotate_2(input_manager, tanks.at(0));
				shoot_2(projectiles, input_manager);
				return false;
			}
		
		}
		
		move_2(input_manager, level_data);
		turret_rotate_2(input_manager, tanks.at(0));
		shoot_2(projectiles, input_manager);
	}

	collide_with_level(level_data);

	if (m_hp <= 0)
		return true;

	return false;
}

void Tank::draw(MyEngine::SpriteBatch& sprite_batch)
{
	glm::vec4 dest_rect (m_position.x,m_position.y, m_tank_size,m_tank_size );
	const glm::vec4 uv_rect( 0.0f,0.0f,1.0f,1.0f );

	const MyEngine::Color player_color{ 255,255,255,255 };
	const MyEngine::Color enemy_color{ 255,150,150,255 };

	glm::vec4 turret_dest_rect(m_position.x + 9, m_position.y - 30, 45, 125);

	if (is_controlable())
	{
		sprite_batch.draw(dest_rect, uv_rect, m_texture.id, 0.5f, player_color, m_angle);
		sprite_batch.draw(turret_dest_rect, uv_rect, m_turret_texture.id, 0.5f, player_color, m_turret_angle);
	}

	else
	{
		sprite_batch.draw(dest_rect, uv_rect, m_texture.id, 0.5f, enemy_color, m_angle);
		sprite_batch.draw(turret_dest_rect, uv_rect, m_turret_texture.id, 0.5f, enemy_color, m_turret_angle);
	}
}

void Tank::check_tile_pos(const std::vector<std::string>& level_data, std::vector<glm::vec2>& collide_tile_pos,  float x, float y)
{
	glm::vec2 tile_pos = glm::vec2(floor(x / TILE_SIZE), floor(y / TILE_SIZE));

	if (level_data.at(tile_pos.y).at(tile_pos.x) == 'b' or level_data.at(tile_pos.y).at(tile_pos.x) == 'w')
		collide_tile_pos.push_back(tile_pos * (float)TILE_SIZE + glm::vec2((float)TILE_SIZE / 2.0f));
}

void Tank::collide_with_tile(glm::vec2 tile_position)
{
	const float tank_rad = (float)m_tank_size / 2.0f;
	const float tile_rad = (float)TILE_SIZE / 2.0f;
	const float min_distance = tank_rad + tile_rad;

	glm::vec2 center_player_pos = m_position + glm::vec2(tank_rad);
	glm::vec2 dist_vec = center_player_pos - tile_position;

	float x_depth = min_distance - abs(dist_vec.x);
	float y_depth = min_distance - abs(dist_vec.y);

	if (x_depth > 0 or y_depth > 0)
	{
		if (std::max(x_depth, 0.0f) < std::max(y_depth, 0.0f))
		{
			if (dist_vec.x < 0)
				m_position.x -= x_depth;
			else
				m_position.x += x_depth;
		}
		else
		{
			if (dist_vec.y < 0)
				m_position.y -= y_depth;
			else
				m_position.y += y_depth;
		}
	}
}

void Tank::collide_with_level(const std::vector<std::string>& level_data)
{
	std::vector<glm::vec2> colliding_tiles;

	check_tile_pos(level_data, colliding_tiles, m_position.x, m_position.y);
	check_tile_pos(level_data, colliding_tiles, m_position.x + m_tank_size, m_position.y);
	check_tile_pos(level_data, colliding_tiles, m_position.x, m_position.y + m_tank_size);
	check_tile_pos(level_data, colliding_tiles, m_position.x + m_tank_size, m_position.y + m_tank_size);


	for (int i = 0; i < colliding_tiles.size(); i++)
		collide_with_tile(colliding_tiles.at(i));
}

bool Tank::rotate(float dest_angle)
{
	
	if (m_angle < 0)
		m_angle = 6.28 + m_angle;
	
	if (m_angle >= 6.28)
		m_angle = 0;

	
	float sub_angle = glm::abs(m_angle - dest_angle);

	if (sub_angle < 0.002 or (sub_angle <= 3.142 and sub_angle >= 3.138))
		return true;

	else
	{
		if (m_angle > dest_angle and sub_angle <= 3.14)
			m_angle -= 0.003;

		else if (m_angle < dest_angle and sub_angle <= 3.14)
			m_angle += 0.003;

		else if (m_angle > dest_angle and sub_angle > 3.14)
			m_angle += 0.003;

		else if (m_angle < dest_angle and sub_angle < 3.14)
			m_angle -= 0.003;
		else
			m_angle -= 0.003;
	}
	return false;
}

glm::vec2 Tank::get_position()
{
	return m_position;
}

int Tank::get_size()
{
	return m_tank_size;
}

void Tank::apply_damage(int damage)
{
	m_hp -= damage;
}

void Tank::fire(glm::vec2 barrel_pos, glm::vec2 direction, std::vector<Projectile>& projectiles)
{
	projectiles.emplace_back(barrel_pos, 10,  direction, m_projectile_speed, m_damage);//
	m_ammo_current--;
}

bool Tank::reload(MyEngine::InputManager& input_manager)
{
	if (input_manager.key_is_pressed(SDLK_r))
	{
		if (m_control)
		{
			m_ammo_current = -1;
		}
			
	}

	if (m_ammo_current < 0)
	{
		if (m_frame_counter <= m_reload_time)
		{
			m_reload_frame_counter++;
			return true;
		}

		else if (m_frame_counter > m_reload_time)
		{
			m_ammo_current = m_ammo_max;
			m_reload_frame_counter = 0;
			return false;
		}

		else return true;
	}

	else return false;
}

void Tank::move_1(MyEngine::InputManager& input_manager)
{
	if (input_manager.key_is_pressed(SDLK_w))
	{
		if (rotate(m_direction.up) == true)
			m_position.y += m_speed;
	}

	else if (input_manager.key_is_pressed(SDLK_s))
	{
		if (rotate(m_direction.down) == true)
			m_position.y -= m_speed;
	}

	else if (input_manager.key_is_pressed(SDLK_a))
	{
		if (rotate(m_direction.left) == true)
			m_position.x -= m_speed;
	}

	else if (input_manager.key_is_pressed(SDLK_d))
	{
		if (rotate(m_direction.right) == true)
			m_position.x += m_speed;
	}
}

void Tank::turret_rotate_1(MyEngine::InputManager& input_manager)
{
	if (m_turret_angle < 0)
		m_turret_angle = 6.283 + m_turret_angle;

	if (m_turret_angle >= 6.283)
		m_turret_angle = 0;


	glm::vec2 pos = glm::vec2(m_position.x + m_tank_size / 2, 768 - m_position.y + m_tank_size / 2);
	glm::vec2 mouse = input_manager.get_mouse_coords();
	mouse = glm::vec2(mouse.x * 2, mouse.y * 2);

	float angle = 0;

	if (mouse.x > pos.x and mouse.y < pos.y)
		angle = 6.28 - glm::atan((mouse.x - pos.x) / (pos.y - mouse.y));

	else if (mouse.x < pos.x and mouse.y < pos.y)
		angle = glm::atan((pos.x - mouse.x) / (pos.y - mouse.y));

	else if (mouse.x < pos.x and mouse.y > pos.y)
		angle = 1.57 + glm::atan((mouse.y - pos.y) / (pos.x - mouse.x));

	else if (mouse.x > pos.x and mouse.y > pos.y)
		angle = 3.14 + glm::atan((mouse.x - pos.x) / (mouse.y - pos.y));

	float sub_angle = glm::abs(m_turret_angle - angle);

	if (sub_angle < 0.002 or (sub_angle <= 3.142 and sub_angle >= 3.138))
		return;

	else
	{
		if (m_turret_angle > angle and sub_angle <= 3.14)
			m_turret_angle -= 0.003;

		else if (m_turret_angle < angle and sub_angle <= 3.14)
			m_turret_angle += 0.003;

		else if (m_turret_angle > angle and sub_angle > 3.14)
			m_turret_angle += 0.003;

		else if (m_turret_angle < angle and sub_angle < 3.14)
			m_turret_angle -= 0.003;
		else
			m_turret_angle -= 0.003;
	}
}

void Tank::shoot_1(std::vector<Projectile>& bullets, MyEngine::InputManager& input_manager)
{
	glm::vec2 def_pos = glm::vec2(m_position.x + m_tank_size / 2 - 5, m_position.y + m_tank_size / 2 - 5);
	static std::mt19937 random_engine(time(0));
	std::uniform_real_distribution<float> accuracy_angle(-m_accuracy, m_accuracy);

	glm::vec2 top(0.0f, 1.0f);//
	glm::vec2 direction = glm::rotate(top, m_turret_angle + accuracy_angle(random_engine));

	if (!reload(input_manager) and m_frame_counter >= m_fire_rate and input_manager.key_is_pressed(SDL_BUTTON_LEFT))
	{
		fire(def_pos, direction, bullets);
		m_frame_counter = 0;
	}
}

void Tank::move_2(MyEngine::InputManager& input_manager, const std::vector<std::string>& level_data)
{
	switch (m_current_direction)
	{
	case(UP):
		if(rotate(m_direction.up) == true)
			m_position.y += m_speed;
		break;
	case(DOWN):
		if (rotate(m_direction.down) == true)
			m_position.y -= m_speed;
		break;
	case(RIGHT):
		if (rotate(m_direction.right) == true)
			m_position.x += m_speed;
		break;
	case(LEFT):
		if (rotate(m_direction.left) == true)
			m_position.x -= m_speed;
		break;
	default:
		//std::cout << "������!" << std::endl;
		break;
	} 

}

void Tank::turret_rotate_2(MyEngine::InputManager& input_manager, Tank* player)
{
	if (m_turret_angle < 0)
		m_turret_angle = 6.283 + m_turret_angle;

	if (m_turret_angle >= 6.283)
		m_turret_angle = 0;


	glm::vec2 pos = glm::vec2(m_position.x + m_tank_size / 2, 768 - m_position.y + m_tank_size / 2);
	glm::vec2 temp_player = player->get_position();
	glm::vec2 player_pos = glm::vec2(temp_player.x + m_tank_size / 2, 768 - temp_player.y + m_tank_size / 2);

	float angle = 0;

	if (player_pos.x > pos.x and player_pos.y < pos.y)
		angle = 6.28 - glm::atan((player_pos.x - pos.x) / (pos.y - player_pos.y));

	else if (player_pos.x < pos.x and player_pos.y < pos.y)
		angle = glm::atan((pos.x - player_pos.x) / (pos.y - player_pos.y));

	else if (player_pos.x < pos.x and player_pos.y > pos.y)
		angle = 1.57 + glm::atan((player_pos.y - pos.y) / (pos.x - player_pos.x));

	else if (player_pos.x > pos.x and player_pos.y > pos.y)
		angle = 3.14 + glm::atan((player_pos.x - pos.x) / (player_pos.y - pos.y));

	float sub_angle = glm::abs(m_turret_angle - angle);

	if (sub_angle < 0.002 or (sub_angle <= 3.142 and sub_angle >= 3.138))
		return;

	else
	{
		if (m_turret_angle > angle and sub_angle <= 3.14)
			m_turret_angle -= 0.003;

		else if (m_turret_angle < angle and sub_angle <= 3.14)
			m_turret_angle += 0.003;

		else if (m_turret_angle > angle and sub_angle > 3.14)
			m_turret_angle += 0.003;

		else if (m_turret_angle < angle and sub_angle < 3.14)
			m_turret_angle -= 0.003;
		else
			m_turret_angle -= 0.003;
	}
}

void Tank::shoot_2(std::vector<Projectile>& bullets, MyEngine::InputManager& input_manager)
{
	glm::vec2 def_pos = glm::vec2(m_position.x + m_tank_size / 2 - 5, m_position.y + m_tank_size / 2 - 5);
	static std::mt19937 random_engine(time(0));
	std::uniform_real_distribution<float> accuracy_angle(-m_accuracy, m_accuracy);

	glm::vec2 top(0.0f, 1.0f);//
	glm::vec2 direction = glm::rotate(top, m_turret_angle + accuracy_angle(random_engine));//

	if (!reload(input_manager) and m_frame_counter >= m_fire_rate )
	{
		fire(def_pos, direction, bullets);
		m_frame_counter = 0;
	}
}

