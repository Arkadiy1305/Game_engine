#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "VAO.h"
#include "ShaderClass.h"
#include "TextRender.h"
#include "Rectangle.h"
#include <vector>
#include "GameOfLife.h"


class GameScreen {
public:
    GameScreen() = delete;
    GameScreen(const glm::mat4& projection, const GLFWvidmode* mode, ShaderClass& shader, const glm::ivec2& split);
    ~GameScreen();
    void update();
    void render();

private:
    TextRender m_textRender;
    ShaderClass& m_shader;
    VAO m_vao;
    const glm::vec3 m_color { glm::vec3 { 0.2f, 0.2f, 0.2f } };
    const double m_left_side_info_panel;
    const double m_top_side_info_panel;
    Rectangle m_rect;
    const glm::ivec2 m_split;
    std::vector<std::vector<bool>> m_arr;
    GameOfLife gol;
};

GameScreen::GameScreen(glm::mat4 const& projection, GLFWvidmode const* mode, ShaderClass& shader, const glm::ivec2& split)
    : m_shader(shader)
    , m_textRender(projection, "./resources/fonts/astron_boy_italic.ttf")
    , m_left_side_info_panel {mode->height * 1.4}
    , m_top_side_info_panel { mode->height * 1.f }
    , m_rect { glm::vec2 {m_left_side_info_panel / split.x, m_top_side_info_panel / split.y}, shader }
    , m_split {split}
    , gol {m_rect, split}
{
    
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

    m_arr.resize(split.y);
    for (auto& arr : m_arr) {
        arr.resize(split.x);
    }

    for (size_t col = 0; col < m_split.y; col++) {
        for (size_t row = 0; row < m_split.x; row++) {            
            m_arr[col][row] = rand() % 2;            
        }
    }
}

GameScreen::~GameScreen() { }

inline void GameScreen::update() { gol.update(); }

inline void GameScreen::render() 
{
    unsigned int count { 0 };
    gol.render();
    m_shader.activate();
    m_vao.bind();
    m_shader.setVec3("Color", m_color);
    auto model = glm::mat4 { 1.f };
    m_shader.setMat4("model", model);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    m_textRender.render("Count rectangle: " + std::to_string(gol.get_count_cell()), m_left_side_info_panel + 10, m_top_side_info_panel / 2, 0.8f, glm::vec3{0.f, 1.f, 0.f});
}
