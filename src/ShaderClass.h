#pragma once
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderClass
{
public:
	ShaderClass() = delete;
	ShaderClass(const char* vertexPath, const char* fragmentPath);
	~ShaderClass();
	void activate();
	void setMat4(const char* name, const glm::mat4& value);
	void setVec3(const char* name, const glm::vec3& value);

private:
	unsigned int m_ID{ 0 };
	bool checkCompileErrors(unsigned int shader, const std::string& type);
	unsigned int loadshader(const char* shaderPath, GLenum shaderType, const std::string& type);
};



ShaderClass::ShaderClass(const char* vertexPath, const char* fragmentPath)
{
	unsigned int vertex, fragment;
	vertex = loadshader(vertexPath, GL_VERTEX_SHADER, "VERTEX");
	fragment = loadshader(fragmentPath, GL_FRAGMENT_SHADER, "FRAGMENT");
	if (vertex && fragment)
	{
		m_ID = glCreateProgram();
		glAttachShader(m_ID, vertex);
		glAttachShader(m_ID, fragment);
		glLinkProgram(m_ID);
		checkCompileErrors(m_ID, "PROGRAM");
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

ShaderClass::~ShaderClass()
{
	glDeleteProgram(m_ID);
}

inline void ShaderClass::activate()
{
	glUseProgram(m_ID);
}

inline void ShaderClass::setMat4(const char* name, const glm::mat4& value)
{
	activate();
	glUniformMatrix4fv(glGetUniformLocation(m_ID, name), 1, GL_FALSE, glm::value_ptr(value));
}

inline void ShaderClass::setVec3(char const* name, glm::vec3 const& value) { 
	activate();
    glUniform3f(glGetUniformLocation(m_ID, name), value.x, value.y, value.z);
}

inline bool ShaderClass::checkCompileErrors(unsigned int shader, const std::string& type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cerr << "ERROR::SHADER::COMPILE_ERROR: " << type << '\n' << infoLog << '\n';
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		glGetShaderiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cerr << "ERROR::SHADER::LINK_ERROR: " << type << '\n' << infoLog << '\n';
			return false;
		}
		else
		{
			return true;
		}
	}
}

inline unsigned int ShaderClass::loadshader(const char* shaderPath, GLenum shaderType, const std::string& type)
{
	std::string code;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		shaderFile.open(shaderPath);
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		code = shaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << '/n';
	}
	const char* shaderCode = code.c_str();
	unsigned int shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);
	if (checkCompileErrors(shader, type))
	{
		return shader;
	}
	else
	{
		return 0;
	}
	
}
