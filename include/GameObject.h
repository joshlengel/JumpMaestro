#pragma once

struct GLFWwindow;

class GameObject
{
public:
    virtual ~GameObject() = default;

    virtual void Update(float dt) = 0;
    virtual void Render(GLFWwindow *window, float camera_x, float camera_y) = 0;
    //virtual void HandleEvent(SDL_Event &event) {};
};