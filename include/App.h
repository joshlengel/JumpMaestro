#pragma once

#include"State.h"

#include<SDL2/SDL_mixer.h>

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

    Mix_Chunk *m_theme_music;
};