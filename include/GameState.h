#pragma once

#include"State.h"

class Map;

class GameState : public State
{
public:
    void OnEnter(void *data) override;
    void OnUpdate(float dt) override;
    void OnRender(SDL_Window *window) override;
    void HandleEvent(SDL_Event &event) override;
    void OnExit() override;

private:
    Map *m_map;
};