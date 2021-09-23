#pragma once

#include"Collider.h"
#include"GameObject.h"

#include<cstdint>

class Player : public AABB, public GameObject
{
public:
    Player(float x, float y, float scale, uint8_t r, uint8_t g, uint8_t b);

    virtual void Render(SDL_Window *window, SDL_Renderer *renderer, float camera_x, float camera_y) override;
    virtual void Update(float dt) override;
    virtual void HandleEvent(SDL_Event &event) override;

private:
    uint8_t m_r, m_g, m_b;
    bool m_on_ground;

    bool m_buttons[4];
};