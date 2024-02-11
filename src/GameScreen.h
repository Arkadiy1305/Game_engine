#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "VAO.h"
#include "ShaderClass.h"
#include "TextRender.h"


class GameScreen {
public:
    GameScreen() = delete;
    GameScreen(const glm::mat4& projection, const GLFWvidmode* mode);
    ~GameScreen();
    void update();
    void render();

private:
    TextRender m_textRender;
    ShaderClass m_shader;
    VAO m_vao;
    const glm::vec3 m_color { glm::vec3 { 0.2f, 0.2f, 0.2f } };
    const double m_left_side_info_panel;
    const double m_top_side_info_panel;
};

GameScreen::GameScreen(glm::mat4 const& projection, const GLFWvidmode* mode)
    : m_shader("./resources/shaders/base.vert", "./resources/shaders/base.frag")
    , m_textRender(projection, "./resources/fonts/astron_boy_italic.ttf")
    , m_left_side_info_panel {mode->height * 1.4}
    , m_top_side_info_panel { mode->height * 1.f }
{
    m_shader.setMat4("projection", projection);
    float vertices[] = {
        m_left_side_info_panel, 0.f,
        mode->width, 0.f,
        mode->width, mode->height,
        mode->height * 1.4, mode->height,
    };
    unsigned int indicies[] = {
        0, 1, 2,
        2, 3, 0,
    };
    m_vao.loadVBO(sizeof(vertices), vertices, 2, GL_STATIC_DRAW);
    m_vao.loadEBO(sizeof(indicies), indicies);
}

GameScreen::~GameScreen() { }

inline void GameScreen::update() { }

inline void GameScreen::render() 
{
    m_shader.activate();
    m_vao.bind();
    m_shader.setVec3("Color", m_color);
    auto view = glm::mat4 { 1.f };
    m_shader.setMat4("view", view);
    auto model = glm::mat4 { 1.f };
    m_shader.setMat4("model", model);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    m_textRender.render("info text", m_left_side_info_panel + 10, m_top_side_info_panel / 2, 2.f, glm::vec3{0.f, 1.f, 0.f});
}
