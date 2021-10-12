#pragma once

#include<string>
#include<map>
#include<vector>

#include<glad/glad.h>

struct GLFWwindow;

class Texture;

class Glyph
{
public:
    int x, y;
    int w, h;
    int off_x, off_y;
    int adv_x;
};

class Font
{
public:
    Font(const std::string &fnt, const std::string &image, int desired_padding);
    ~Font();

    const Glyph &LoadGlyph(char c) const;
    Texture *GetTexture() const;

    void GetPadding(int dst[4]) const;
    int GetLineHeight() const;
    int GetDesiredPadding() const;

private:
    int m_padding[4];
    int m_line_height;
    int m_desired_padding;

    std::map<char, Glyph> m_glyphs;
    Texture *m_texture;
};

class Text
{
public:
    Text(const Font &font, const std::string &str, float scale);
    ~Text();

    float GetR() const;
    float GetG() const;
    float GetB() const;
 
    void SetColor(float r, float g, float b);

    void Render();

private:
    GLuint m_vao_id;
    GLuint m_buff_ids[2];
    size_t m_count;

    float m_r, m_g, m_b;

    Texture *m_texture;
};

class TextRenderer
{
public:
    TextRenderer();
    ~TextRenderer();

    void Add(Text *text);
    void Render(GLFWwindow *window);

private:
    std::vector<Text*> m_texts;

    GLuint m_program_id;
    GLuint m_shader_ids[2];
    GLint m_aspect_ratio_location;
    GLint m_color_location;
};