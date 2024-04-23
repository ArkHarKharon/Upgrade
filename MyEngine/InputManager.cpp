#include "InputManager.hpp"

namespace MyEngine
{

	InputManager::InputManager():
		m_mouse_coords{0,0}
	{

	}

	InputManager::~InputManager()
	{

	}

	void InputManager::press_key(unsigned int key_id)
	{
		m_key_map[key_id] = true;
	}
	void InputManager::release_key(unsigned int key_id)
	{
		m_key_map[key_id] = false;
	}

	bool InputManager::key_is_pressed(unsigned int key_id)
	{
		auto key = m_key_map.find(key_id);

		if (key != m_key_map.end())
			return key->second;

		else 
			return false;
	}

	void InputManager::set_mouse_coords(int x, int y)
	{
		m_mouse_coords.x = x;
		m_mouse_coords.y = y;
	}

	glm::vec2 InputManager::get_mouse_coords()
	{
		return m_mouse_coords;
	}

}