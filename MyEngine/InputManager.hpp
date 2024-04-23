#pragma once 

#include <SDL.h>
#include <glew.h>
#include <glm/glm.hpp>

#include <iostream>
#include <unordered_map>

namespace MyEngine
{
	enum class Key
	{
		MOUSE_LEFT,

	};

	
	class InputManager
	{
		std::unordered_map<unsigned int, bool> m_key_map;
		glm::vec2 m_mouse_coords;

	public:
		InputManager();
		~InputManager();

		void press_key(unsigned int key_id);
		void release_key(unsigned int key_id);

		bool key_is_pressed(unsigned int key_id);

		void set_mouse_coords(int x, int y);
		glm::vec2 get_mouse_coords();
	};
}