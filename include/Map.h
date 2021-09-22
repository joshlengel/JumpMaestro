#pragma once

#include"GameObject.h"
#include"Collider.h"

#include<vector>
#include<cstdint>

class Rect : public AABB, public GameObject
{
public:
    Rect(float x, float y, float width, float height, uint8_t r, uint8_t g, uint8_t b);

    virtual void Update(float dt) override;
    virtual void Render(SDL_Window *window, SDL_Renderer *renderer, float camera_x, float camera_y) override;

private:
    uint8_t r, g, b;
};

class Bounds : public AABBInverse, public GameObject
{
public:
    Bounds(float x, float y, float width, float height, uint8_t r, uint8_t g, uint8_t b);

    virtual void Update(float dt) override;
    virtual void Render(SDL_Window *window, SDL_Renderer *renderer, float camera_x, float camera_y) override;

private:
    uint8_t r, g, b;
};

class Map : public GameObject
{
public:
    Map();
    ~Map();

    virtual void Update(float dt) override;
    virtual void Render(SDL_Window *window, SDL_Renderer *render, float camera_x, float camera_y) override;

private:
    Bounds m_bounds;
    std::vector<Rect> m_rects;
};