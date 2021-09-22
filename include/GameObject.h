#pragma once

struct SDL_Window;
struct SDL_Renderer;

class GameObject
{
public:
    virtual ~GameObject() = default;

    virtual void Update(float dt) = 0;
    virtual void Render(SDL_Window *window, SDL_Renderer *renderer, float camera_x, float camera_y) = 0;
};