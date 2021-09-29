#pragma once

#include<functional>

struct SDL_Window;
union SDL_Event;

class App
{
public:
    App(int width, int height, const char *title);
    ~App();

    void Show();
    void Start(const std::function<void(float, SDL_Window*)> &loop, const std::function<void(SDL_Event&)> &event);

private:
    SDL_Window *m_window;
};