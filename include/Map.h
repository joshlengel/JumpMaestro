#pragma once

#include"GameObject.h"
#include"Quad.h"

#include<vector>
#include<cstdint>
#include<string>

class Rect;
class Bounds;
class Player;
class Target;

class Map : public GameObject
{
public:
    Map(const std::string &file);
    ~Map();

    virtual void Update(float dt) override;
    virtual void Render(SDL_Window *window, float camera_x, float camera_y) override;
    virtual void HandleEvent(SDL_Event &event) override;

    bool Finished() const;

private:
    float m_fr, m_fg, m_fb;

    Bounds *m_bounds;
    std::vector<Rect*> m_rects;

    Player *m_player;
    Target *m_target;

    QuadRenderer m_renderer;
};