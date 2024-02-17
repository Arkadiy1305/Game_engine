#pragma once

#include "Rectangle.h"
#include <list>
#include <oalpp/sound.hpp>
#include <oalpp/sound_context.hpp>
#include <oalpp/sound_data.hpp>
#include <thread>

void playSnd(oalpp::Sound& snd)
{ 
    snd.play();
    try {
        while (snd.isPlaying()) {
            snd.update();
        }
    } catch (std::exception const&) {

    }
    
}

using position = glm::ivec2;

enum DIRECTION {
    UP,
    LEFT,
    DOWN,
    RIGHT,
};

enum APPLE_DIRECTION {
    LEFT_UP,
    ONLY_UP,
    RIGHT_UP,
    ONLY_LEFT,
    ONLY_RIGHT,
    LEFT_DOWN,
    ONLY_DOWN,
    RIGHT_DOWN,
};

class Snake {
public:
    Snake() = delete;
    Snake(glm::vec2 const& sizeRect, ShaderClass& shader, glm::ivec2 const& split);
    ~Snake();
    void update();
    void render();
    void input(int key);

    std::string get_apple_direction();

    size_t& get_snake_lenght() { return m_snake_lenght; }

private:
    Rectangle m_rect;
    const glm::vec3 m_color { 0.f, 1.f, 0.f };
    std::list<position> m_body;
    const glm::ivec2 m_field;
    DIRECTION m_direction { RIGHT };
    size_t m_snake_lenght { 3 };
    position m_apple;
    APPLE_DIRECTION m_apple_direction;
    const glm::vec3 m_apple_color { 1.f, 0.f, 0.f };
    
    oalpp::SoundDataBuilder builder;    
    oalpp::Sound snd { oalpp::SoundData(builder.fromFile("./resources/sound/poedanie-ukus-yabloka.mp3").create()) };
    void move();
    bool eat_apple();
    void replace_apple();
    void calculate_apple_direction();
    void change_snake_direction();
    bool check_collision_with_body();
    bool check_collision_with_border();
    void init();
};

Snake::Snake(glm::vec2 const& sizeRect, ShaderClass& shader, glm::ivec2 const& split)
    : m_rect {sizeRect, shader}
    , m_field {split}
    , m_apple { rand() % split.x, rand() % split.y }
{
    init();
    
}

Snake::~Snake() { }

inline void Snake::init() { 
    m_body.clear();
    m_body.push_back(glm::ivec2 { m_field.x / 2, m_field.y / 2 });
    m_body.push_back(glm::ivec2 { (m_field.x / 2) - 1, m_field.y / 2 });
    m_body.push_back(glm::ivec2 { (m_field.x / 2) - 2, m_field.y / 2 });
    replace_apple();
    m_snake_lenght = 3;
}

inline void Snake::update() { 
    
    if (!check_collision_with_body() && !check_collision_with_border()) {
        if (!eat_apple()) {
            m_body.pop_back();
        } else {
            snd.play();
            try {
                snd.update();
            } catch (std::exception const&) {
                std::cout << "audio Error" << '\n';
            }
            replace_apple();
            m_snake_lenght++;
        }
        calculate_apple_direction();
        change_snake_direction();
        move();
    } else {
        init();
    }
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

void Snake::calculate_apple_direction()
{
    if (m_body.front().x > m_apple.x) {
        if (m_body.front().y > m_apple.y) {
            m_apple_direction = LEFT_DOWN;
        } else if (m_body.front().y < m_apple.y) {
            m_apple_direction = LEFT_UP;
        } else {
            m_apple_direction = ONLY_LEFT;
        }
    } else if (m_body.front().x < m_apple.x) {
        if (m_body.front().y > m_apple.y) {
            m_apple_direction = RIGHT_DOWN;
        } else if (m_body.front().y < m_apple.y) {
            m_apple_direction = RIGHT_UP;
        } else {
            m_apple_direction = ONLY_RIGHT;
        }
    } else {
        if (m_body.front().y > m_apple.y) {
            m_apple_direction = ONLY_DOWN;
        } else if (m_body.front().y < m_apple.y) {
            m_apple_direction = ONLY_UP;
        }
    } 
    
    
}

inline std::string Snake::get_apple_direction() { 
    switch (m_apple_direction) {
    case LEFT_UP:
        return "LEFT_UP";        
    case ONLY_UP:
        return "ONLY_UP";
    case RIGHT_UP:
        return "RIGHT_UP";
    case ONLY_LEFT:
        return "ONLY_LEFT";
    case ONLY_RIGHT:
        return "ONLY_RIGHT";
    case LEFT_DOWN:
        return "LEFT_DOWN";
    case ONLY_DOWN:
        return "ONLY_DOWN";
    case RIGHT_DOWN:
        return "RIGHT_DOWN";
    default:
        break;
    }
}

inline void Snake::change_snake_direction() { 
    switch (m_apple_direction) {
    case LEFT_UP:
        switch (m_direction) {
        case UP:
            break;
        case LEFT:
            break;
        case DOWN:
            m_direction = LEFT;
            break;
        case RIGHT:
            m_direction = UP;
            break;
        default:
            break;
        }
        break;
    case ONLY_UP:
        switch (m_direction) {
        case UP:
            break;
        case LEFT:
            m_direction = UP;
            break;
        case DOWN:
            m_direction = LEFT;
            break;
        case RIGHT:
            m_direction = UP;
            break;
        default:
            break;
        }
        break;
    case RIGHT_UP:
        switch (m_direction) {
        case UP:
            break;
        case LEFT:
            m_direction = UP;
            break;
        case DOWN:
            m_direction = RIGHT;
            break;
        case RIGHT:
            break;
        default:
            break;
        }
        break;
    case ONLY_LEFT:
        switch (m_direction) {
        case UP:
            m_direction = LEFT;
            break;
        case LEFT:
            break;
        case DOWN:
            m_direction = LEFT;
            break;
        case RIGHT:
            m_direction = UP;
            break;
        default:
            break;
        }
        break;
    case ONLY_RIGHT:
        switch (m_direction) {
        case UP:
            m_direction = RIGHT;
            break;
        case LEFT:
            m_direction = DOWN;
            break;
        case DOWN:
            m_direction = RIGHT;
            break;
        case RIGHT:
            break;
        default:
            break;
        }
        break;
    case LEFT_DOWN:
        switch (m_direction) {
        case UP:
            m_direction = LEFT;
            break;
        case LEFT:
            break;
        case DOWN:
            break;
        case RIGHT:
            m_direction = DOWN;
            break;
        default:
            break;
        }
        break;
    case ONLY_DOWN:
        switch (m_direction) {
        case UP:
            m_direction = LEFT;
            break;
        case LEFT:
            m_direction = DOWN;
            break;
        case DOWN:
            break;
        case RIGHT:
            m_direction = DOWN;
            break;
        default:
            break;
        }
        break;
    case RIGHT_DOWN:
        switch (m_direction) {
        case UP:
            m_direction = RIGHT;
            break;
        case LEFT:
            m_direction = DOWN;
            break;
        case DOWN:
            break;
        case RIGHT:
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

inline bool Snake::check_collision_with_body()
{
    unsigned int count_collision { 0 };
    for (auto& snake_element : m_body) { 
        if (snake_element == m_body.front()) {
            count_collision++;
        }
    }
    if (count_collision > 1) {
        return true;
    }
    return false;
}

inline bool Snake::check_collision_with_border() 
{
    if (m_body.front().x < 0 || m_body.front().x > m_field.x || m_body.front().y < 0
        || m_body.front().y > m_field.y) {
        return true;
    }
    return false;
}