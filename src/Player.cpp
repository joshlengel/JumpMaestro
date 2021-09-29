#include"Player.h"

#include<SDL2/SDL.h>

#include<cstring>

static const float AIR_DRAG_COEFFICIENT = 0.1f;
static const float GROUND_DRAG_COEFFICIENT = 0.5f;
static const float EPSILON = 0.01f;
static const float SPEED = 1.0f;
static const float JUMP_SPEED = 2.5f;
static const float GRAVITY = 9.81f;

Player::Player(float x, float y, float scale, float r, float g, float b, QuadRenderer &renderer):
    AABB(x, y, 0.0f, 0.0f, scale, scale),
    m_r(r), m_g(g), m_b(b),
    m_on_ground(false),
    m_renderer(renderer)
{
    std::memset(m_buttons, 0, sizeof(m_buttons));
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
    if (m_buttons[0]) set_vx += -SPEED;
    if (m_buttons[1]) set_vx +=  SPEED;

    if (set_vx != 0.0f) vx = set_vx;
    if (set_vy != 0.0f) vy = set_vy;

    float v_sqr = vx * vx + vy * vy;
    float v;
    if (v_sqr < EPSILON * EPSILON)
    {
        vx = 0.0f;
        vy = 0.0f;
        v = 0.0f;
    }
    else
    {
        v = std::sqrt(v_sqr);
    }

    float drag_x = m_on_ground? (v > EPSILON? -GRAVITY * GROUND_DRAG_COEFFICIENT * vx / v : 0) : -0.5f / width * AIR_DRAG_COEFFICIENT * vx * v;
    float drag_y = -0.5f / width * AIR_DRAG_COEFFICIENT * vy * v;

    float gravity = -GRAVITY;

    float ax = drag_x;
    float ay = drag_y + gravity;

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

bool Player::HitGround() const { return m_on_ground; }
void Player::SetHitGround(bool hit_ground) { m_on_ground = hit_ground; }
void Player::Jump()
{
    if (m_on_ground)
    {
        m_on_ground = false;
        vy = JUMP_SPEED;
    }
}