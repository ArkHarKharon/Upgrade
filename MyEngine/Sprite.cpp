#include <iostream>
#include <string>
#include <array>

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <cstddef>

#include "Sprite.hpp"
#include "Vertex.hpp"
#include "ResourceManager.hpp"

namespace MyEngine
{

	Sprite::Sprite() : m_vboID{ 0 }, m_texture{ 0 }
	{}

	Sprite::~Sprite()
	{
		if (m_vboID)
			glDeleteBuffers(1, &m_vboID);
	}


	void Sprite::init(float x, float y, float width, float height, std::string filepath)
	{
		m_x = x;
		m_y = y;
		m_width = width;
		m_height = height;

		m_texture = ResourceManager::get_texture(filepath);

		if (!m_vboID)
			glGenBuffers(1, &m_vboID);

		std::array<Vertex, 6> vertex_data{};

		vertex_data.at(0).set_position(x + width, y + height);
		vertex_data.at(1).set_position(x, y + height);
		vertex_data.at(2).set_position(x, y);
		vertex_data.at(3).set_position(x, y);
		vertex_data.at(4).set_position(x + width, y);
		vertex_data.at(5).set_position(x + width, y + height);

		vertex_data.at(0).set_color(255, 255, 255, 255);
		vertex_data.at(1).set_color(255, 255, 255, 255);
		vertex_data.at(2).set_color(255, 255, 255, 255);
		vertex_data.at(3).set_color(255, 255, 255, 255);
		vertex_data.at(4).set_color(255, 255, 255, 255);
		vertex_data.at(5).set_color(255, 255, 255, 255);

		vertex_data.at(0).set_UV(1.0f, 1.0f);
		vertex_data.at(1).set_UV(0.0f, 1.0f);
		vertex_data.at(2).set_UV(0.0f, 0.0f);
		vertex_data.at(3).set_UV(0.0f, 0.0f);
		vertex_data.at(4).set_UV(1.0f, 0.0f);
		vertex_data.at(5).set_UV(1.0f, 1.0f);

		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), &vertex_data, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Sprite::draw()
	{
		glBindTexture(GL_TEXTURE_2D, m_texture.id);

		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
		
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}