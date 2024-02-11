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
	void loadVBO(GLsizeiptr size, GLfloat* data, GLint count, GLenum usage);
	void loadEBO(GLsizeiptr size, unsigned int* indicies);
	void changeVBOtext(GLsizeiptr size, GLfloat* data);

private:
	unsigned int m_ID{ 0 };
	std::vector<unsigned int> VBOvector{};
	unsigned int m_EBO{ 0 };
};

VAO::VAO()
{
	glGenVertexArrays(1, &m_ID);
}

VAO::~VAO()
{
	glDeleteVertexArrays(1, &m_ID);
}

inline void VAO::loadVBO(GLsizeiptr size, GLfloat* data, GLint count, GLenum usage)
{
	unsigned int VBO;
	bind();
	glGenBuffers(1, &VBO);	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
	glVertexAttribPointer(VBOvector.size(), count, GL_FLOAT, GL_FALSE, count * sizeof(float), nullptr);
	glEnableVertexAttribArray(VBOvector.size());
	VBOvector.push_back(VBO);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	unbind();
}


inline void VAO::loadEBO(GLsizeiptr size, unsigned int* indicies)
{
	bind();
	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indicies, GL_STATIC_DRAW);
	unbind();
}

inline void VAO::changeVBOtext(GLsizeiptr size, GLfloat* data)
{ 
	
    glBindBuffer(GL_ARRAY_BUFFER, VBOvector[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
