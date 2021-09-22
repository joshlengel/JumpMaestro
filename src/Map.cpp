#include"Map.h"

#include<SDL2/SDL.h>

Rect::Rect(float x, float y, float width, float height, uint8_t r, uint8_t g, uint8_t b):
    AABB(x, y, 0.0f, 0.0f, width, height),
    r(r), g(g), b(b)
{}

void Rect::Update(float dt)
{}

void Rect::Render(SDL_Window *window, SDL_Renderer *renderer, float camera_x, float camera_y)
{
    int ww, wh;
    SDL_GetWindowSize(window, &ww, &wh);
    float ar = static_cast<float>(ww) / static_cast<float>(wh);

    SDL_SetRenderDrawColor(renderer, r, g, b, 255);

    float vx = (x - camera_x) / ar;
    float vy = (y - camera_y);
    float hw = width * 0.5f;
    float hh = height * 0.5f;

    SDL_Rect rect{ static_cast<int>((vx - hw + 1.0f) * 0.5f * ww), static_cast<int>((1.0f - (vy - hh)) * 0.5f * wh), static_cast<int>(hw * ww), static_cast<int>(hh * wh) };
    SDL_RenderFillRect(renderer, &rect);
}

Bounds::Bounds(float x, float y, float width, float height, uint8_t r, uint8_t g, uint8_t b):
    AABBInverse(x, y, 0.0f, 0.0f, width, height),
    r(r), g(g), b(b)
{}

void Bounds::Update(float dt)
{}

void Bounds::Render(SDL_Window *window, SDL_Renderer *renderer, float camera_x, float camera_y)
{
    int ww, wh;
    SDL_GetWindowSize(window, &ww, &wh);
    float ar = static_cast<float>(ww) / static_cast<float>(wh);

    SDL_SetRenderDrawColor(renderer, r, g, b, 255);

    float hw = width * 0.5f;
    float hh = height * 0.5f;

    float l = x - hw;
    float r = x + hw;
    float b = y - hh;
    float t = y + hh;

    float left[4], right[4], bottom[4], top[4];

    // Left rect
    left[0]   = -1.0f;
    right[0]  = (l - camera_x) / ar;
    bottom[0] = -1.0f;
    top[0]    =  1.0f;

    // Right rect
    left[1]   = (r - camera_x) / ar;
    right[1]  =  1.0f;
    bottom[1] = -1.0f;
    top[1]    =  1.0f;

    // Bottom rect
    left[2]   = -1.0f;
    right[2]  =  1.0f;
    bottom[2] = -1.0f;
    top[2]    = (b - camera_y);

    // Top rect
    left[3]   = -1.0f;
    right[3]  =  1.0f;
    bottom[3] = (t - camera_y);
    top[3]    =  1.0f;

    for (size_t i = 0; i < 4; ++i)
    {
        if (left[i] >= right[i] || bottom[i] >= top[i]) continue;
        SDL_Rect rect{ static_cast<int>((left[i] + 1.0f) * 0.5f * ww), static_cast<int>((1.0f - top[i]) * 0.5f * wh), static_cast<int>((right[i] - left[i]) * 0.5f * ww), static_cast<int>((top[i] - bottom[i]) * 0.5f * wh) };
        SDL_RenderFillRect(renderer, &rect);
    }
}

Map::Map():
    m_bounds(0.0f, 0.0f, 1.5f, 1.5f, 255, 0, 0),
    m_rects()
{
    m_rects.push_back(Rect(0.0f, 0.0f, 0.5f, 0.2f, 200, 50, 20));
}

Map::~Map()
{}

void Map::Update(float dt)
{
    m_bounds.Update(dt);

    for (auto &rect : m_rects)
    {
        rect.Update(dt);
    }
}

void Map::Render(SDL_Window *window, SDL_Renderer *renderer, float camera_x, float camera_y)
{
    m_bounds.Render(window, renderer, camera_x, camera_y);

    for (auto &rect : m_rects)
    {
        rect.Render(window, renderer, camera_x, camera_y);
    }
}