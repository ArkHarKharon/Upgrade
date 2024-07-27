#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace MyEngine
{

	class Camera2D
	{
		glm::vec2 m_position;
		glm::mat4 m_cam_matrix;
		glm::mat4 m_ortho_matrix;
		float m_scale;
		bool m_need_update;
		int m_screen_width;
		int m_screen_height;

	public:
		Camera2D();
		~Camera2D();

		void init_camera(int screen_width, int screen_height);

		void update();

		void set_position(const glm::vec2& position);
		void set_scale(float scale);

		glm::vec2 get_position();
		float get_scale();
		glm::mat4 get_cam_matrix();

		int get_screen_width();
		int get_screen_height();

		glm::vec2 get_world_coords(glm::vec2 screen_coords);
	};

}