#include <iostream>
#include <string>
#include <array>

#include <SDL.h>
#include <glew.h>
#include <cstddef>

#include "main_game.hpp"
#include "Sprite.hpp"
#include "Vertex.hpp"

Sprite::Sprite(): m_vboID{0}
{}

Sprite::~Sprite()
{
	if (m_vboID)
		glDeleteBuffers(1, &m_vboID);
}


void Sprite::init(float x, float y, float width, float height)
{
	m_x = x;
	m_y = y;
	m_width = width;
	m_height = height;

	if (!m_vboID)
		glGenBuffers(1, &m_vboID);

	std::array<Vertex, 6> vertex_data{};
	vertex_data.at(0).position.x = x + width;
	vertex_data.at(0).position.y = y + height;

	vertex_data.at(1).position.x = x;
	vertex_data.at(1).position.y = y + height;

	vertex_data.at(2).position.x = x;
	vertex_data.at(2).position.y = y;

	vertex_data.at(3).position.x = x;
	vertex_data.at(3).position.y = y;

	vertex_data.at(4).position.x = x + width;
	vertex_data.at(4).position.y = y;

	vertex_data.at(5).position.x = x + width;
	vertex_data.at(5).position.y = y + height;


	vertex_data.at(0).color.blue = 255;
	vertex_data.at(0).color.red = 0;
	vertex_data.at(0).color.green = 0;
	vertex_data.at(0).color.alpha = 255;

	vertex_data.at(1).color.blue = 150;
	vertex_data.at(1).color.red = 150;
	vertex_data.at(1).color.green = 0;
	vertex_data.at(1).color.alpha = 255;

	vertex_data.at(2).color.blue = 0;
	vertex_data.at(2).color.red = 0;
	vertex_data.at(2).color.green = 255;
	vertex_data.at(2).color.alpha = 255;

	vertex_data.at(3).color.blue = 0;
	vertex_data.at(3).color.red = 0;
	vertex_data.at(3).color.green = 255;
	vertex_data.at(3).color.alpha = 255;

	vertex_data.at(4).color.blue = 0;
	vertex_data.at(4).color.red = 255;
	vertex_data.at(4).color.green = 0;
	vertex_data.at(4).color.alpha = 255;

	vertex_data.at(5).color.blue = 255;
	vertex_data.at(5).color.red = 0;
	vertex_data.at(5).color.green = 0;
	vertex_data.at(5).color.alpha = 255;



	



	glBindBuffer(GL_ARRAY_BUFFER,m_vboID);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertex_data),&vertex_data,GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sprite::draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,position)); //Атрибут позиции
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color)); //Атрибут цвета
	glDrawArrays(GL_TRIANGLES, 0, 6);





	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
