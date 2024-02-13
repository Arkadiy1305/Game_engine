#pragma once

#include "Rectangle.h"
#include <vector>
#include <iostream>

class GameOfLife {
public:
    GameOfLife() = delete;
    GameOfLife(Rectangle& rect, glm::ivec2 const& split);
    ~GameOfLife();
    void update();
    void render();

    size_t& get_count_cell() { return m_count_cell; };

private:
    Rectangle& m_rect;
    size_t m_count_cell;
    std::vector<std::vector<bool>> m_field1;
    std::vector<std::vector<bool>> m_field2;
    std::vector<std::vector<bool>>* m_field_current;
    void resize_vector(std::vector<std::vector<bool>>& vec, glm::ivec2 const& split);
    void fill_vector_zero(std::vector<std::vector<bool>>& vec);
    void fill_vector_random(std::vector<std::vector<bool>>& vec);
    void refill_vector(std::vector<std::vector<bool>>* vector_current, std::vector<std::vector<bool>>& new_vec);
};

GameOfLife::GameOfLife(Rectangle& rect, glm::ivec2 const& split)
    : m_rect { rect }
{
    resize_vector(m_field1, split);
    resize_vector(m_field2, split);
    fill_vector_zero(m_field2);
    fill_vector_random(m_field1);
    m_field_current = &m_field1;
}

GameOfLife::~GameOfLife() { }

inline void GameOfLife::update() {
    if (m_field_current == &m_field1) {
        refill_vector(m_field_current, m_field2);
        m_field_current = &m_field2;
    } else {
        refill_vector(m_field_current, m_field1);
        m_field_current = &m_field1;
    }
}

inline void GameOfLife::render() { 
    for (size_t col = 0; col < m_field1.size(); col++) {
        for (size_t row = 0; row < m_field1[0].size(); row++) {
            if (m_field_current->at(col)[row]) {
                
                m_rect.render(glm::vec2 { row * (m_rect.get_size().x), col * (m_rect.get_size().y) },
                    glm::vec3 { 0.f, 0.f, 0.f });
            }
        }
    }
}

inline void GameOfLife::resize_vector(std::vector<std::vector<bool>>& vec, glm::ivec2 const& split)
{
    vec.resize(split.y);
    for (auto& arr : vec) {
        arr.resize(split.x);
    }
}

inline void GameOfLife::fill_vector_zero(std::vector<std::vector<bool>>& vec) 
{ 
    for (size_t i = 0; i < vec.size(); i++) {
        for (size_t j = 0; j < vec[0].size(); j++) {
            vec[i][j] = 0;
        }
    }
}

inline void GameOfLife::fill_vector_random(std::vector<std::vector<bool>>& vec)
{
    for (size_t i = 0; i < vec.size(); i++) {
        for (size_t j = 0; j < vec[0].size(); j++) {
            vec[i][j] = static_cast<bool>(rand() % 2);
        }
    }
}

inline void GameOfLife::refill_vector(std::vector<std::vector<bool>>* vector_current, std::vector<std::vector<bool>>& new_vec)
{
    fill_vector_zero(new_vec);
    m_count_cell = 0;
    size_t count_cell_around { 0 };
    for (size_t i = 0; i < new_vec.size(); i++) {
        for (size_t j = 0; j < new_vec[0].size(); j++) {
            //std::cout << "loop " << i << j << '\n'; 
            count_cell_around = 0;
            for (int k = i - 1; k <= i + 1; k++) {
                for (int l = j - 1; l <= j + 1; l++) {
                    //std::cout << i << ' ' << j << ' ' << k << ' ' << l << ' ' << count_cell << '\n';
                    if (k < 0 || k > new_vec.size() - 1 || l < 0 || l > new_vec[0].size() - 1 || (k == i && l == j)) {
                        continue;
                    } 
                        if (vector_current->at(k)[l]) {
                        count_cell_around++;
                        }
                    
                }
            }
            
            if (vector_current->at(i)[j]) {
                if (count_cell_around < 2 || count_cell_around > 3) {
                    new_vec[i][j] = false;
                } else {
                    new_vec[i][j] = true;
                    m_count_cell++;
                }
            } else {
                if (count_cell_around == 3) {
                    new_vec[i][j] = true;
                    m_count_cell++;
                }
            }
            
        }
    }
    
}
