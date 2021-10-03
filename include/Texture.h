#pragma once

#include<SDL2/SDL_ttf.h>
#include<GL/glew.h>

#include<string>

class Texture
{
public:
    Texture();
    ~Texture();

    size_t GetWidth() const;
    size_t GetHeight() const;
    void Bind(size_t index = 0);

    static Texture *FromImage(const std::string &path);
    static Texture *FromTTF(TTF_Font *font, const std::string &text);

private:
    size_t m_width, m_height;
    GLuint m_id;

    static Texture *FromSurface(SDL_Surface *surface);
};