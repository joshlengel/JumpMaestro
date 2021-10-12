#include"Texture.h"
#include"Quad.h"

#include<stb_image.h>

#include<vector>
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

Texture *Texture::FromSurface(int width, int height, uint8_t *pixels)
{
    Texture *texture = new Texture;
    texture->m_width = width;
    texture->m_height = height;
    texture->Bind();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
}

Texture *Texture::FromImage(const std::string &path)
{
    int width, height;
    uint8_t *pixels = stbi_load(path.c_str(), &width, &height, nullptr, STBI_rgb_alpha);
    if (!pixels)
    {
        std::cerr << "Error loading image at '" << path << "'" << std::endl;
        return nullptr;
    }

    Texture *texture = FromSurface(width, height, pixels);
    stbi_image_free(pixels);
    return texture;
}