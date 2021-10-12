#pragma once

#include<glad/glad.h>

#include<string>

class Texture
{
public:
    Texture();
    ~Texture();

    size_t GetWidth() const;
    size_t GetHeight() const;
    void Bind(size_t index = 0);

    static Texture *FromSurface(int width, int height, uint8_t *pixels);
    static Texture *FromImage(const std::string &path);

private:
    size_t m_width, m_height;
    GLuint m_id;
};