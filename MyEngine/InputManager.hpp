#pragma once 

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <iostream>
#include <unordered_map>

namespace MyEngine
{

	class InputManager
	{
	private:
		std::unordered_map<unsigned int, bool> m_key_map;
		std::unordered_map<unsigned int, bool> m_previous_key_map;

		glm::vec2 m_mouse_coords;

		bool was_key_down(unsigned int key_id);

	public:
		InputManager();
		~InputManager();

		void update();

		void press_key(unsigned int key_id);
		void release_key(unsigned int key_id);

		bool is_key_down(unsigned int key_id);

		bool is_key_pressed(unsigned int key_id);

		void set_mouse_coords(float x, float y);
		glm::vec2 get_mouse_coords();
	};
}