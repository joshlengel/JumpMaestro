#pragma once

#include<glad/glad.h>

#include<vector>
#include<map>

struct GLFWwindow;

class Texture;

struct Quad
{
    float x, y;
    float width, height;

    float r, g, b, a;

    Texture *texture = nullptr;
};

class QuadRenderer
{
public:
    QuadRenderer();
    ~QuadRenderer();

    void Add(const Quad &quad);
    void Render(GLFWwindow *window);

private:
    std::map<Texture*, std::vector<Quad>> m_quads;
    size_t m_num_indices;

    GLuint m_vao_id;
    GLuint m_buff_ids[2];

    GLuint m_program_id;
    GLuint m_shader_ids[2];

    GLint m_aspect_ratio_location;

    Texture *m_default_texture;
};