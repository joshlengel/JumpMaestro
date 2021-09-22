#include"App.h"

#include<iostream>

#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>

App::App(int width, int height, const char *title):
    m_window(),
    m_renderer()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Error initializing SDL" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (TTF_Init() < 0)
    {
        std::cerr << "Error initializing TTF" << std::endl;
        exit(EXIT_FAILURE);
    }

    SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_HIDDEN, &m_window, &m_renderer);
    SDL_SetWindowTitle(m_window, title);
}

App::~App()
{
    SDL_Quit();
}

void App::Show()
{
    SDL_ShowWindow(m_window);
}

void App::Start(const std::function<void(SDL_Window*, SDL_Renderer*)> &loop, const std::function<void(SDL_Event&)> &event)
{
    while (true)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT) return;

            event(e);
        }

        loop(m_window, m_renderer);
    }
}