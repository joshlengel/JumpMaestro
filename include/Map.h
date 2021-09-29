#pragma once

#include"GameObject.h"
#include"Collider.h"
#include"Quad.h"

#include<vector>
#include<cstdint>
#include<string>

class Player;

class Rect : public AABB, public GameObject
{
public:
    Rect(float x, float y, float width, float height, float r, float g, float b, QuadRenderer &renderer);

    virtual void Update(float dt) override;
    virtual void Render(SDL_Window *window, float camera_x, float camera_y) override;

private:
    float r, g, b;
    QuadRenderer &m_renderer;
};

class Bounds : public AABBInverse, public GameObject
{
public:
    Bounds(float x, float y, float width, float height, float r, float g, float b, QuadRenderer &renderer);

    virtual void Update(float dt) override;
    virtual void Render(SDL_Window *window, float camera_x, float camera_y) override;

private:
    float r, g, b;
    QuadRenderer &m_renderer;
};

class Map : public GameObject
{
public:
    Map(const std::string &file);
    ~Map();

    virtual void Update(float dt) override;
    virtual void Render(SDL_Window *window, float camera_x, float camera_y) override;
    virtual void HandleEvent(SDL_Event &event) override;

private:
    float m_fr, m_fg, m_fb;

    Bounds *m_bounds;
    std::vector<Rect*> m_rects;

    Player *m_player;

    QuadRenderer m_renderer;
};