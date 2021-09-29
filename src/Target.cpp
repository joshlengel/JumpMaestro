#include"Target.h"
#include"Quad.h"
#include"Player.h"

static const float TARGET_MIN_REST_THRESHOLD = 1.5f;

Target::Target(float x, float y, float width, float height, float r, float g, float b, QuadRenderer &renderer, const Player &player):
    AABB(x, y, 0.0f, 0.0f, width, height),
    m_r(r), m_g(g), m_b(b),
    m_renderer(renderer),
    m_player(player)
{}

void Target::Render(SDL_Window *window, float camera_x, float camera_y)
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

void Target::Update(float dt)
{
    float hw = width * 0.5f;
    float hh = height * 0.5f;

    float p_hw = m_player.width * 0.5f;
    float p_hh = m_player.height * 0.5f;

    if (m_player.x - p_hw >= x - hw && m_player.x + p_hw <= x + hw && m_player.y - p_hh >= y - hh && m_player.y + p_hh <= y + hh)
    {
        m_elapsed += dt;
    }
    else
    {
        m_elapsed = 0.0f;
    }
}

void Target::HandleEvent(SDL_Event &event)
{}

bool Target::Finished() const { return m_elapsed > TARGET_MIN_REST_THRESHOLD; }