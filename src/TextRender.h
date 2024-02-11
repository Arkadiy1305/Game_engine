#pragma once
#include <iostream>
#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <string>
#include "ShaderClass.h"
#include "VAO.h"

struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2 Size; // Size of glyph
    glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
    unsigned int Advance; // Horizontal offset to advance to next glyph
};

std::map<GLchar, Character> Characters;

class TextRender {
public:
    TextRender(glm::mat4 const& projection, char const* pathFont);
    ~TextRender();
    void render(const std::string& text, float x, float y, float scale, const glm::vec3& color);

private:
    ShaderClass m_shader;
    VAO m_vao;
};

TextRender::TextRender(const glm::mat4& projection, const char* pathFont)
    : m_shader("./resources/shaders/text.vert", "./resources/shaders/text.frag")
{
    
    m_shader.setMat4("projection", projection);
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        
    }

    // load font as face
    FT_Face face;
    if (FT_New_Face(ft, pathFont, 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        
    } else {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++) {
            // Load character glyph
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
                face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character
                = { texture, glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                      glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                      static_cast<unsigned int>(face->glyph->advance.x) };
            Characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    std::cout << Characters.size() << '\n';
    m_vao.loadVBO(sizeof(float) * 6 * 4, NULL, 4, GL_DYNAMIC_DRAW);
}

TextRender::~TextRender() { }

inline void TextRender::render(
    std::string const& text, float x, float y, float scale, glm::vec3 const& color)
{
    m_shader.setVec3("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    m_vao.bind();

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = { { xpos, ypos + h, 0.0f, 0.0f }, { xpos, ypos, 0.0f, 1.0f },
            { xpos + w, ypos, 1.0f, 1.0f },

            { xpos, ypos + h, 0.0f, 0.0f }, { xpos + w, ypos, 1.0f, 1.0f },
            { xpos + w, ypos + h, 1.0f, 0.0f } };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        m_vao.changeVBOtext(sizeof(vertices), &vertices[0][0]);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide
                                        // amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
