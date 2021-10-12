#include"Text.h"
#include"Texture.h"

#include<fstream>
#include<iostream>
#include<cstring>

#include<GLFW/glfw3.h>

Font::Font(const std::string &fnt, const std::string &image, int desired_padding):
    m_glyphs(),
    m_texture(Texture::FromImage(image)),
    m_desired_padding(desired_padding)
{
    // parse fnt file

    std::ifstream fnt_file(fnt);
    if (!fnt_file)
    {
        std::cerr << "Error reading '" << fnt << "'" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string line;
    while (std::getline(fnt_file, line))
    {
        char *identifier_end = strchr(&line[0], ' ');
        *identifier_end = '\0';
        char *identifier = &line[0];

        if (strcmp(identifier, "") == 0) continue;

        Glyph *current;
        
        char *itr = identifier_end + 1;
        char *tok = strtok(itr, " \r");
        while (tok)
        {
            char *key_end = strchr(tok, '=');
            *key_end = '\0';
            char *key = tok;
            char *value = key_end + 1;

            // Use identifier-key-value
            if (strcmp(identifier, "info") == 0 && strcmp(key, "padding") == 0)
            {
                char *p0 = value;
                char *p1 = strchr(p0, ',') + 1;
                char *p2 = strchr(p1, ',') + 1;
                char *p3 = strchr(p2, ',') + 1;

                m_padding[0] = strtol(p0, nullptr, 10);
                m_padding[1] = strtol(p1, nullptr, 10);
                m_padding[2] = strtol(p2, nullptr, 10);
                m_padding[3] = strtol(p3, nullptr, 10);
            }
            else if (strcmp(identifier, "common") == 0 && strcmp(key, "lineHeight") == 0)
            {
                m_line_height = strtol(value, nullptr, 10);
            }
            else if (strcmp(identifier, "char") == 0)
            {
                if (strcmp(key, "id") == 0)
                {
                    char c = static_cast<char>(strtol(value, nullptr, 10));
                    current = &(m_glyphs[c] = {});
                }
                else if (strcmp(key, "x") == 0)
                {
                    current->x = static_cast<int>(strtol(value, nullptr, 10));
                }
                else if (strcmp(key, "y") == 0)
                {
                    current->y = static_cast<int>(strtol(value, nullptr, 10));
                }
                else if (strcmp(key, "width") == 0)
                {
                    current->w = static_cast<int>(strtol(value, nullptr, 10));
                }
                else if (strcmp(key, "height") == 0)
                {
                    current->h = static_cast<int>(strtol(value, nullptr, 10));
                }
                else if (strcmp(key, "xoffset") == 0)
                {
                    current->off_x = static_cast<int>(strtol(value, nullptr, 10));
                }
                else if (strcmp(key, "yoffset") == 0)
                {
                    current->off_y = static_cast<int>(strtol(value, nullptr, 10));
                }
                else if (strcmp(key, "xadvance") == 0)
                {
                    current->adv_x = static_cast<int>(strtol(value, nullptr, 10));
                }
            }

            tok = strtok(nullptr, " ");
        }
    }
}

Font::~Font()
{
    delete m_texture;
}

const Glyph &Font::LoadGlyph(char c) const { return m_glyphs.at(c); }
Texture *Font::GetTexture() const { return m_texture; }

void Font::GetPadding(int dst[4]) const { std::memcpy(dst, m_padding, sizeof(m_padding)); }
int Font::GetLineHeight() const { return m_line_height; }
int Font::GetDesiredPadding() const { return m_desired_padding; }

Text::Text(const Font &font, const std::string &str, float scale):
    m_texture(font.GetTexture())
{
    glGenVertexArrays(1, &m_vao_id);
    glGenBuffers(2, m_buff_ids);

    std::vector<float> vertices;
    vertices.reserve(str.length() * 4 * (2 + 2));
    std::vector<unsigned int> indices;
    indices.reserve(str.length() * 6);
    size_t index = 0;

    float cx = 0, cy = 0;
    int padding[4];
    font.GetPadding(padding);

    float pl = (padding[1] - font.GetDesiredPadding()) / static_cast<float>(m_texture->GetWidth());
    float pt = (padding[0] - font.GetDesiredPadding()) / static_cast<float>(m_texture->GetHeight());
    float pw = (padding[1] + padding[3] - 2 * font.GetDesiredPadding()) / static_cast<float>(m_texture->GetWidth());
    float ph = (padding[0] + padding[2] - 2 * font.GetDesiredPadding()) / static_cast<float>(m_texture->GetHeight());

    for (char c : str)
    {
        const Glyph &g = font.LoadGlyph(c);
        float tx = static_cast<float>(g.x) / static_cast<float>(m_texture->GetWidth()) + pl;
        float ty = static_cast<float>(g.y) / static_cast<float>(m_texture->GetHeight()) + pt;
        float tw = static_cast<float>(g.w) / static_cast<float>(m_texture->GetWidth()) - pw;
        float th = static_cast<float>(g.h) / static_cast<float>(m_texture->GetHeight()) - ph;

        float w = tw * scale;
        float h = th * scale;
        float off_x = (static_cast<float>(g.off_x) / static_cast<float>(m_texture->GetWidth()) + pl) * scale;
        float off_y = (static_cast<float>(g.off_y) / static_cast<float>(m_texture->GetHeight()) + pt) * scale;
        float adv_x = static_cast<float>(g.adv_x - padding[2] - padding[3]) / static_cast<float>(m_texture->GetWidth()) * scale;

        float x = cx + off_x;
        float y = cy - off_y;

        vertices.push_back(x);
        vertices.push_back(y - h);
        vertices.push_back(tx);
        vertices.push_back(ty + th);

        vertices.push_back(x + w);
        vertices.push_back(y - h);
        vertices.push_back(tx + tw);
        vertices.push_back(ty + th);

        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(tx);
        vertices.push_back(ty);

        vertices.push_back(x + w);
        vertices.push_back(y);
        vertices.push_back(tx + tw);
        vertices.push_back(ty);

        indices.push_back(index + 0);
        indices.push_back(index + 1);
        indices.push_back(index + 3);
        indices.push_back(index + 0);
        indices.push_back(index + 3);
        indices.push_back(index + 2);
        index += 4;

        cx += adv_x;
    }

    m_count = indices.size();

    glBindVertexArray(m_vao_id);

    glBindBuffer(GL_ARRAY_BUFFER, m_buff_ids[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, (2 + 2) * sizeof(float), reinterpret_cast<void*>(0 * sizeof(float)));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, (2 + 2) * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buff_ids[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
}

Text::~Text()
{
    glDeleteVertexArrays(1, &m_vao_id);
    glDeleteBuffers(2, m_buff_ids);
}

float Text::GetR() const { return m_r; }
float Text::GetG() const { return m_g; }
float Text::GetB() const { return m_b; }

void Text::SetColor(float r, float g, float b) { m_r = r; m_g = g; m_b = b; }

void Text::Render()
{
    m_texture->Bind();

    glBindVertexArray(m_vao_id);
    glDrawElements(GL_TRIANGLES, m_count, GL_UNSIGNED_INT, nullptr);
}

static const char *const VERTEX_SOURCE = R"(
    #version 330

    layout(location = 0) in vec2 position;
    layout(location = 1) in vec2 uv;

    out vec2 _uv;

    uniform float aspect_ratio;

    void main() {
        gl_Position = vec4(position, 0.0, 1.0);
        gl_Position.x /= aspect_ratio;
        _uv = uv;
    }
)";

static const char *const FRAGMENT_SOURCE = R"(
    #version 330

    in vec2 _uv;

    out vec4 fragment;

    uniform sampler2D tex;
    uniform vec3 color;

    const float SIZE = 0.5;
    const float EDGE = 0.4;

    void main() {
        float dist = 1 - texture2D(tex, _uv).a;
        float value = smoothstep(SIZE, SIZE + EDGE, dist);
        fragment = texture2D(tex, _uv).aaaa;
    }
)";

TextRenderer::TextRenderer():
    m_texts()
{
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

    m_color_location = glGetUniformLocation(m_program_id, "color");
    m_aspect_ratio_location = glGetUniformLocation(m_program_id, "aspect_ratio");

    glDetachShader(m_program_id, m_shader_ids[0]);
    glDetachShader(m_program_id, m_shader_ids[1]);
}

TextRenderer::~TextRenderer()
{
    glDeleteProgram(m_program_id);
    glDeleteShader(m_shader_ids[0]);
    glDeleteShader(m_shader_ids[1]);
}

void TextRenderer::Add(Text *text) { m_texts.push_back(text); }

void TextRenderer::Render(GLFWwindow *window)
{
    int ww, wh;
    glfwGetWindowSize(window, &ww, &wh);
    float ar = static_cast<float>(ww) / static_cast<float>(wh);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glUseProgram(m_program_id);
    glUniform1f(m_aspect_ratio_location, ar);

    for (Text *text : m_texts)
    {
        glUniform3f(m_color_location, text->GetR(), text->GetG(), text->GetB());
        text->Render();
    }
}