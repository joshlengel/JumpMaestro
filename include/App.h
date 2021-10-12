#pragma once

#include"State.h"

/*
struct AudioData
{
    SDL_AudioSpec spec;
    uint8_t *buffer;
    uint32_t length;
    uint8_t *position;
    int left;
};*/

struct GLFWwindow;

class App
{
public:
    App(int width, int height, const char *title);
    ~App();

    void Start();

private:
    StateMachine m_machine;
    GLFWwindow *m_window;

    //AudioData m_theme_data;
};