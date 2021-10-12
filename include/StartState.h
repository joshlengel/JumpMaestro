#pragma once

#include"State.h"
#include"Quad.h"
#include"Text.h"

class StartState : public State
{
public:
    void OnEnter(void *data) override;
    void OnUpdate(float dt) override;
    void OnRender(GLFWwindow *window) override;
    //void HandleEvent(SDL_Event &event) override;
    void OnExit() override;

private:
    QuadRenderer m_quad_renderer;
    TextRenderer m_text_renderer;

    Font *m_font;
    Text *m_title_text;
    Text *m_instruction_text;
    Texture *tex;
    float m_elapsed, m_color;
};