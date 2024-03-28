#include "Shader.h"
#include <fstream>
#include <sstream>
#include <string>
#include <glad/glad.h>
#include <iostream>
#include "Debug.h"


Shader::Shader() {
	id = glCreateProgram();
}

void 
Shader::loadVFShader(const char* vertexPath, const char* fragmentPath) const {
	unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vertex_code;
	std::string fragment_code;
	std::ifstream vertex_file;
	std::ifstream fragment_file;

	try {
		vertex_file.open(vertexPath);
		fragment_file.open(fragmentPath);

		std::stringstream vertex_stream;
		std::stringstream fragment_stream;

		vertex_stream << vertex_file.rdbuf();
		fragment_stream << fragment_file.rdbuf();

		vertex_file.close();
		fragment_file.close();

		vertex_code = vertex_stream.str();
		fragment_code = fragment_stream.str();
	} catch (std::ifstream::failure& e) {
		std::cerr << "Failed to read shader file" << std::endl;
		exit(1);
	}

	const char* vertex_code_c = vertex_code.c_str();
	const char* fragment_code_c = fragment_code.c_str();

	glShaderSource(vertex_shader, 1, &vertex_code_c, nullptr);
	glShaderSource(fragment_shader, 1, &fragment_code_c, nullptr);

	glCompileShader(vertex_shader);
	glCompileShader(fragment_shader);

	int success;
	char info_log[512];

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
		std::cerr << "Vertex shader compilation failed: " << info_log << std::endl;
		exit(1);
	}

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
		std::cerr << "Fragment shader compilation failed: " << info_log << std::endl;
		exit(1);
	}

	glAttachShader(id, vertex_shader);
	glAttachShader(id, fragment_shader);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, nullptr, info_log);
		std::cerr << "Shader program linking failed: " << info_log << std::endl;
		exit(1);
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	DEBUG("Vertex and fragment shader loaded");
}

void
Shader::loadVGFShader(const char* vertexPath, const char* geometryPath, const char* fragmentPath) const {
	unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
	unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vertex_code;
	std::string geometry_code;
	std::string fragment_code;
	std::ifstream vertex_file;
	std::ifstream geometry_file;
	std::ifstream fragment_file;

	try {
		vertex_file.open(vertexPath);
		geometry_file.open(geometryPath);
		fragment_file.open(fragmentPath);

		std::stringstream vertex_stream;
		std::stringstream geometry_stream;
		std::stringstream fragment_stream;

		vertex_stream << vertex_file.rdbuf();
		geometry_stream << geometry_file.rdbuf();
		fragment_stream << fragment_file.rdbuf();

		vertex_file.close();
		geometry_file.close();
		fragment_file.close();

		vertex_code = vertex_stream.str();
		geometry_code = geometry_stream.str();
		fragment_code = fragment_stream.str();
	} catch (std::ifstream::failure& e) {
		std::cerr << "Failed to read shader file" << std::endl;
		exit(1);
	}

	const char* vertex_code_c = vertex_code.c_str();
	const char* geometry_code_c = geometry_code.c_str();
	const char* fragment_code_c = fragment_code.c_str();

	glShaderSource(vertex_shader, 1, &vertex_code_c, nullptr);
	glShaderSource(geometry_shader, 1, &geometry_code_c, nullptr);
	glShaderSource(fragment_shader, 1, &fragment_code_c, nullptr);

	glCompileShader(vertex_shader);
	glCompileShader(geometry_shader);
	glCompileShader(fragment_shader);

	int success;
	char info_log[512];

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
		std::cerr << "Vertex shader compilation failed: " << info_log << std::endl;
		std::cerr << "Vertex code:\n" << vertex_code << std::endl;
		exit(1);
	}

	glGetShaderiv(geometry_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(geometry_shader, 512, nullptr, info_log);
		std::cerr << "Geometry shader compilation failed: " << info_log << std::endl;
		exit(1);
	}

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
		std::cerr << "Fragment shader compilation failed: " << info_log << std::endl;
		exit(1);
	}

	glAttachShader(id, vertex_shader);
	glAttachShader(id, geometry_shader);
	glAttachShader(id, fragment_shader);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, nullptr, info_log);
		std::cerr << "Shader program linking failed: " << info_log << std::endl;
		exit(1);
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(geometry_shader);
	glDeleteShader(fragment_shader);
	DEBUG("Vertex, geometry and fragment shader loaded");
}

void
Shader::use() const {
	glUseProgram(id);
}