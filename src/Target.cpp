#include"Target.h"
#include"Quad.h"
#include"Player.h"

static const float TARGET_MIN_REST_THRESHOLD = 1.5f;

Target::Target(float left, float right, float bottom, float top, float r, float g, float b, QuadRenderer &renderer, const Player &player):
    AABB(left, right, bottom, top),
    m_r(r), m_g(g), m_b(b),
    m_renderer(renderer),
    m_player(player)
{}

void Target::Render(SDL_Window *window, float camera_x, float camera_y)
{
    Quad quad;
    quad.x = (AABB::l + AABB::r) * 0.5f - camera_x;
    quad.y = (AABB::b + AABB::t) * 0.5f - camera_y;
    quad.width = AABB::r - AABB::l;
    quad.height = AABB::t - AABB::b;
    quad.r = m_r;
    quad.g = m_g;
    quad.b = m_b;
    quad.a = 1.0f;
    m_renderer.Add(quad);
}

void Target::Update(float dt)
{
    float p_hs = m_player.scale * 0.5f;

    if (m_player.x - p_hs >= AABB::l && m_player.x + p_hs <= AABB::r && m_player.y - p_hs >= AABB::b && m_player.y + p_hs <= AABB::t)
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