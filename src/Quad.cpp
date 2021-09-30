#include"Quad.h"

#include<iostream>
#include<string>

#include<SDL2/SDL.h>

static const char *const VERTEX_SOURCE = R"(
    #version 330

    layout(location = 0) in vec2 position;
    layout(location = 1) in vec2 uv;
    layout(location = 2) in vec4 color;

    out vec2 _uv;
    out vec4 _color;

    uniform float aspect_ratio;

    void main() {
        gl_Position = vec4(position.x / aspect_ratio, position.y, 0.0, 1.0);
        _uv = uv;
        _color = color;
    }
)";

static const char *const FRAGMENT_SOURCE = R"(
    #version 330

    in vec2 _uv;
    in vec4 _color;

    out vec4 fragment;

    uniform sampler2D tex;

    void main() {
        fragment = _color * texture2D(tex, _uv);
    }
)";

QuadRenderer::QuadRenderer()
{
    glGenVertexArrays(1, &m_vao_id);
    glGenBuffers(2, m_buff_ids);

    m_program_id = glCreateProgram();
    m_shader_ids[0] = glCreateShader(GL_VERTEX_SHADER);
    m_shader_ids[1] = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(m_shader_ids[0], 1, &VERTEX_SOURCE, nullptr);
    glShaderSource(m_shader_ids[1], 1, &FRAGMENT_SOURCE, nullptr);

    glCompileShader(m_shader_ids[0]);
    glCompileShader(m_shader_ids[1]);

    GLint status;

    glGetShaderiv(m_shader_ids[0], GL_COMPILE_STATUS, &status);
    if (!status)
    {
        GLint length;
        glGetShaderiv(m_shader_ids[0], GL_INFO_LOG_LENGTH, &length);

        std::string msg(length, '\0');
        glGetShaderInfoLog(m_shader_ids[0], length, nullptr, &msg[0]);

        std::cerr << "Error compiling vertex shader: " << msg << std::endl;
    }

    glGetShaderiv(m_shader_ids[1], GL_COMPILE_STATUS, &status);
    if (!status)
    {
        GLint length;
        glGetShaderiv(m_shader_ids[1], GL_INFO_LOG_LENGTH, &length);

        std::string msg(length, '\0');
        glGetShaderInfoLog(m_shader_ids[1], length, nullptr, &msg[0]);

        std::cerr << "Error compiling fragment shader: " << msg << std::endl;
    }

    glAttachShader(m_program_id, m_shader_ids[0]);
    glAttachShader(m_program_id, m_shader_ids[1]);

    glLinkProgram(m_program_id);

    glGetProgramiv(m_program_id, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        GLint length;
        glGetProgramiv(m_program_id, GL_INFO_LOG_LENGTH, &length);

        std::string msg(length, '\0');
        glGetProgramInfoLog(m_program_id, length, nullptr, &msg[0]);

        std::cerr << "Error linking program: " << msg << std::endl;
    }

    m_aspect_ratio_location = glGetUniformLocation(m_program_id, "aspect_ratio");

    glDetachShader(m_program_id, m_shader_ids[0]);
    glDetachShader(m_program_id, m_shader_ids[1]);

    glGenTextures(1, &m_default_texture);
    glBindTexture(GL_TEXTURE_2D, m_default_texture);

    unsigned char pixels[4] = { 255U, 255U, 255U, 255U };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

QuadRenderer::~QuadRenderer()
{
    glDeleteVertexArrays(1, &m_vao_id);
    glDeleteBuffers(2, m_buff_ids);

    glDeleteProgram(m_program_id);
    glDeleteShader(m_shader_ids[0]);
    glDeleteShader(m_shader_ids[1]);

    glDeleteTextures(1, &m_default_texture);
}

void QuadRenderer::Add(const Quad &quad)
{
    m_quads[quad.texture].push_back(quad);
}

void QuadRenderer::Render(SDL_Window *window)
{
    int ww, wh;
    SDL_GetWindowSize(window, &ww, &wh);
    float aspect_ratio = static_cast<float>(ww) / static_cast<float>(wh);

    glUseProgram(m_program_id);
    glUniform1f(m_aspect_ratio_location, aspect_ratio);

    glBindVertexArray(m_vao_id);

    for (auto &entry : m_quads)
    {
        GLuint texture = entry.first == -1? m_default_texture : entry.first;
        std::vector<Quad> &quads = entry.second;

        glBindTexture(GL_TEXTURE_2D, texture);

        std::vector<float> data;
        data.reserve(quads.size() * (2 + 2 + 4) * 4);

        std::vector<unsigned int> indices;
        indices.reserve(quads.size() * 6);

        size_t index = 0;

        for (const Quad &quad : quads)
        {
            float hw = quad.width * 0.5f;
            float hh = quad.height * 0.5f;

            // Vertex 1
            data.push_back(quad.x - hw);
            data.push_back(quad.y - hh);

            data.push_back(0.0f);
            data.push_back(1.0f);

            data.push_back(quad.r);
            data.push_back(quad.g);
            data.push_back(quad.b);
            data.push_back(quad.a);

            // Vertex 2
            data.push_back(quad.x + hw);
            data.push_back(quad.y - hh);

            data.push_back(1.0f);
            data.push_back(1.0f);

            data.push_back(quad.r);
            data.push_back(quad.g);
            data.push_back(quad.b);
            data.push_back(quad.a);

            // Vertex 3
            data.push_back(quad.x - hw);
            data.push_back(quad.y + hh);

            data.push_back(0.0f);
            data.push_back(0.0f);

            data.push_back(quad.r);
            data.push_back(quad.g);
            data.push_back(quad.b);
            data.push_back(quad.a);

            // Vertex 4
            data.push_back(quad.x + hw);
            data.push_back(quad.y + hh);

            data.push_back(1.0f);
            data.push_back(0.0f);

            data.push_back(quad.r);
            data.push_back(quad.g);
            data.push_back(quad.b);
            data.push_back(quad.a);

            indices.push_back(index + 0);
            indices.push_back(index + 1);
            indices.push_back(index + 3);
            indices.push_back(index + 0);
            indices.push_back(index + 3);
            indices.push_back(index + 2);
            index += 4;
        }

        m_num_indices = indices.size();

        glBindVertexArray(m_vao_id);

        glBindBuffer(GL_ARRAY_BUFFER, m_buff_ids[0]);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, (2 + 2 + 4) * sizeof(float), reinterpret_cast<void*>(0 * sizeof(float)));
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, (2 + 2 + 4) * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, (2 + 2 + 4) * sizeof(float), reinterpret_cast<void*>(4 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buff_ids[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        glDrawElements(GL_TRIANGLES, m_num_indices, GL_UNSIGNED_INT, nullptr);
    }

    m_quads.clear();
}