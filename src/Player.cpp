#include"Player.h"
#include"Constants.h"

#include<GLFW/glfw3.h>

#include<cstring>
#include<iostream>
#include<cmath>

Player::Player(float x, float y, float scale, float r, float g, float b, QuadRenderer &renderer):
    x(x), y(y), scale(scale),
    vx(0.0f), vy(0.0f),
    m_r(r), m_g(g), m_b(b),
    m_on_ground(false),
    m_have_double_jump(true),
    m_renderer(renderer)
{
    std::memset(m_buttons, 0, sizeof(m_buttons));

    const PlayerInfo &info = Constants::PLAYERS["default"];
    m_acceleration = info.acceleration;
    m_max_speed = info.max_speed;
    m_jump_speed = info.jump_speed;
}

void Player::Render(GLFWwindow *window, float camera_x, float camera_y)
{
    Quad quad;
    quad.x = x - camera_x;
    quad.y = y - camera_y;
    quad.width = scale;
    quad.height = scale;
    quad.r = m_r;
    quad.g = m_g;
    quad.b = m_b;
    quad.a = 1.0f;
    m_renderer.Add(quad);
}

void Player::Update(float dt)
{
    float ax = 0.0f;
    float ay = 0.0f;
    if (m_buttons[0]) ax += -m_acceleration;
    if (m_buttons[1]) ax +=  m_acceleration;

    float v_sqr = vx * vx + vy * vy;
    float v = v = std::sqrt(v_sqr);

    float drag_x = m_on_ground? (v > Constants::EPSILON? -m_gravity * m_friction * vx / v : 0) : -0.5f / scale * m_air_drag * vx * v;
    float drag_y = -0.5f / scale * m_air_drag * vy * v;

    ax += drag_x;
    ay += drag_y - m_gravity;

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
    
    vx = vx > m_max_speed? m_max_speed : vx < -m_max_speed? -m_max_speed : vx;

    m_on_ground = false;
}

/*void Player::HandleEvent(SDL_Event &event)
{
    if (event.type == SDL_KEYDOWN)
    {
        switch (event.key.keysym.sym)
        {
            case SDLK_LEFT: m_buttons[0] = true; break;
            case SDLK_RIGHT: m_buttons[1] = true; break;
            case SDLK_SPACE:
                if (!m_on_ground)
                {
                    if (!m_have_double_jump) break;
                    else m_have_double_jump = false;
                }

                Jump();
                break;
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
}*/

void Player::SetGravity(float gravity) { m_gravity = gravity; }
void Player::SetAirDrag(float air_drag) { m_air_drag = air_drag; }

bool Player::HitGround() const { return m_on_ground; }
void Player::SetHitGround(float friction) { m_on_ground = true; m_have_double_jump = true; m_friction = friction; }
void Player::Jump()
{
    m_on_ground = false;
    vy = m_jump_speed;
}