#include"Map.h"
#include"Elements.h"
#include"Player.h"
#include"Target.h"
#include"Constants.h"

#include<nlohmann/json.hpp>

#include<fstream>
#include<iostream>

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

    // Player
    auto &player_obj = json["player"];
    float player_scale = player_obj["scale"].get<float>();

    auto &player_pos_arr = player_obj["position"];
    float player_pos_x = player_pos_arr[0].get<float>();
    float player_pos_y = player_pos_arr[1].get<float>();

    // Target
    auto &target_obj = json["target"];
    auto &target_rect_arr = target_obj["rect"];
    float target_rect_l = target_rect_arr[0].get<float>();
    float target_rect_r = target_rect_arr[1].get<float>();
    float target_rect_b = target_rect_arr[2].get<float>();
    float target_rect_t = target_rect_arr[3].get<float>();

    auto &target_color_arr = target_obj["color"];
    float target_color_r = target_color_arr[0].get<float>();
    float target_color_g = target_color_arr[1].get<float>();
    float target_color_b = target_color_arr[2].get<float>();

    // Theme
    auto &theme = json["theme"];
    auto &foreground_arr = theme["foreground"];
    m_fr = foreground_arr[0].get<float>();
    m_fg = foreground_arr[1].get<float>();
    m_fb = foreground_arr[2].get<float>();

    auto &background_arr = theme["background"];
    float b_r = background_arr[0].get<float>();
    float b_g = background_arr[1].get<float>();
    float b_b = background_arr[2].get<float>();

    auto &player_color_arr = theme["player"];
    float p_r = player_color_arr[0].get<float>();
    float p_g = player_color_arr[1].get<float>();
    float p_b = player_color_arr[2].get<float>();

    // Bounds
    auto &bounds_arr = json["bounds"];
    float bounds_l = bounds_arr[0].get<float>();
    float bounds_r = bounds_arr[1].get<float>();
    float bounds_b = bounds_arr[2].get<float>();
    float bounds_t = bounds_arr[3].get<float>();

    m_player = new Player(player_pos_x, player_pos_y, player_scale, p_r, p_g, p_b, m_renderer);
    m_target = new Target(target_rect_l, target_rect_r, target_rect_b, target_rect_t, target_color_r, target_color_g, target_color_b, m_renderer, *m_player);
    m_bounds = new Bounds(bounds_l, bounds_r, bounds_b, bounds_t, b_r, b_g, b_b, m_renderer);

    // Rects
    auto rects_arr = json["rects"];
    for (auto &rect : rects_arr)
    {
        auto &rect_position = rect["position"];
        float rect_l = rect_position[0].get<float>();
        float rect_r = rect_position[1].get<float>();
        float rect_b = rect_position[2].get<float>();
        float rect_t = rect_position[3].get<float>();

        std::string rect_type = rect["type"].get<std::string>();
        Rect::Type type = Rect::GetType(rect_type);

        float r, g, b;

        if (rect_type == "normal")
        {
            r = m_fr;
            g = m_fg;
            b = m_fb;
        }
        else
        {
            auto &color_arr = theme[rect_type];
            r = color_arr[0].get<float>();
            g = color_arr[1].get<float>();
            b = color_arr[2].get<float>();
        }

        m_rects.push_back(Rect::ForType(type, rect_l, rect_r, rect_b, rect_t, r, g, b, m_renderer));
    }

    // Other
    float gravity = json["gravity"].get<float>();
    float air_drag_coefficient = json["air_drag_coefficient"].get<float>();

    m_player->SetGravity(gravity);
    m_player->SetAirDrag(air_drag_coefficient);
}

Map::~Map()
{
    delete m_target;
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

    std::vector<std::pair<Collider*, Rigid*>> colliders;
    colliders.reserve(m_rects.size() + 1);

    for (Rect *rect : m_rects) colliders.push_back({ rect, rect });
    colliders.push_back({ m_bounds, m_bounds });

    auto DoSimulation = [this, &colliders](float dt) -> float
    {
        Collision collision
        {
            .dt=dt,
            .x=m_player->x + m_player->vx * dt, .y=m_player->y + m_player->vy * dt,
            .collider=nullptr
        };

        for (auto &c : colliders)
        {
            c.first->CheckCollision(*m_player, collision);
        }

        m_player->x = collision.x;
        m_player->y = collision.y;

        if (collision.collider)
        {
            for (auto &c : colliders)
            {
                if (c.first == collision.collider)
                    c.second->HandleCollision(*m_player, collision);
            }
        }

        return dt - collision.dt;
    };

    float t = dt;

    while (t > 0.0f)
    {
        t = DoSimulation(t);
    }

    m_target->Update(dt);
}

void Map::Render(SDL_Window *window, float camera_x, float camera_y)
{
    glClearColor(m_fr, m_fg, m_fb, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    

    m_bounds->Render(window, camera_x, camera_y);
    m_target->Render(window, camera_x, camera_y);

    for (Rect *rect : m_rects)
    {
        rect->Render(window, camera_x, camera_y);
    }

    m_player->Render(window, camera_x, camera_y);

    m_renderer.Render(window);
}

void Map::HandleEvent(SDL_Event &event)
{
    m_bounds->HandleEvent(event);

    for (Rect *rect : m_rects)
    {
        rect->HandleEvent(event);
    }

    m_player->HandleEvent(event);
    m_target->HandleEvent(event);
}

bool Map::Finished() const { return m_target->Finished(); }