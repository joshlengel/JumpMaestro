#include"Player.h"
#include"Constants.h"

#include<SDL2/SDL.h>

#include<cstring>
#include<iostream>

Player::Player(float x, float y, float scale, float r, float g, float b, QuadRenderer &renderer):
    AABB(x, y, 0.0f, 0.0f, scale, scale),
    m_r(r), m_g(g), m_b(b),
    m_on_ground(false),
    m_renderer(renderer)
{
    std::memset(m_buttons, 0, sizeof(m_buttons));

    const PlayerInfo &info = Constants::PLAYERS["default"];
    m_speed = info.speed;
    m_jump_speed = info.jump_speed;
}

void Player::Render(SDL_Window *window, float camera_x, float camera_y)
{
    Quad quad;
    quad.x = x - camera_x;
    quad.y = y - camera_y;
    quad.width = width;
    quad.height = height;
    quad.r = m_r;
    quad.g = m_g;
    quad.b = m_b;
    quad.a = 1.0f;
    m_renderer.Add(quad);
}

void Player::Update(float dt)
{
    float set_vx = 0.0f;
    float set_vy = 0.0f;
    if (m_buttons[0]) set_vx += -m_speed;
    if (m_buttons[1]) set_vx +=  m_speed;

    if (set_vx != 0.0f) vx = set_vx;
    if (set_vy != 0.0f) vy = set_vy;

    float v_sqr = vx * vx + vy * vy;
    float v;
    if (v_sqr < Constants::EPSILON * Constants::EPSILON)
    {
        vx = 0.0f;
        vy = 0.0f;
        v = 0.0f;
    }
    else
    {
        v = std::sqrt(v_sqr);
    }

    float drag_x = m_on_ground? (v > Constants::EPSILON? -m_gravity * m_friction * vx / v : 0) : -0.5f / width * m_air_drag * vx * v;
    float drag_y = -0.5f / width * m_air_drag * vy * v;

    float ax = drag_x;
    float ay = drag_y - m_gravity;

    float dvx = ax * dt;
    float dvy = ay * dt;

    if ((vx > 0 && vx < -dvx) || (vx < 0 && vx > -dvx))
    {
        vx = 0;
    }
    else
    {
        vx += dvx;
    }

    vy += dvy;

    x += vx * dt;
    y += vy * dt;

    m_on_ground = false;
}

void Player::HandleEvent(SDL_Event &event)
{
    if (event.type == SDL_KEYDOWN)
    {
        switch (event.key.keysym.sym)
        {
            case SDLK_LEFT: m_buttons[0] = true; break;
            case SDLK_RIGHT: m_buttons[1] = true; break;
            case SDLK_SPACE: Jump(); break;
        }
    }
    else if (event.type == SDL_KEYUP)
    {
        switch (event.key.keysym.sym)
        {
            case SDLK_LEFT: m_buttons[0] = false; break;
            case SDLK_RIGHT: m_buttons[1] = false; break;
        }
    }
}

void Player::SetGravity(float gravity) { m_gravity = gravity; }
void Player::SetAirDrag(float air_drag) { m_air_drag = air_drag; }

bool Player::HitGround() const { return m_on_ground; }
void Player::SetHitGround(float friction) { m_on_ground = true; m_friction = friction; }
void Player::Jump()
{
    if (m_on_ground)
    {
        m_on_ground = false;
        vy = m_jump_speed;
    }
}