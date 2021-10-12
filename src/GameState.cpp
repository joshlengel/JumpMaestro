#include"GameState.h"
#include"Map.h"

void GameState::OnEnter(void *data)
{
    m_map = new Map("assets/map.json");
}

void GameState::OnUpdate(float dt)
{
    m_map->Update(dt);
    if (m_map->Finished()) Exit();
}

void GameState::OnRender(GLFWwindow *window)
{
    m_map->Render(window, 0.0f, 0.0f);
}

/*void GameState::HandleEvent(SDL_Event &event)
{
    m_map->HandleEvent(event);
}*/

void GameState::OnExit()
{
    delete m_map;
}