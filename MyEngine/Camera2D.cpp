#include "Camera2D.hpp"

namespace MyEngine
{

	Camera2D::Camera2D() :
		m_position{ 0.0f,0.0f },
		m_cam_matrix{ 1.0f },
		m_ortho_matrix{ 1.0f },
		m_scale{ 1.0f },
		m_need_update{ true },
		m_screen_height{ 720 },
		m_screen_width{ 1080 }
	{

	}

	Camera2D::~Camera2D()
	{

	}

	void Camera2D::init_camera(int screen_width, int screen_height)
	{
		m_screen_width = screen_width;
		m_screen_height = screen_height;
		m_ortho_matrix = glm::ortho(0.0f, (float)m_screen_width, 0.0f, (float)m_screen_height);

	}

	void Camera2D::update()
	{
		if (m_need_update)
		{
			glm::vec3 translate{ -m_position.x + m_screen_width / 2, -m_position.y + m_screen_height / 2, 0.0f };
			m_cam_matrix = glm::translate(m_ortho_matrix, translate);

			glm::vec3 scale{ m_scale,m_scale,0.0f };
			m_cam_matrix = glm::scale(glm::mat4(1.0f), scale) * m_cam_matrix;

			m_need_update = false;
		}
	}

	void Camera2D::set_position(const glm::vec2& new_position)
	{
		m_position = new_position;
		m_need_update = true;
	}

	glm::vec2 Camera2D::get_position()
	{
		return m_position;
	}

	void Camera2D::set_scale(float scale)
	{
		m_scale = scale;
		m_need_update = true;

	}

	float Camera2D::get_scale()
	{
		return m_scale;
	}

	glm::mat4 Camera2D::get_cam_matrix()
	{
		return m_cam_matrix;
	}

	glm::vec2 Camera2D::get_world_coords(glm::vec2 screen_coords)
	{
		screen_coords.y = m_screen_height - screen_coords.y;

		screen_coords -= glm::vec2(m_screen_width / 2, m_screen_height / 2);
		screen_coords /= m_scale;
		screen_coords += m_position;

		return screen_coords;
	}

	int Camera2D::get_screen_width()
	{
		return m_screen_width;
	}

	int Camera2D::get_screen_height()
	{
		return m_screen_height;
	}

}