#pragma once

#include<type_traits>

struct SDL_Window;
union SDL_Event;

class State
{
public:
    State();
    virtual ~State() = default;

    virtual void OnEnter(void *data) = 0;
    virtual void OnUpdate(float dt) = 0;
    virtual void OnRender(SDL_Window *window) = 0;
    virtual void HandleEvent(SDL_Event &event) = 0;
    virtual void OnExit() = 0;

    bool ShouldExit() const;
    State *GetNext() const;
    void *GetNextArg() const;

protected:
    void Exit();

    template <typename T, std::enable_if_t<std::is_base_of<State, T>::value, bool> = true>
    void SetNext(void *arg = nullptr);

private:
    bool m_should_exit;
    State *m_prev, *m_next;
    void *m_next_arg;
};

class StateMachine
{
public:
    template <typename T, std::enable_if_t<std::is_base_of<State, T>::value, bool> = true>
    void Init(void *arg = nullptr);

    void Update(float dt);
    void Render(SDL_Window *window);
    void HandleEvent(SDL_Event &event);

    bool ShouldExit() const;

private:
    State *m_current;

    void CheckExit();
};

template <typename T, std::enable_if_t<std::is_base_of<State, T>::value, bool>>
void State::SetNext(void *arg)
{
    m_next = new T;
    m_next->m_prev = this;
    m_next_arg = arg;
}

template <typename T, std::enable_if_t<std::is_base_of<State, T>::value, bool>>
void StateMachine::Init(void *arg)
{
    m_current = new T;
    m_current->OnEnter(arg);
}