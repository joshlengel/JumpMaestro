#include"App.h"
#include"Constants.h"

#include<iostream>
#include<chrono>

#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>

#include<GL/glew.h>

App::App(int width, int height, const char *title):
    m_window()
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

    m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
    SDL_GL_CreateContext(m_window);

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Error initializing GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }

    Constants::Init("assets/constants.json");
}

App::~App()
{
    SDL_Quit();
}

void App::Show()
{
    SDL_ShowWindow(m_window);
}

void App::Start(const std::function<void(float, SDL_Window*)> &loop, const std::function<void(SDL_Event&)> &event)
{
    std::chrono::high_resolution_clock::time_point t1, t2;
    t1 = t2 = std::chrono::high_resolution_clock::now();

    while (true)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT) return;

            event(e);
        }

        t2 = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration_cast<std::chrono::duration<float>>(t2 - t1).count();
        t1 = t2;

        loop(dt, m_window);

        SDL_GL_SwapWindow(m_window);
    }
}