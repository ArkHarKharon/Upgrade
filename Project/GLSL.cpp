#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <vector>

#include <SDL.h>
#include <glew.h>

#include "GLSL.hpp"
#include "Errors.hpp"

GLSLProgram::GLSLProgram():
	m_program_ID{0},
	m_vertex_shader_ID{0},
	m_fragment_shader_ID{0},
	m_attributes_num{0}
{}

GLSLProgram::~GLSLProgram()
{

}

void GLSLProgram::compile_shaders(const std::string& vetrex_shader_filepath, const std::string& fragment_shader_filepath)
{
	m_program_ID = glCreateProgram();

	m_vertex_shader_ID = glCreateShader(GL_VERTEX_SHADER);
	if (m_vertex_shader_ID == 0)
		fatal_error("Вершиинный шейдер НЕ был создан!");

	m_fragment_shader_ID = glCreateShader(GL_FRAGMENT_SHADER);
	if (m_fragment_shader_ID == 0)
		fatal_error("Фрагментный шейдер НЕ был создан!");

	compile_shader(vetrex_shader_filepath, m_vertex_shader_ID);
	compile_shader(fragment_shader_filepath, m_fragment_shader_ID);

}

void GLSLProgram::link_shaders()
{
	glAttachShader(m_program_ID, m_vertex_shader_ID);
	glAttachShader(m_program_ID, m_fragment_shader_ID);

	glLinkProgram(m_program_ID);

	GLint isLinked = 0;
	glGetProgramiv(m_program_ID, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_program_ID, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(m_program_ID, maxLength, &maxLength, &infoLog[0]);

		glDeleteProgram(m_program_ID);
		glDeleteShader(m_vertex_shader_ID);
		glDeleteShader(m_fragment_shader_ID);

		fatal_error("Шейдер не был линкован!");
	}

	glDetachShader(m_program_ID, m_vertex_shader_ID);
	glDetachShader(m_program_ID, m_fragment_shader_ID);
}

void GLSLProgram::add_attribute(const std::string attribute_name)
{
	glBindAttribLocation(m_program_ID, m_attributes_num++, attribute_name.c_str());
}

void GLSLProgram::use()
{
	glUseProgram(m_program_ID);
	for (int i = 0; i < m_attributes_num; i++)
		glEnableVertexAttribArray(i);
}

void GLSLProgram::unuse()
{
	glUseProgram(0);
	for (int i = 0; i < m_attributes_num; i++)
		glDisableVertexAttribArray(i);

}

void GLSLProgram::compile_shader(const std::string& shader_filepath, GLuint &id)
{
	std::fstream vertex_file(shader_filepath);
	if (vertex_file.fail())
		fatal_error("Файл шейдера НЕ был открыт: " + shader_filepath);

	std::string file_data = "";
	std::string line{};

	while (std::getline(vertex_file, line))
		file_data += line + "\n";

	vertex_file.close();

	const char* data_ptr = file_data.c_str();
	glShaderSource(id, 1, &data_ptr, nullptr);
	glCompileShader(id);

	GLint success = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> error_log(maxLength);
		glGetShaderInfoLog(id, maxLength, &maxLength, &error_log[0]);

		glDeleteShader(id);
		return;

		for (auto ch : error_log)
			std::cout << ch;
		fatal_error("Шейдер НЕ был скомпилирован!");
	}
}

GLint GLSLProgram::get_uniform_location(const std::string uniform_name)
{
	GLint location = glGetUniformLocation(m_program_ID, uniform_name.c_str());
	if (location == GL_INVALID_INDEX)
		fatal_error("Форма " + uniform_name + " не найдена в шейдере!");
	return location;
}
