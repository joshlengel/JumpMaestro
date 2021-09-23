#include"Player.h"

#include<SDL2/SDL.h>

#include<cstring>

static const float AIR_DRAG_COEFFICIENT = 1000.0f;
static const float GROUND_DRAG_COEFFICIENT = 1000.0f;
static const float EPSILON = 0.01f;
static const float SPEED = 1.0f;

Player::Player(float x, float y, float scale, uint8_t r, uint8_t g, uint8_t b):
    AABB(x, y, 0.0f, 0.0f, scale, scale),
    m_r(r), m_g(g), m_b(b),
    m_on_ground(false)
{
    std::memset(m_buttons, 0, sizeof(m_buttons));
}

void Player::Render(SDL_Window *window, SDL_Renderer *renderer, float camera_x, float camera_y)
{
    int ww, wh;
    SDL_GetWindowSize(window, &ww, &wh);
    float ar = static_cast<float>(ww) / static_cast<float>(wh);

    SDL_SetRenderDrawColor(renderer, m_r, m_g, m_b, 255);

    float vx = (x - camera_x) / ar;
    float vy = (y - camera_y);
    float hw = width * 0.5f / ar;
    float hh = height * 0.5f;

    SDL_Rect rect{ static_cast<int>((vx - hw + 1.0f) * 0.5f * ww), static_cast<int>((1.0f - (vy + hh)) * 0.5f * wh), static_cast<int>(hw * ww), static_cast<int>(hh * wh) };
    SDL_RenderFillRect(renderer, &rect);
}

void Player::Update(float dt)
{
    float set_vx = 0.0f;
    float set_vy = 0.0f;
    if (m_buttons[0]) set_vx += -SPEED;
    if (m_buttons[1]) set_vx +=  SPEED;
    if (m_buttons[2]) set_vy += -SPEED;
    if (m_buttons[3]) set_vy +=  SPEED;

    if (set_vx != 0.0f) vx = set_vx;
    if (set_vy != 0.0f) vy = set_vy;

    float v_sqr = vx * vx + vy * vy;
    float v = std::sqrt(v_sqr);

    float drag = -9.81f * GROUND_DRAG_COEFFICIENT - 0.5f / width * AIR_DRAG_COEFFICIENT * v_sqr;

    float drag_x = v == 0? 0 : -0.5f / width * AIR_DRAG_COEFFICIENT * vx * v - m_on_ground * 9.81f * GROUND_DRAG_COEFFICIENT * vx / v;
    float drag_y = v == 0? 0 : -0.5f / width * AIR_DRAG_COEFFICIENT * vy * v;

    float dvx = drag_x * dt;
    float dvy = drag_y * dt;

    vx += dvx * dt;
    vy += dvy * dt;

    x += vx * dt;
    y += vy * dt;

    if (v_sqr < EPSILON * EPSILON)
    {
        vx = 0.0f;
        vy = 0.0f;
    }
}

void Player::HandleEvent(SDL_Event &event)
{
    if (event.type == SDL_KEYDOWN)
    {
        switch (event.key.keysym.sym)
        {
            case SDLK_LEFT: m_buttons[0] = true; break;
            case SDLK_RIGHT: m_buttons[1] = true; break;
            case SDLK_DOWN: m_buttons[2] = true; break;
            case SDLK_UP: m_buttons[3] = true; break;
        }
    }
    else if (event.type == SDL_KEYUP)
    {
        switch (event.key.keysym.sym)
        {
            case SDLK_LEFT: m_buttons[0] = false; break;
            case SDLK_RIGHT: m_buttons[1] = false; break;
            case SDLK_DOWN: m_buttons[2] = false; break;
            case SDLK_UP: m_buttons[3] = false; break;
        }
    }
}