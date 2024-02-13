#pragma once

#include "Rectangle.h"
#include <list>

using position = glm::ivec2;

enum DIRECTION {
    UP,
    LEFT,
    DOWN,
    RIGHT,
};

class Snake {
public:
    Snake() = delete;
    Snake(glm::vec2 const& sizeRect, ShaderClass& shader, glm::ivec2 const& split);
    ~Snake();
    void update();
    void render();
    void input(int key);

    size_t& get_snake_lenght() { return m_snake_lenght; }

private:
    Rectangle m_rect;
    const glm::vec3 m_color { 0.f, 1.f, 0.f };
    std::list<position> m_body;
    const glm::ivec2 m_field;
    DIRECTION m_direction { RIGHT };
    size_t m_snake_lenght { 3 };
    position m_apple;
    const glm::vec3 m_apple_color { 1.f, 0.f, 0.f };
    void move();
    bool eat_apple();
    void replace_apple();
};

Snake::Snake(glm::vec2 const& sizeRect, ShaderClass& shader, glm::ivec2 const& split)
    : m_rect {sizeRect, shader}
    , m_field {split}
    , m_apple { rand() % split.x, rand() % split.y }
{
    m_body.push_back(glm::ivec2{split.x / 2, split.y / 2});
    m_body.push_back(glm::ivec2 { (split.x / 2) - 1, split.y / 2 });
    m_body.push_back(glm::ivec2 { (split.x / 2) - 2, split.y / 2 });
}

Snake::~Snake() { }

inline void Snake::update() { 
    if (!eat_apple()) {
        m_body.pop_back();
    } else {
        replace_apple();
        m_snake_lenght++;
    }   
    move(); 
}

inline void Snake::render() {
    for (auto& posRect : m_body)
    {
        m_rect.render(glm::vec2 { posRect.x * m_rect.get_size().x, posRect.y * m_rect.get_size().y }, m_color);
    }
    m_rect.render(glm::vec2 { m_apple.x * m_rect.get_size().x, m_apple.y * m_rect.get_size().y }, m_apple_color);
}

inline void Snake::input(int key) {
    switch (key) {
    case GLFW_KEY_UP:
        if (m_direction != DOWN) {
            m_direction = UP;
        }
        break;
    case GLFW_KEY_LEFT:
        if (m_direction != RIGHT) {
            m_direction = LEFT;
        }
        break;
    case GLFW_KEY_DOWN:
        if (m_direction != UP) {
            m_direction = DOWN;
        }
        break;
    case GLFW_KEY_RIGHT:
        if (m_direction != LEFT) {
            m_direction = RIGHT;
        }
        break;
    default:
        break;
    }
}

inline void Snake::move() { 
    position head { 0 };
    switch (m_direction) {
    case UP:
        head = position(m_body.begin()->x, m_body.begin()->y + 1);
        m_body.push_front(std::move(head));
        break;
    case LEFT:
        head = position(m_body.begin()->x - 1, m_body.begin()->y);
        m_body.push_front(std::move(head));
        break;
    case DOWN:
        head = position(m_body.begin()->x, m_body.begin()->y - 1);
        m_body.push_front(std::move(head));
        break;
    case RIGHT:
        head = position(m_body.begin()->x + 1, m_body.begin()->y);
        m_body.push_front(std::move(head));
        break;
    default:
        break;
    }
}

inline bool Snake::eat_apple() { 
    if (m_body.front() == m_apple) {
        return true;
    }
    return false; 
}

inline void Snake::replace_apple() { m_apple = position(rand() % m_field.x, rand() % m_field.y); }
