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
	
	void InputManager::update()
	{
		for (auto& it : m_key_map)
			m_previous_key_map[it.first] = it.second;
	}

	void InputManager::press_key(unsigned int key_id)
	{
		m_key_map[key_id] = true;
	}
	void InputManager::release_key(unsigned int key_id)
	{
		m_key_map[key_id] = false;
	}

	bool InputManager::is_key_pressed(unsigned int key_id)
	{
		if (is_key_down(key_id) == true and was_key_down(key_id) == false) 
			return true;

		return false;
	}

	bool InputManager::is_key_down(unsigned int key_id)
	{
		auto key = m_key_map.find(key_id);

		if (key != m_key_map.end())
			return key->second;

		else
			return false;

	}

	bool InputManager::was_key_down(unsigned int key_id)
	{
		auto key = m_previous_key_map.find(key_id);

		if (key != m_previous_key_map.end())
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