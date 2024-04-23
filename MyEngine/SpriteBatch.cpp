#include "SpriteBatch.hpp"

namespace MyEngine
{
	SpriteBatch::SpriteBatch():
		m_vbo{0},
		m_vao{0},
		m_sort_type{GlyphSort::TEXTURE}
	{

	}

	SpriteBatch::~SpriteBatch()
	{

	}

	void SpriteBatch::init()
	{
		create_vertex_array();

	}

	void SpriteBatch::begin(GlyphSort sort_type)
	{
		m_sort_type = sort_type;
		m_render_batches.clear();

		for (int i = 0; i < m_glyphs.size(); i++)
			delete m_glyphs.at(i);

		m_glyphs.clear();
	}

	void SpriteBatch::draw(const glm::vec4& destination_rect, const glm::vec4& uv_rect, GLuint texture, float depth, const Color& color)
	{
		Glyph* new_glyph = new Glyph;
		new_glyph->texture = texture;
		new_glyph->depth = depth;

		new_glyph->top_left.set_position(destination_rect.x, destination_rect.y + destination_rect.w);
		new_glyph->top_left.color = color;
		new_glyph->top_left.set_UV(uv_rect.x, uv_rect.y + uv_rect.w);
		
		new_glyph->bottom_left.set_position(destination_rect.x, destination_rect.y );
		new_glyph->bottom_left.color = color;
		new_glyph->bottom_left.set_UV(uv_rect.x, uv_rect.y);

		new_glyph->top_right.set_position(destination_rect.x + destination_rect.z, destination_rect.y + destination_rect.w);
		new_glyph->top_right.color = color;
		new_glyph->top_right.set_UV(uv_rect.x + uv_rect.z, uv_rect.y + uv_rect.w);

		new_glyph->bottom_right.set_position(destination_rect.x + destination_rect.z, destination_rect.y );
		new_glyph->bottom_right.color = color;
		new_glyph->bottom_right.set_UV(uv_rect.x + uv_rect.z, uv_rect.y);

		m_glyphs.push_back(new_glyph);

	}

	void SpriteBatch::end()
	{
		sort_glyphs();
		create_render_batches();
	}

	void SpriteBatch::render_batch()
	{
		glBindVertexArray(m_vao);

		for (int i = 0; i < m_render_batches.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D, m_render_batches.at(i).m_texture);
			glDrawArrays(GL_TRIANGLES, m_render_batches.at(i).m_offset, m_render_batches.at(i).m_num_vert);
		}
		glBindVertexArray(0);
	}

	void SpriteBatch::create_render_batches()
	{
		std::vector<Vertex> verticies;
		verticies.resize(m_glyphs.size() * 6);

		if (m_glyphs.empty())
			return;

		int offset = 0;

		m_render_batches.emplace_back(0,6,m_glyphs.at(0)->texture);

		int current_vert = 0;
		verticies.at(current_vert++) = m_glyphs.at(0)->top_left;
		verticies.at(current_vert++) = m_glyphs.at(0)->bottom_left;
		verticies.at(current_vert++) = m_glyphs.at(0)->bottom_right;
		verticies.at(current_vert++) = m_glyphs.at(0)->bottom_right;
		verticies.at(current_vert++) = m_glyphs.at(0)->top_right;
		verticies.at(current_vert++) = m_glyphs.at(0)->top_left;
		offset += 6;

		for (int current_glyph = 1; current_glyph < m_glyphs.size(); current_glyph++)
		{
			if (m_glyphs.at(current_glyph)->texture != m_glyphs.at(current_glyph - 1)->texture)
				m_render_batches.emplace_back(offset, 6, m_glyphs.at(0)->texture);
			else
				m_render_batches.back().m_num_vert += 6;

			verticies.at(current_vert++) = m_glyphs.at(current_glyph)->top_left;
			verticies.at(current_vert++) = m_glyphs.at(current_glyph)->bottom_left;
			verticies.at(current_vert++) = m_glyphs.at(current_glyph)->bottom_right;
			verticies.at(current_vert++) = m_glyphs.at(current_glyph)->bottom_right;
			verticies.at(current_vert++) = m_glyphs.at(current_glyph)->top_right;
			verticies.at(current_vert++) = m_glyphs.at(current_glyph)->top_left;
			offset += 6;
		}
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, verticies.size() * sizeof(Vertex), verticies.data());

	}


	void SpriteBatch::create_vertex_array()
	{
		if(m_vao == 0) ///
			glGenVertexArrays(1,&m_vao);
		glBindVertexArray(m_vao);

		if(!m_vbo)
			glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));		//Атрибут позиции
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color)); //Атрибут цвета
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));			//Атрибут UV

		glBindBuffer(GL_ARRAY_BUFFER,0);

	}

	void SpriteBatch::sort_glyphs()
	{
		switch (m_sort_type)
		{
		case(GlyphSort::NONE):
			break;

		case(GlyphSort::BACK_TO_FRONT):
			std::stable_sort(m_glyphs.begin(),m_glyphs.end(),compare_back_to_front);
			break;

		case(GlyphSort::FRONT_TO_BACK):
			std::stable_sort(m_glyphs.begin(), m_glyphs.end(), compare_front_to_back);
			break;

		case(GlyphSort::TEXTURE):
			std::stable_sort(m_glyphs.begin(), m_glyphs.end(), compare_texture);
			break;
		}
	}
}