#include <iostream>
#include <string>
#include <array>

#include <SDL.h>
#include "main_game.hpp"
#include "Sprite.hpp"
#include <glew.h>

Sprite::Sprite()
{
	m_vboID = 0;
}

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

	std::array<float,6*2> vertex_data;
	vertex_data.at(0) = x + width;
	vertex_data.at(1) = y + width;

	vertex_data.at(2) = x;
	vertex_data.at(3) = y + height;

	vertex_data.at(4) = x;
	vertex_data.at(5) = y;

	vertex_data.at(6) = x;
	vertex_data.at(7) = y;

	vertex_data.at(8) = x + width;
	vertex_data.at(9) = y;

	vertex_data.at(10) = x + width;
	vertex_data.at(11) = y + height;


	glBindBuffer(GL_ARRAY_BUFFER,m_vboID);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertex_data),&vertex_data,GL_STATIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Sprite::draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(0, 2, GL_FLOAT,GL_FALSE,0,0);
	glDrawArrays(GL_TRIANGLES, 0, 6);





	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
