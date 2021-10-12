#include"StartState.h"
#include"GameState.h"
#include"Texture.h"

#include<cmath>
#include<iostream>

#include<GLFW/glfw3.h>
#include<glad/glad.h>

void StartState::OnEnter(void *data)
{
    m_font = new Font("assets/candara.fnt", "assets/candara.png", 5);

    m_title_text = new Text(*m_font, "Jump Maestro", 4.0f);
    m_instruction_text = new Text(*m_font, "", 0.25f);
    tex = Texture::FromImage("assets/candara.png");

    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_elapsed = 0.0f;
}

void StartState::OnUpdate(float dt)
{
    m_elapsed += dt;
    m_color = std::cos(m_elapsed * 0.5f) * 0.8f * 0.5f + 0.5f;
}

void StartState::OnRender(GLFWwindow *window)
{
    glClearColor(m_color, m_color, m_color, 1.0f);

    m_title_text->SetColor(/*1 - m_color, 1 - m_color, 1 - m_color*/1,0,0);
    m_text_renderer.Add(m_title_text);

    m_instruction_text->SetColor(0.5f, 0.2f, 0.3f);
    m_text_renderer.Add(m_instruction_text);

    glClear(GL_COLOR_BUFFER_BIT);

    Quad quad;
    quad.x = 0.0f; quad.y = 0.0f;
    quad.width = 2.0f;
    quad.height = 2.0f;
    quad.r = quad.g = quad.b = 1.0f;
    quad.a = 1.0f;
    quad.texture = tex;
    m_quad_renderer.Add(quad);

    m_quad_renderer.Render(window);
    m_text_renderer.Render(window);
}

/*void StartState::HandleEvent(SDL_Event &event)
{
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) Exit();
}*/

void StartState::OnExit()
{
    delete m_title_text;
    delete m_instruction_text;
    delete m_font;

    SetNext<GameState>();
}