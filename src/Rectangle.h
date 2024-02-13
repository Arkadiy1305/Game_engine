#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "VAO.h"
#include "ShaderClass.h"

class Rectangle {
public:
    Rectangle() = delete;
    Rectangle(const glm::vec2& size, ShaderClass& rectShader);
    ~Rectangle();
    void render(const glm::vec2& position, const glm::vec3& color);

    glm::vec2 const& get_size() { return m_size; };

private:
    ShaderClass& m_rectShader;
    VAO m_vao;
    const glm::vec2 m_size;
};

Rectangle::Rectangle(glm::vec2 const& size, ShaderClass& rectShader)
    : m_rectShader {rectShader}
    , m_size {size}
{
    float vertices[] = 
    {
        0.f, 0.f,
        size.x, 0.f,
        size.x, size.y,
        0.f, size.y,
    };
    unsigned int indicies[] = {
        0, 1, 2,
        2, 3, 0,
    };
    m_vao.loadVBO(sizeof(vertices), vertices, 2, GL_STATIC_DRAW);
    m_vao.loadEBO(sizeof(indicies), indicies);
}

Rectangle::~Rectangle() { }

inline void Rectangle::render(glm::vec2 const& position, glm::vec3 const& color) {
    m_rectShader.activate();
    m_rectShader.setVec3("Color", color);
    m_vao.bind();
    glm::mat4 model { 1.f };
    model = glm::translate(model, glm::vec3 { position, 0.f });
    m_rectShader.setMat4("model", model);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
