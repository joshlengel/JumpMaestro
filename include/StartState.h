#pragma once

#include"State.h"
#include"Quad.h"

class Texture;

class StartState : public State
{
public:
    void OnEnter(void *data) override;
    void OnUpdate(float dt) override;
    void OnRender(SDL_Window *window) override;
    void HandleEvent(SDL_Event &event) override;
    void OnExit() override;

private:
    QuadRenderer m_renderer;

    Texture *m_title_text;
    Texture *m_instruction_text;
    float m_elapsed, m_color;
};