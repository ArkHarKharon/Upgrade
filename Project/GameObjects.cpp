
#include "GameObjects.hpp"


const int TILE_SIZE = 64;

Projectile::Projectile(glm::vec2 position, float projectile_size, glm::vec2 direction, float speed, int damage, bool bot_sender, bool test) : m_lifetime{ 0 }
{
    m_position = position;
    m_start_position = m_position;
    m_speed = speed;
    m_direction = direction;
    m_damage = damage;
    m_projectile_size = projectile_size;
    m_is_bot_sender = bot_sender;
    m_is_test_projectile = test;
}

Projectile::~Projectile()
{

}

void Projectile::draw(MyEngine::SpriteBatch& sprite_batch)
{
    MyEngine::Color color_bot{ 255,255,0,255 };
    MyEngine::Color color_player{ 255,255,255,255 };

    glm::vec4 uv_pos(0.0f, 0.0f, 1.0f, 1.0f);
    glm::vec4 pos_size = glm::vec4(m_position.x, m_position.y, m_projectile_size, m_projectile_size);
    MyEngine::GLTexture bullet_texture = MyEngine::ResourceManager::get_texture("Data/Textures/bulletBlue.png");

    sprite_batch.draw(pos_size, uv_pos, bullet_texture.id, 1.0f, (m_is_bot_sender) ? (color_bot) : (color_player));

}

bool Projectile::update(std::vector<Tank*> tanks, const std::vector<std::string>& level_data)
{
    m_position += m_direction * m_speed;
    m_lifetime++;

    if (m_lifetime > 125)
    {
        if (collide_with_tanks(tanks))
            return true;
    }

    if (collide_with_level(level_data))
        return true;

    return false;
}

