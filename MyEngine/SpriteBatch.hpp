#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <iostream>
#include <algorithm>
#include <vector>

#include "Vertex.hpp"

namespace MyEngine
{
	enum class GlyphSort
	{
		NONE,
		FRONT_TO_BACK,
		BACK_TO_FRONT,
		TEXTURE
	};


	struct Glyph
	{
		GLuint texture;
		float depth;

		Vertex top_left;
		Vertex top_right;
		Vertex bottom_left;
		Vertex bottom_right;
	};


	class RenderBatch
	{
	public:
		GLuint m_offset;
		GLuint m_num_vert;
		GLuint m_texture;

		RenderBatch(GLuint offset, GLuint num_vert, GLuint texture):
			m_offset{offset},
			m_num_vert{num_vert},
			m_texture{texture}
		{};
	};


	class SpriteBatch
	{
		GLuint m_vbo;
		GLuint m_vao;

		std::vector<Glyph*> m_glyphs;
		std::vector<RenderBatch> m_render_batches;

		GlyphSort m_sort_type;

		void create_render_batches();
		void create_vertex_array();
		void sort_glyphs();

		static bool compare_front_to_back(Glyph* a, Glyph* b) { return (a->depth < b->depth); };
		static bool compare_back_to_front(Glyph* a, Glyph* b) { return (a->depth > b->depth); };
		static bool compare_texture(Glyph* a, Glyph* b) { return (a->texture < b->texture); };

		glm::vec2 rotate_point(glm::vec2 position, float angle);
	public:
		SpriteBatch();
		~SpriteBatch();

		void init();

		void begin(GlyphSort sort_type = GlyphSort::TEXTURE);

		void draw(const glm::vec4& destination_rect, const glm::vec4& uv_rect, GLuint texture,float depth, const Color& color, float angle);
		void draw(const glm::vec4& destination_rect, const glm::vec4& uv_rect, GLuint texture, float depth, const Color& color);
		void draw(const glm::vec4& destination_rect, const glm::vec4& uv_rect, GLuint texture, float depth, const Color& color, glm::vec2 dir_vec);

		//NonSym -- копии draw() для объектов, у которых ось вращения находится не по центру
		void non_sym_draw(int spin_x, int spin_y, const glm::vec4& destination_rect, const glm::vec4& uv_rect, GLuint texture, float depth, const Color& color, float angle);
		void non_sym_draw(int spin_x, int spin_y, const glm::vec4& destination_rect, const glm::vec4& uv_rect, GLuint texture, float depth, const Color& color, glm::vec2 dir_vec);



		void end();

		void render_batch();

	};
}