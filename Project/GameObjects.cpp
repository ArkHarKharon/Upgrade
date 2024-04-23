#include "GameObjects.hpp"


Projectile::Projectile(glm::vec2 position, glm::vec2 direction, float speed, int lifetime)
{
	m_position = position;
	m_speed = speed;
	m_direction = direction;
	m_lifetime = lifetime;

}

Projectile::~Projectile()
{

}

void Projectile::draw(MyEngine::SpriteBatch& sprite_batch)
{
	MyEngine::Color color{255,255,255,255};
	glm::vec4 uv_pos(0.0f, 0.0f, 1.0f, 1.0f);
	glm::vec4 pos_size = glm::vec4(m_position.x, m_position.y, 30, 30);
	static MyEngine::GLTexture bullet_texture = MyEngine::ResourceManager::get_texture("Textures/bulletBlue.png");

	sprite_batch.draw(pos_size, uv_pos, bullet_texture.id, 0.0f, color);
}

bool Projectile::update()
{
	m_position += m_direction * m_speed;
	m_lifetime--;

	if (m_lifetime == 0)
		return true;

	return false;
}



const int TILE_SIZE = 64;

Level::Level(const std::string& level_file) : m_tile_size{TILE_SIZE}
{

	std::ifstream file;
	file.open(level_file);

	if (file.fail())
		MyEngine::fatal_error("Файл " + level_file + " НЕ открыт!");

	std::string temp;
	while (std::getline(file, temp))
		m_level_data.push_back(temp);

	m_sprite_batch.init();
	m_sprite_batch.begin();

	glm::vec4 uv_rect(0.0f,0.0f,1.0f, 1.0f);
	MyEngine::Color color{ 255,255,255,255 };

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

			case('t'):
				m_level_data.at(y).at(x) = 'f';
				m_enemy_start_position.emplace_back(x * m_tile_size, y * m_tile_size);
				break;

			case('f'):
				break;

			case('e'):
				///
				m_level_data.at(y).at(x) = 'f';
				break;

			case('s'):
				m_level_data.at(y).at(x) = 'f';
				m_start_position.x = x * m_tile_size;
				m_start_position.y = y * m_tile_size;
				break;

			case('w'):
				m_sprite_batch.draw(dest_rect, uv_rect,
					MyEngine::ResourceManager::get_texture("Data/Textures/sandBrick.png").id,
					0.0f, color);
				break;

			default:
				std::cout << "Неверный символ в файле уровня: " << tile << "(" << x << ", " << y << ")\n";
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

const std::vector<glm::ivec2>& Level::get_enemies_pos()
{
	return m_enemy_start_position;
}

std::vector<std::string>& Level::get_level_data()
{
	return m_level_data;
}




Tank::Tank(): m_tank_size{32}
{

}


Tank::~Tank()
{
	
}


void Tank::init(float speed, glm::vec2 position,std::string filepath)
{
	m_speed = speed;
	m_position = position;
	m_texture.id = MyEngine::ResourceManager::get_texture(filepath).id;
}


bool Tank::update(MyEngine::InputManager input_manager,
				  const std::vector<std::string>& level_data,
				  std::vector <Tank*> tanks)
{
	if (input_manager.key_is_pressed(SDLK_w))
	{
		m_position.y += m_speed;
	}

	else if (input_manager.key_is_pressed(SDLK_s))
	{
		m_position.y -= m_speed;
	}

	if (input_manager.key_is_pressed(SDLK_a))
	{
		m_position.x -= m_speed;
	}
	
	if (input_manager.key_is_pressed(SDLK_d))
	{
		m_position.x += m_speed;
	}

	//collide_with_level(level_data);

	return true;
}


void Tank::draw(MyEngine::SpriteBatch& sprite_batch)
{
	
	glm::vec4 dest_rect{ m_position.x,m_position.y, m_tank_size,m_tank_size };
	const glm::vec4 uv_rect{ 0.0f,0.0f,1.0f,1.0f };
	const MyEngine::Color color{ 255,255,255,255 };

	sprite_batch.draw(dest_rect,uv_rect,m_texture.id,1.0f,color);
}


void Tank::check_tile_pos(const std::vector<std::string>& level_data,
						  std::vector<glm::vec2>& collide_tile_pos,
						  float x, float y)
{
	glm::vec2 tile_pos = glm::vec2(floor(x / TILE_SIZE), floor(y / TILE_SIZE));

	if (level_data.at(tile_pos.x).at(tile_pos.y) != 'f')
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

	check_tile_pos(level_data, colliding_tiles, m_position.x, m_position.y + (float)TILE_SIZE / 2);
	check_tile_pos(level_data, colliding_tiles, m_position.x, m_position.y - (float)TILE_SIZE/2 );
	check_tile_pos(level_data, colliding_tiles, m_position.x + (float)TILE_SIZE , m_position.y/2 );
	check_tile_pos(level_data, colliding_tiles, m_position.x - (float)TILE_SIZE , m_position.y/2 );

	for (int i = 0; i < colliding_tiles.size(); i++)
		collide_with_tile(colliding_tiles.at(i));
}





PlayerTank::PlayerTank()
{

}

PlayerTank::~PlayerTank()
{

}