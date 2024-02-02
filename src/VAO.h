#pragma once

#include <glad/glad.h>
#include <vector>

class VAO
{
public:
	VAO();
	~VAO();
	void bind() { glBindVertexArray(m_ID); }
	void unbind() { glBindVertexArray(0); }
	void loadVBO(GLsizeiptr size, GLfloat* data);

private:
	unsigned int m_ID{ 0 };
	std::vector<unsigned int> VBOvector{};
};

VAO::VAO()
{
	glGenVertexArrays(1, &m_ID);
}

VAO::~VAO()
{
	glDeleteVertexArrays(1, &m_ID);
}

inline void VAO::loadVBO(GLsizeiptr size, GLfloat* data)
{
	unsigned int VBO;
	bind();
	glGenBuffers(1, &VBO);	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glVertexAttribPointer(VBOvector.size(), 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
	glEnableVertexAttribArray(VBOvector.size());
	VBOvector.push_back(VBO);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	unbind();
}
