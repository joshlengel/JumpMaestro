#include"Texture.h"

#include<SDL2/SDL_image.h>

#include<iostream>

Texture::Texture()
{
    glGenTextures(1, &m_id);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_id);
}

size_t Texture::GetWidth() const { return m_width; }
size_t Texture::GetHeight() const { return m_height; }

void Texture::Bind(size_t index)
{
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

Texture *Texture::FromSurface(SDL_Surface *surface)
{
    Texture *texture = new Texture;
    texture->m_width = surface->w;
    texture->m_height = surface->h;
    texture->Bind();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    SDL_FreeSurface(surface);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
}

Texture *Texture::FromImage(const std::string &path)
{
    SDL_Surface *surface = IMG_Load(path.c_str());
    if (!surface)
    {
        std::cerr << "Error loading image at '" << path << "'" << std::endl;
        return nullptr;
    }

    return FromSurface(surface);
}

Texture *Texture::FromTTF(TTF_Font *font, const std::string &text)
{
    SDL_Surface *surface = TTF_RenderText_Blended(font, text.c_str(), { 255U, 255U, 255U });
    if (!surface)
    {
        std::cerr << "Error rendering text" << std::endl;
        return nullptr;
    }

    return FromSurface(surface);
}