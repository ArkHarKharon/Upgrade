#pragma once

#include <iostream>
#include <Windows.h>
#include <SDL.h>
#include <glew.h>
#include <string>

namespace MyEngine
{
	class GLSLProgram
	{
		GLuint m_program_ID;

		GLuint m_vertex_shader_ID;
		GLuint m_fragment_shader_ID;
		int m_attributes_num;

		void compile_shader(const std::string& shader_filepath, GLuint& id);


	public:
		GLSLProgram();
		~GLSLProgram();

		void compile_shaders(const std::string& vetrex_shader_filepath, const std::string& fragment_shader_filepath);
		void link_shaders();
		void add_attribute(const std::string attribute_name);
		void use();
		void unuse();
		GLint get_uniform_location(const std::string uniform_name);
	};
}