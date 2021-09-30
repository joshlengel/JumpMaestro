#pragma once

#include"GameObject.h"
#include"Collider.h"
#include"Quad.h"

#include<vector>
#include<cstdint>
#include<string>

class Player;
class Target;

class Rigid : public GameObject
{
public:
    Rigid(float r, float g, float b, QuadRenderer &renderer);

    virtual void Update(float dt) override;
    virtual void Render(SDL_Window *window, float camera_x, float camera_y) = 0;

    float GetFriction() const;

protected:
    float r, g, b;
    QuadRenderer &renderer;

    float friction;
};

class Rect : public AABB, public Rigid
{
public:
    Rect(float x, float y, float width, float height, float r, float g, float b, QuadRenderer &renderer);

    virtual void Render(SDL_Window *window, float camera_x, float camera_y) override;
};

class Bounds : public AABBInverse, public Rigid
{
public:
    Bounds(float x, float y, float width, float height, float r, float g, float b, QuadRenderer &renderer);

    virtual void Render(SDL_Window *window, float camera_x, float camera_y) override;
};

class Map : public GameObject
{
public:
    Map(const std::string &file);
    ~Map();

    virtual void Update(float dt) override;
    virtual void Render(SDL_Window *window, float camera_x, float camera_y) override;
    virtual void HandleEvent(SDL_Event &event) override;

    bool Finished() const;

private:
    float m_fr, m_fg, m_fb;

    Bounds *m_bounds;
    std::vector<Rect*> m_rects;

    Player *m_player;
    Target *m_target;

    QuadRenderer m_renderer;
};