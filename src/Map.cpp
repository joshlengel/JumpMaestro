#include"Map.h"
#include"Player.h"

#include<nlohmann/json.hpp>

#include<fstream>
#include<iostream>

Rect::Rect(float x, float y, float width, float height, float r, float g, float b, QuadRenderer &renderer):
    AABB(x, y, 0.0f, 0.0f, width, height),
    r(r), g(g), b(b),
    m_renderer(renderer)
{}

void Rect::Update(float dt)
{}

void Rect::Render(SDL_Window *window, float camera_x, float camera_y)
{
    Quad quad;
    quad.x = x - camera_x;
    quad.y = y - camera_y;
    quad.width = width;
    quad.height = height;
    quad.r = r;
    quad.g = g;
    quad.b = b;
    quad.a = 1.0f;
    m_renderer.Add(quad);
}

Bounds::Bounds(float x, float y, float width, float height, float r, float g, float b, QuadRenderer &renderer):
    AABBInverse(x, y, 0.0f, 0.0f, width, height),
    r(r), g(g), b(b),
    m_renderer(renderer)
{}

void Bounds::Update(float dt)
{}

void Bounds::Render(SDL_Window *window, float camera_x, float camera_y)
{
    Quad quad;
    quad.x = x - camera_x;
    quad.y = y - camera_y;
    quad.width = width;
    quad.height = height;
    quad.r = r;
    quad.g = g;
    quad.b = b;
    quad.a = 1.0f;
    m_renderer.Add(quad);
}

Map::Map(const std::string &file)
{
    std::ifstream f(file);
    if (!f)
    {
        std::cerr << "Error opening file '" << file << "'" << std::endl;
        exit(EXIT_FAILURE);
    }

    nlohmann::json json;
    f >> json;

    auto &player_obj = json["player"];
    float player_scale = player_obj["scale"].get<float>();

    auto &player_pos_arr = player_obj["position"];
    float player_pos_x = player_pos_arr[0].get<float>();
    float player_pos_y = player_pos_arr[1].get<float>();

    auto &player_color_arr = player_obj["color"];
    float player_color_r = player_color_arr[0].get<float>();
    float player_color_g = player_color_arr[1].get<float>();
    float player_color_b = player_color_arr[2].get<float>();

    auto &foreground_arr = json["foreground"];
    m_fr = foreground_arr[0].get<float>();
    m_fg = foreground_arr[1].get<float>();
    m_fb = foreground_arr[2].get<float>();

    auto &background_arr = json["background"];
    float b_r = background_arr[0].get<float>();
    float b_g = background_arr[1].get<float>();
    float b_b = background_arr[2].get<float>();

    auto &bounds_arr = json["bounds"];
    float bounds_l = bounds_arr[0].get<float>();
    float bounds_r = bounds_arr[1].get<float>();
    float bounds_b = bounds_arr[2].get<float>();
    float bounds_t = bounds_arr[3].get<float>();

    m_player = new Player(player_pos_x, player_pos_y, player_scale, player_color_r, player_color_g, player_color_b, m_renderer);
    m_bounds = new Bounds((bounds_l + bounds_r) * 0.5f, (bounds_b + bounds_t) * 0.5f, bounds_r - bounds_l, bounds_t - bounds_b, b_r, b_g, b_b, m_renderer);

    auto rects_arr = json["rects"];
    for (auto &rect : rects_arr)
    {
        float rect_l = rect[0].get<float>();
        float rect_r = rect[1].get<float>();
        float rect_b = rect[2].get<float>();
        float rect_t = rect[3].get<float>();

        m_rects.push_back(new Rect((rect_l + rect_r) * 0.5f, (rect_b + rect_t) * 0.5f, rect_r - rect_l, rect_t - rect_b, m_fr, m_fg, m_fb, m_renderer));
    }
}

Map::~Map()
{
    delete m_player;
    delete m_bounds;

    for (Rect *rect : m_rects) delete rect;
}

void Map::Update(float dt)
{
    m_bounds->Update(dt);

    for (Rect *rect : m_rects)
    {
        rect->Update(dt);
    }

    m_player->Update(dt);
    m_player->SetHitGround(false);

    std::vector<Collider*> colliders(m_rects.begin(), m_rects.end());
    colliders.push_back(m_bounds);

    for (Collider *c : colliders)
    {
        Collision collision;
        c->CheckCollision(*m_player, collision);

        if (collision.colliding)
        {
            // Resolve collision
            m_player->x += collision.nx * collision.penetration_depth;
            m_player->y += collision.ny * collision.penetration_depth;
            
            if (collision.nx) m_player->vx = 0.0f;
            if (collision.ny) m_player->vy = 0.0f;
            if (collision.ny > 0.0f) m_player->SetHitGround(true);
        }
    }
}

void Map::Render(SDL_Window *window, float camera_x, float camera_y)
{
    glClearColor(m_fr, m_fg, m_fb, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    m_bounds->Render(window, camera_x, camera_y);

    for (Rect *rect : m_rects)
    {
        rect->Render(window, camera_x, camera_y);
    }

    m_player->Render(window, camera_x, camera_y);

    m_renderer.FlushAndRender(window);
}

void Map::HandleEvent(SDL_Event &event)
{
    m_bounds->HandleEvent(event);

    for (Rect *rect : m_rects)
    {
        rect->HandleEvent(event);
    }

    m_player->HandleEvent(event);
}