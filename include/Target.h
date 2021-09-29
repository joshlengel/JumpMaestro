#pragma once

#include"Collider.h"
#include"GameObject.h"

class QuadRenderer;
class Player;

class Target : public AABB, public GameObject
{
public:
    Target(float x, float y, float width, float height, float r, float g, float b, QuadRenderer &renderer, const Player &player);

    virtual void Render(SDL_Window *window, float camera_x, float camera_y) override;
    virtual void Update(float dt) override;
    virtual void HandleEvent(SDL_Event &event) override;

    bool Finished() const;

private:
    float m_r, m_g, m_b;

    float m_elapsed;

    QuadRenderer &m_renderer;
    const Player &m_player;
};