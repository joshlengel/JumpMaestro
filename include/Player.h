#pragma once

#include"Collider.h"
#include"GameObject.h"
#include"Quad.h"

#include<cstdint>

class Player : public AABB, public GameObject
{
public:
    Player(float x, float y, float scale, float r, float g, float b, QuadRenderer &renderer);

    virtual void Render(SDL_Window *window, float camera_x, float camera_y) override;
    virtual void Update(float dt) override;
    virtual void HandleEvent(SDL_Event &event) override;

    bool HitGround() const;
    void SetHitGround(bool hit_ground);
    void Jump();

private:
    float m_r, m_g, m_b;
    bool m_on_ground;

    bool m_buttons[2];

    QuadRenderer &m_renderer;
};