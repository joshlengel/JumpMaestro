#include"StartState.h"
#include"GameState.h"

#include<SDL2/SDL.h>

void StartState::OnEnter(void *data)
{}

void StartState::OnUpdate(float dt)
{}

void StartState::OnRender(SDL_Window *window)
{}

void StartState::HandleEvent(SDL_Event &event)
{
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) Exit();
}

void StartState::OnExit()
{
    SetNext<GameState>();
}