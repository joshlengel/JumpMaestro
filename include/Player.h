#pragma once

#include"Collider.h"
#include"GameObject.h"
#include"Quad.h"

#include<cstdint>

class Player : public GameObject
{
public:
    Player(float x, float y, float scale, float r, float g, float b, QuadRenderer &renderer);

    virtual void Render(GLFWwindow *window, float camera_x, float camera_y) override;
    virtual void Update(float dt) override;
    //virtual void HandleEvent(SDL_Event &event) override;

    void SetGravity(float gravity);
    void SetAirDrag(float air_drag);

    bool HitGround() const;
    void SetHitGround(float friction);
    void Jump();

public:
    float x, y, scale;
    float vx, vy;

private:
    float m_r, m_g, m_b;
    bool m_on_ground;
    bool m_have_double_jump;

    bool m_buttons[2];

    QuadRenderer &m_renderer;

    float m_acceleration, m_max_speed, m_jump_speed;
    float m_gravity;
    float m_air_drag, m_friction;
};