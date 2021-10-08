#include"StartState.h"
#include"GameState.h"
#include"Texture.h"

#include<cmath>
#include<iostream>

#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_audio.h>
#include<SDL2/SDL_mixer.h>
#include<GL/glew.h>

void StartState::OnEnter(void *data)
{
    TTF_Font *large_font = TTF_OpenFont("assets/OpenSans-Light.ttf", 90);
    TTF_Font *small_font = TTF_OpenFont("assets/OpenSans-Light.ttf", 45);

    m_title_text = Texture::FromTTF(large_font, "Jump Maestro");
    m_instruction_text = Texture::FromTTF(small_font, "Press enter to start");

    TTF_CloseFont(large_font);
    TTF_CloseFont(small_font);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_elapsed = 0.0f;
}

void StartState::OnUpdate(float dt)
{
    m_elapsed += dt;
    m_color = std::cos(m_elapsed * 0.5f) * 0.8f * 0.5f + 0.5f;
}

void StartState::OnRender(SDL_Window *window)
{
    glClearColor(m_color, m_color, m_color, 1.0f);

    float ar;
    Quad quad;

    ar = static_cast<float>(m_title_text->GetWidth()) / static_cast<float>(m_title_text->GetHeight());
    
    quad.x = 0.0f;
    quad.y = 0.3f;
    quad.width = 2.0f;
    quad.height = 2.0f / ar;
    quad.r = 1.0f - m_color;
    quad.g = 1.0f - m_color;
    quad.b = 1.0f - m_color;
    quad.a = 1.0f;
    quad.texture = m_title_text;

    m_renderer.Add(quad);

    ar = static_cast<float>(m_instruction_text->GetWidth()) / static_cast<float>(m_instruction_text->GetHeight());

    quad.x = 0.0f;
    quad.y = -0.1f;
    quad.width = 1.0f;
    quad.height = 1.0f / ar;
    quad.r = 0.5f;
    quad.g = 0.2f;
    quad.b = 0.3f;
    quad.a = 1.0f;
    quad.texture = m_instruction_text;

    m_renderer.Add(quad);

    glClear(GL_COLOR_BUFFER_BIT);
    m_renderer.Render(window);
}

void StartState::HandleEvent(SDL_Event &event)
{
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) Exit();
}

void StartState::OnExit()
{
    delete m_title_text;

    SetNext<GameState>();
}