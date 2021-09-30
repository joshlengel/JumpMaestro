#pragma once

#include"State.h"

struct SDL_Window;

class App
{
public:
    App(int width, int height, const char *title);
    ~App();

    void Start();

private:
    StateMachine m_machine;
    SDL_Window *m_window;
};