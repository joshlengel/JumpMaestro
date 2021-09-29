#pragma once

#include<GL/glew.h>

#include<vector>

struct SDL_Window;

struct Quad
{
    float x, y;
    float width, height;

    float r, g, b, a;
};

class QuadRenderer
{
public:
    QuadRenderer();
    ~QuadRenderer();

    void Add(const Quad &quad);
    void Flush();
    void Render(SDL_Window *window);
    void FlushAndRender(SDL_Window *window);

private:
    std::vector<Quad> m_quads;
    size_t m_num_indices;

    GLuint m_vao_id;
    GLuint m_buff_ids[2];

    GLuint m_program_id;
    GLuint m_shader_ids[2];

    GLint m_aspect_ratio_location;
};