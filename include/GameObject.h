#pragma once

struct SDL_Window;
union SDL_Event;

class GameObject
{
public:
    virtual ~GameObject() = default;

    virtual void Update(float dt) = 0;
    virtual void Render(SDL_Window *window, float camera_x, float camera_y) = 0;
    virtual void HandleEvent(SDL_Event &event) {};
};