bool Projectile::collide_with_tanks(std::vector<Tank*> tanks)
{
    for (int i = 0; i < tanks.size(); i++)
    {
        glm::vec2 tank_pos = glm::vec2(tanks.at(i)->get_position().x + tanks.at(i)->get_size() / 2, tanks.at(i)->get_position().y + tanks.at(i)->get_size() / 2);
        glm::vec2 projectile_pos = glm::vec2(m_position.x + m_projectile_size / 2, m_position.y + m_projectile_size / 2);

        int min_distance = (tanks.at(i)->get_size() / 2) + (m_projectile_size / 2);

        float dist_x = glm::abs(tank_pos.x - projectile_pos.x);
        float dist_y = glm::abs(tank_pos.y - projectile_pos.y);

        if (dist_x < min_distance and dist_y < min_distance)
        {
            if ((!tanks.at(i)->is_controlable() and m_is_bot_sender) or m_is_test_projectile)
                return true;
            else
            {
                tanks.at(i)->apply_damage(m_damage);
                return true;
            }
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
        MyEngine::fatal_error("Файл " + level_file + " НЕ открыт!");

    std::string temp;
    while (std::getline(file, temp))
        m_level_data.push_back(temp);

    m_sprite_batch.init();
    m_sprite_batch.begin();

    glm::vec4 uv_rect(0.0f, 0.0f, 1.0f, 1.0f);
    MyEngine::Color color{ 255,255,255,255 };

    glm::vec4 grass_dest_rect(0.0f, 0.0f, m_level_data.at(0).size() * TILE_SIZE, m_level_data.size() * TILE_SIZE);
    m_sprite_batch.draw(grass_dest_rect, uv_rect, MyEngine::ResourceManager::get_texture("Data/Textures/grass.png").id, 0.0f, color);

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
                m_knots.push_back(glm::ivec2(x * m_tile_size, y * m_tile_size));
                break;

            case('k'):
                m_knots.push_back(glm::ivec2(x * m_tile_size, y * m_tile_size));
                break;

            case('f'):
                break;

            case('z'):
                m_level_data.at(y).at(x) = ' ';
                break;

            case('p'):
                m_start_position = glm::ivec2(x * m_tile_size, y * m_tile_size);
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

int Level::get_random_int(int min, int max)
{
    static std::mt19937 random_engine(time(0));
    std::uniform_real_distribution<float> pos_number(min, max);

    return pos_number(random_engine);
}

glm::ivec2 Level::get_enemy_pos()
{
    int current_pos_number = get_random_int(0, m_enemy_positions.size() - 1);
    //std::cout << current_pos_number;
    return glm::ivec2(m_enemy_positions.at(current_pos_number).x, m_enemy_positions.at(current_pos_number).y);
}

std::vector<std::string>& Level::get_level_data()
{
    return m_level_data;
}








Tank::Tank() : m_tank_size{ 64 }, m_angle{ 0 }, m_turret_angle{ 0.0f }
{

}

Tank::Tank(bool control, int hp, int damage, float speed, int ammo_max, int reload_time, float turret_speed, int fire_rate, float projectile_speed,
    float accuracy, glm::vec2 start_position, std::string tank_filepath, std::string turret_filepath, std::string hp_filepath, std::string ammo_filepath,
    MyEngine::SoundEffect fire_effect, MyEngine::SoundEffect death_effect)
    : m_tank_size{ 64 }, m_angle{ 0 }, m_turret_angle{ 0.0f }
{
    m_control = control;
    m_hp = hp;
    m_max_hp = hp;
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
    m_hp_texture.id = MyEngine::ResourceManager::get_texture(hp_filepath).id;
    m_ammo_texture.id = MyEngine::ResourceManager::get_texture(ammo_filepath).id;
    m_frame_counter = 0;
    m_reload_frame_counter = 0;
    m_shoot_effect = fire_effect;
    m_death_effect = death_effect;

}


Tank::~Tank()
{

}

bool Tank::is_controlable()
{
    return m_control;
}

void Tank::init(bool control, int hp, int damage, float speed, int ammo_max,
    int reload_time, float turret_speed, int fire_rate, float projectile_speed,
    float accuracy, glm::vec2 start_position, std::string tank_filepath,
    std::string turret_filepath, std::string hp_filepath, std::string ammo_filepath,
    MyEngine::SoundEffect fire_effect, MyEngine::SoundEffect death_effect)
{
    m_control = control;
    m_hp = hp;
    m_max_hp = hp;
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
    m_hp_texture.id = MyEngine::ResourceManager::get_texture(hp_filepath).id;
    m_ammo_texture.id = MyEngine::ResourceManager::get_texture(ammo_filepath).id;
    m_frame_counter = 0;
    m_reload_frame_counter = 0;
    m_shoot_effect = fire_effect;
    m_death_effect = death_effect;
}

void Tank::draw(MyEngine::SpriteBatch& sprite_batch)
{
    glm::vec4 dest_rect(m_position.x, m_position.y, m_tank_size, m_tank_size);
    const glm::vec4 uv_rect(0.0f, 0.0f, 1.0f, 1.0f);

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


    glm::vec4 hp_dest_rect(m_position.x - 18, m_position.y + m_tank_size + 25, 100 * ((float)m_hp / (float)m_max_hp), 10);
    glm::vec4 ammo_dest_rect(m_position.x - 18, m_position.y + m_tank_size + 10, 100 * ((float)m_ammo_current / (float)m_ammo_max), 10);

    sprite_batch.draw(hp_dest_rect, uv_rect, m_hp_texture.id, 1.0f, player_color, 0.0f);
    sprite_batch.draw(ammo_dest_rect, uv_rect, m_ammo_texture.id, 1.0f, player_color, 0.0f);

    for (int i = 0; i < m_projectiles.size(); i++)
    {
        if (!m_projectiles.at(i).is_test())
            m_projectiles.at(i).draw(sprite_batch);
    }
}

void Tank::check_tile_pos(const std::vector<std::string>& level_data, std::vector<glm::vec2>& collide_tile_pos, float x, float y)
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

void Tank::fire(glm::vec2 barrel_pos, glm::vec2 direction)
{
    m_shoot_effect.play();
    m_projectiles.emplace_back(barrel_pos, 10, direction, m_projectile_speed, m_damage, !m_control);//
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








void PlayerTank::move(MyEngine::InputManager& input_manager, const std::vector<std::string>& level_data)
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

void PlayerTank::turret_rotate(MyEngine::InputManager& input_manager, Tank* player)
{
    if (m_turret_angle < 0)
        m_turret_angle = 6.283 + m_turret_angle;

    if (m_turret_angle >= 6.283)
        m_turret_angle = 0;


    glm::vec2 pos = glm::vec2(m_position.x + m_tank_size / 2, m_position.y + m_tank_size / 2 - 20);
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

    else if (mouse.y == pos.y and mouse.x < pos.x)
        angle = 1.57;

    else if (mouse.y == pos.y and mouse.x > pos.x)
        angle = 5, 71;

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

void PlayerTank::shoot(MyEngine::InputManager& input_manager, std::vector <Tank*> tanks, const std::vector<std::string>& level_data)
{
    glm::vec2 def_pos = glm::vec2(m_position.x + m_tank_size / 2 - 5, m_position.y + m_tank_size / 2 - 5);
    static std::mt19937 random_engine(time(0));
    std::uniform_real_distribution<float> accuracy_angle(-m_accuracy, m_accuracy);

    glm::vec2 top(0.0f, 1.0f);//
    glm::vec2 direction = glm::rotate(top, m_turret_angle + accuracy_angle(random_engine));

    if (!reload(input_manager) and m_frame_counter >= m_fire_rate and input_manager.key_is_pressed(SDL_BUTTON_LEFT))
    {
        fire(def_pos, direction);
        m_frame_counter = 0;
    }
}

bool PlayerTank::update(MyEngine::InputManager input_manager, const std::vector<std::string>& level_data, std::vector <Tank*> tanks)
{
    m_frame_counter++;

    for (int i = 0; i < m_projectiles.size();)
    {
        if (m_projectiles.at(i).update(tanks, level_data))
        {
            m_projectiles.at(i) = m_projectiles.back();
            m_projectiles.pop_back();
        }
        else i++;
    }


    move(input_manager, level_data);
    turret_rotate(input_manager, nullptr);
    shoot(input_manager, tanks, level_data);

    collide_with_level(level_data);

    if (m_hp <= 0)
    {
        m_death_effect.play();
        return true;

    }

    return false;

}




void BotTank::move(MyEngine::InputManager& input_manager, const std::vector<std::string>& level_data)
{
    switch (m_current_direction)
    {
    case(UP):
        if (rotate(m_direction.up) == true)
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
        //std::cout << "Втыкаю!" << std::endl;
        break;
    }

}

void BotTank::turret_rotate(MyEngine::InputManager& input_manager, Tank* player)
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

    else if (player_pos.y == pos.y and player_pos.x < pos.x)
        angle = 1.57;

    else if (player_pos.y == pos.y and player_pos.x > pos.x)
        angle = 4.71;

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

void BotTank::shoot(MyEngine::InputManager& input_manager, std::vector <Tank*> tanks, const std::vector<std::string>& level_data)
{
    glm::vec2 def_pos = glm::vec2(m_position.x + m_tank_size / 2 - 5, m_position.y + m_tank_size / 2 - 5);
    static std::mt19937 random_engine(time(0));
    std::uniform_real_distribution<float> accuracy_angle(-m_accuracy, m_accuracy);

    glm::vec2 top(0.0f, 1.0f);//
    glm::vec2 direction = glm::rotate(top, m_turret_angle + accuracy_angle(random_engine));//

    if (!reload(input_manager) and m_frame_counter >= m_fire_rate) //and test_shot(input_manager, def_pos, direction, tanks)
    {
        if (test_shot(def_pos, direction, tanks, level_data)) //
            fire(def_pos, direction);
        m_frame_counter = 0;
    }
}

bool BotTank::test_shot(glm::vec2 barrel_pos, glm::vec2 direction, std::vector <Tank*> tanks, const std::vector<std::string>& level_data)
{
    if (!m_test_in_progress)
    {
        //std::cout << "Запускаю тестовый!\n";

        m_projectiles.emplace_back(barrel_pos, 10, direction, m_projectile_speed * 1.0f, 0, !m_control, true);
        m_test_in_progress = true;
    }

    if (m_test_in_progress)
    {
        //std::cout << "Готов к выстрелу!\n";
        bool is_granted = false;

        for (size_t i = 0; i < m_projectiles.size(); i++)
        {
            if (m_projectiles.at(i).is_test() and !m_projectiles.at(i).collide_with_level(level_data))
            {
                if (m_projectiles.at(i).collide_with_tanks(tanks) and m_projectiles.at(i).is_test())
                {
                    //std::cout << "Успешный пристрелочный!\n";
                    m_test_in_progress = false;

                    is_granted = true;
                }

            }

        }
        return is_granted;
    }

    


        if (false)//m_projectiles.size() == 0)
        {
            std::cout << "Без тестов!\n";
            m_projectiles.emplace_back(barrel_pos, 10, direction, m_projectile_speed * 10.0f, 0, !m_control, true);
            m_test_in_progress = true;
        }

}

bool BotTank::update(MyEngine::InputManager input_manager, const std::vector<std::string>& level_data, std::vector <Tank*> tanks)
{
    m_frame_counter++;

    m_tank_pos = glm::ivec2((m_position.x) / m_tank_size, (m_position.y) / m_tank_size);

    bool is_player_found = false;
    short int player_id{};

    for (size_t i = 0; i < tanks.size(); i++)
    {
        if (tanks.at(i)->is_controlable())
        {
            is_player_found = true;
            player_id = i;
        }
    }



    float error = 0.05;

    if ((level_data.at(m_tank_pos.y).at(m_tank_pos.x) == 'e' or level_data.at(m_tank_pos.y).at(m_tank_pos.x) == 's') or level_data.at(m_tank_pos.y).at(m_tank_pos.x) == 'k')
    {
        glm::vec2 curr_knot_pos = glm::vec2(m_tank_pos.x * m_tank_size + m_tank_size / 2, m_tank_pos.y * m_tank_size + m_tank_size / 2);
        if ((curr_knot_pos.x + error > m_position.x + m_tank_size / 2 and curr_knot_pos.y + error > m_position.y + m_tank_size / 2) and (curr_knot_pos.x - error < m_position.x + m_tank_size / 2 and curr_knot_pos.y - error < m_position.y + m_tank_size / 2))
        {
            m_possible_directions.clear();
            //m_possible_directions.resize(0);
            int directions_number = 0;

            if ((level_data.at(m_tank_pos.y + 1).at(m_tank_pos.x) != 'w') and (level_data.at(m_tank_pos.y + 1).at(m_tank_pos.x) != 'b'))
            {
                m_possible_directions.push_back(UP);
                directions_number++;

                if (m_position.y < tanks.at(player_id)->get_position().y)
                {
                    m_possible_directions.push_back(UP);
                    directions_number++;
                }
            }


            if ((level_data.at(m_tank_pos.y - 1).at(m_tank_pos.x) != 'w') and (level_data.at(m_tank_pos.y - 1).at(m_tank_pos.x) != 'b'))
            {
                m_possible_directions.push_back(DOWN);
                directions_number++;

                if (m_position.y > tanks.at(player_id)->get_position().y)
                {
                    m_possible_directions.push_back(DOWN);
                    directions_number++;
                }
            }


            if ((level_data.at(m_tank_pos.y).at(m_tank_pos.x + 1) != 'w') and (level_data.at(m_tank_pos.y).at(m_tank_pos.x + 1) != 'b'))
            {
                m_possible_directions.push_back(RIGHT);
                directions_number++;

                if (m_position.x < tanks.at(player_id)->get_position().x)
                {
                    m_possible_directions.push_back(RIGHT);
                    directions_number++;
                }
            }

            if ((level_data.at(m_tank_pos.y).at(m_tank_pos.x - 1) != 'w') and (level_data.at(m_tank_pos.y).at(m_tank_pos.x - 1) != 'b'))
            {
                m_possible_directions.push_back(LEFT);
                directions_number++;

                if (m_position.x > tanks.at(player_id)->get_position().x)
                {
                    m_possible_directions.push_back(LEFT);
                    directions_number++;
                }
            }

            static std::mt19937 random_engine(time(0));
            std::uniform_real_distribution<float> pos_number(0, directions_number);

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

            move(input_manager, level_data);
            if (is_player_found)
            {
                turret_rotate(input_manager, tanks.at(player_id));
                shoot(input_manager, tanks, level_data);
            }
            return false;
        }


    }

    for (int i = 0; i < m_projectiles.size();)
    {
        if (m_projectiles.at(i).update(tanks, level_data))
        {
            m_projectiles.at(i) = m_projectiles.back();
            m_projectiles.pop_back();
        }
        else i++;
    }

    move(input_manager, level_data);
    turret_rotate(input_manager, tanks.at(player_id));
    shoot(input_manager, tanks, level_data);

    collide_with_level(level_data);

    if (m_hp <= 0)
    {
        m_death_effect.play();
        return true;

    }

    return false;
}







GameManager::GameManager() : m_score{ 0 }, m_frame_count{ 0 }, m_max_player_lives{ 3 }, m_is_player_dead{ false }, m_is_round_started{ false }, m_is_initialized{ false }, m_is_session_started{ false }
{
    m_current_player_lives = m_max_player_lives;
}




void GameManager::start_round(std::vector<BotTank*>& bots, std::vector<Tank*>& tanks, std::vector <Level*>& levels, MyEngine::AudioManager& audio_manager, bool bot_spawn)
{
    bool player = true;
    bool bot = false;
    int hp = 2000;
    int damage = 50;
    float speed = 0.1f;
    int ammo_max = 50;
    int reload_time = 5000;
    float turret_speed = 0.003f;
    int fire_rate = 300;
    float projectile_speed = 0.4f;
    float accuracy = 0.01f;
    glm::vec2 player_start_pos = levels.at(0)->get_start_pos();
    std::vector<glm::vec2> enemy_start_pos;
    std::string tank_texture = "Data/Textures/tankBlue.png";
    std::string turret_texture = "Data/Textures/tankTurret.png";
    std::string hp_texture = "Data/Textures/hp.png";
    std::string ammo_texture = "Data/Textures/ammo.png";
    MyEngine::SoundEffect fire_effect = audio_manager.load_sound_effect("Data/Sound/shot.mp3");
    MyEngine::SoundEffect death_player = audio_manager.load_sound_effect("Data/Sound/death_player.mp3");
    MyEngine::SoundEffect death_bot = audio_manager.load_sound_effect("Data/Sound/death_bot.mp3");


    PlayerTank* player_tank = new PlayerTank();
    player_tank->init(player, hp, damage, speed, ammo_max, reload_time,
        turret_speed, fire_rate, projectile_speed, accuracy, player_start_pos,
        tank_texture, turret_texture, hp_texture, ammo_texture, fire_effect, death_bot);
    tanks.push_back(player_tank);

    if (bot_spawn)
    {
        BotTank* bot1 = new BotTank();
        BotTank* bot2 = new BotTank();
        BotTank* bot3 = new BotTank();
        BotTank* bot4 = new BotTank();
        BotTank* bot5 = new BotTank();

        bots.push_back(bot1);
        bots.push_back(bot2);
        bots.push_back(bot3);
        bots.push_back(bot4);
        bots.push_back(bot5);

        enemy_start_pos.resize(bots.size());
        for (size_t i = 0; i < bots.size(); i++)
        {
            enemy_start_pos.at(i) = levels.at(0)->get_enemy_pos();
        }

        for (size_t i = 0; i < bots.size(); i++)
        {
            bots.at(i)->init(bot, hp * m_bot_strengh_scale, damage * m_bot_strengh_scale, speed * m_bot_strengh_scale, ammo_max * m_bot_strengh_scale, reload_time / m_bot_strengh_scale,
                turret_speed * m_bot_strengh_scale, fire_rate / m_bot_strengh_scale, projectile_speed, accuracy,
                enemy_start_pos.at(i), tank_texture, turret_texture, hp_texture,
                ammo_texture, fire_effect, death_player);
            tanks.push_back(bots.at(i));
        }

    }
}

void GameManager::start_session(std::vector<BotTank*>& bots, std::vector<Tank*>& tanks, std::vector <Level*>& levels, MyEngine::AudioManager& audio_manager)
{
    m_is_session_started = true;

    m_current_player_lives = m_max_player_lives;
    m_score = 0;

    start_round(bots, tanks, levels, audio_manager);
}

void GameManager::end_game(std::vector<BotTank*>& bots, std::vector<Tank*>& tanks, std::vector <Level*>& levels, MyEngine::AudioManager& audio_manager)
{
    for (size_t i = 0; i < tanks.size(); i++)
    {
        delete tanks.at(i);
    }

    for (size_t i = 0; i < bots.size(); i++)
    {
        delete bots.at(i);
    }


    tanks.resize(0);
    bots.resize(0);
}

void GameManager::bot_number_control(std::vector<BotTank*>& bots, std::vector<Tank*>& tanks, std::vector <Level*>& levels, MyEngine::AudioManager& audio_manager)
{
    short int bot_num_limit = 0;
    switch (m_difficulty)
    {
    case(EASY):
        bot_num_limit = 15;
        break;

    case(NORMAL):
        bot_num_limit = 10;
        break;

    case(HARD):
        bot_num_limit = 7;
        break;

    case(IMPOSIBLE):
        bot_num_limit = 5;
        break;

    }

    if (m_frame_count > 25000)
    {
        if (tanks.size() - 1 < bot_num_limit)
        {
            spawn_bot(bots, tanks, levels, audio_manager);
            m_frame_count = 0;
        }
    }

}




void GameManager::spawn_bot(std::vector<BotTank*>& bots, std::vector<Tank*>& tanks, std::vector <Level*>& levels, MyEngine::AudioManager& audio_manager)
{
    bool player = true;
    bool bot = false;
    int hp = 2000;
    int damage = 50;
    float speed = 0.1f;
    int ammo_max = 50;
    int reload_time = 5000;
    float turret_speed = 0.003f;
    int fire_rate = 300;
    float projectile_speed = 0.4f;
    float accuracy = 0.01f;
    glm::vec2 player_start_pos = levels.at(0)->get_start_pos();
    std::vector<glm::vec2> enemy_start_pos;
    std::string tank_texture = "Data/Textures/tankBlue.png";
    std::string turret_texture = "Data/Textures/tankTurret.png";
    std::string hp_texture = "Data/Textures/hp.png";
    std::string ammo_texture = "Data/Textures/ammo.png";
    MyEngine::SoundEffect fire_effect = audio_manager.load_sound_effect("Data/Sound/shot.mp3");
    MyEngine::SoundEffect death_player = audio_manager.load_sound_effect("Data/Sound/death_player.mp3");
    MyEngine::SoundEffect death_bot = audio_manager.load_sound_effect("Data/Sound/death_bot.mp3");

    BotTank* bot1 = new BotTank();
    bot1->init(bot, hp * m_bot_strengh_scale, damage * m_bot_strengh_scale, speed * m_bot_strengh_scale, ammo_max * m_bot_strengh_scale, reload_time / m_bot_strengh_scale,
        turret_speed * m_bot_strengh_scale, fire_rate / m_bot_strengh_scale, projectile_speed, accuracy,
        levels.at(0)->get_enemy_pos(), tank_texture, turret_texture, hp_texture,
        ammo_texture, fire_effect, death_player);

    tanks.push_back(bot1);
}

void GameManager::session_control(std::vector<BotTank*>& bots, std::vector<Tank*>& tanks, std::vector <Level*>& levels, MyEngine::AudioManager& audio_manager, MyEngine::Window& window)
{
    if (!m_is_initialized)
    {
        session_control_init_setting();
        m_is_initialized = true;
        SDL_RaiseWindow(window.get_window());
    }

    if (!m_is_session_started)
    {
        start_session(bots, tanks, levels, audio_manager);
    }

    bot_number_control(bots, tanks, levels, audio_manager);



    m_frame_count++;
}

void GameManager::session_control_init_setting()
{
    //system("cls");

    //std::cout
    //    << "Выберите уровень сложности:\n"
    //    << "1) Лёгкий (боты в 4 раза слабее игрока, 25 очков за бота)\n"
    //    << "2) Нормальный (боты в 2 раза слабее игрока, 100 очков за бота)\n"
    //    << "3) Тяжелый (боты равны игроку, 200 очков за бота)\n"
    //    << "4) Невозможный (боты в 1.5 раза сильнее, 400 очков за бота)\n"
    //    << "Выбор: ";

    //short int choice;
    //std::cin >> choice;//

    //m_difficulty = choice;

    //switch (choice)
    //{
    //case(EASY):
    //    m_bot_strengh_scale = 0.25f;
    //    break;

    //case(NORMAL):
    //    m_bot_strengh_scale = 0.5f;
    //    break;

    //case(HARD):
    //    m_bot_strengh_scale = 1.0f;
    //    break;

    //case(IMPOSIBLE):
    //    m_bot_strengh_scale = 1.5f;
    //    break;

    //}

    m_bot_strengh_scale = 0.25f;
}

void GameManager::increase_score()
{
    switch (m_difficulty)
    {
    case(EASY):
        m_score += 25;
        break;

    case(NORMAL):
        m_score += 100;
        break;

    case(HARD):
        m_score += 200;
        break;

    case(IMPOSIBLE):
        m_score += 400;
        break;
    }
    m_bots_killed++;
}

void GameManager::handle_player_death(std::vector<BotTank*>& bots, std::vector<Tank*>& tanks, std::vector <Level*>& levels, MyEngine::AudioManager& audio_manager)
{
    if (m_current_player_lives == 1)
        end_screen(bots, tanks, levels, audio_manager);

    else
    {
        m_current_player_lives--;
        start_round(bots, tanks, levels, audio_manager, false);

        std::cout
            << "ВЫ ПОГИБЛИ!!!\n"
            << "У вас осталось " << m_current_player_lives << " жизней!\n\n";
    }

}

void GameManager::end_screen(std::vector<BotTank*>& bots, std::vector<Tank*>& tanks, std::vector <Level*>& levels, MyEngine::AudioManager& audio_manager)
{
    //end_game(bots,tanks, levels, audio_manager);w

    std::string difficulty;
    switch (m_difficulty)
    {
    case(EASY):
        difficulty = "лёгкая";
        break;

    case(NORMAL):
        difficulty = "нормальная";
        break;

    case(HARD):
        difficulty = "сложная";
        break;

    case(IMPOSIBLE):
        difficulty = "невозможная";
        break;
    }

        std::cout
        << "\tВЫ ПОГИБЛИ!\n"
        << "Сложность: " << difficulty << std::endl
        << "Ботов уничтожено: " << m_bots_killed << std::endl
        << "Счёт: " << m_score << std::endl << std::endl
        << "Для начала новой игры нажмите Enter...";

    char a;
    std::cin >> a;

    m_is_initialized = false;
    m_is_session_started = false;
    m_is_round_started = false;

    start_session(bots, tanks, levels, audio_manager);
}