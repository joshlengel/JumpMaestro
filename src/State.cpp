#include"State.h"

State::State():
    m_should_exit(false),
    m_prev(nullptr),
    m_next(nullptr),
    m_next_arg(nullptr)
{}

bool State::ShouldExit() const { return m_should_exit; }
State *State::GetNext() const { return m_next; }
void *State::GetNextArg() const { return m_next_arg; }

void State::Exit() { m_should_exit = true; delete m_prev; m_prev = nullptr; }

void StateMachine::CheckExit()
{
    if (m_current && m_current->ShouldExit())
    {
        m_current->OnExit();
        State *next = m_current->GetNext();
        void *next_arg = m_current->GetNextArg();
        m_current = next;

        if (m_current) m_current->OnEnter(next_arg);
    }
}

void StateMachine::Update(float dt)
{
    CheckExit();
    if (m_current) m_current->OnUpdate(dt);
}

void StateMachine::Render(GLFWwindow *window)
{
    CheckExit();
    if (m_current) m_current->OnRender(window);
}

/*void StateMachine::HandleEvent(SDL_Event &event)
{
    CheckExit();
    if (m_current) m_current->HandleEvent(event);
}*/

bool StateMachine::ShouldExit() const { return !m_current || m_current->ShouldExit(); }