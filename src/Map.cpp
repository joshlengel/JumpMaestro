#include"Map.h"
#include"Player.h"
#include"Target.h"
#include"Constants.h"

#include<nlohmann/json.hpp>

#include<fstream>
#include<iostream>

Rigid::Rigid(float r, float g, float b, QuadRenderer &renderer):
    r(r), g(g), b(b),
    renderer(renderer),
    friction(Constants::RECTS["default"].friction)
{}

void Rigid::Update(float dt)
{}

float Rigid::GetFriction() const { return friction; }

Rect::Rect(float x, float y, float width, float height, float r, float g, float b, QuadRenderer &renderer):
    AABB(x, y, 0.0f, 0.0f, width, height),
    Rigid(r, g, b, renderer)
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
    renderer.Add(quad);
}

Bounds::Bounds(float x, float y, float width, float height, float r, float g, float b, QuadRenderer &renderer):
    AABBInverse(x, y, 0.0f, 0.0f, width, height),
    Rigid(r, g, b, renderer)
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
    renderer.Add(quad);
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

    // Player
    auto &player_obj = json["player"];
    float player_scale = player_obj["scale"].get<float>();

    auto &player_pos_arr = player_obj["position"];
    float player_pos_x = player_pos_arr[0].get<float>();
    float player_pos_y = player_pos_arr[1].get<float>();

    auto &player_color_arr = player_obj["color"];
    float player_color_r = player_color_arr[0].get<float>();
    float player_color_g = player_color_arr[1].get<float>();
    float player_color_b = player_color_arr[2].get<float>();

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

    // Foreground/Background
    auto &foreground_arr = json["foreground"];
    m_fr = foreground_arr[0].get<float>();
    m_fg = foreground_arr[1].get<float>();
    m_fb = foreground_arr[2].get<float>();

    auto &background_arr = json["background"];
    float b_r = background_arr[0].get<float>();
    float b_g = background_arr[1].get<float>();
    float b_b = background_arr[2].get<float>();

    // Bounds
    auto &bounds_arr = json["bounds"];
    float bounds_l = bounds_arr[0].get<float>();
    float bounds_r = bounds_arr[1].get<float>();
    float bounds_b = bounds_arr[2].get<float>();
    float bounds_t = bounds_arr[3].get<float>();

    m_player = new Player(player_pos_x, player_pos_y, player_scale, player_color_r, player_color_g, player_color_b, m_renderer);
    m_target = new Target((target_rect_l + target_rect_r) * 0.5f, (target_rect_b + target_rect_t) * 0.5f, target_rect_r - target_rect_l, target_rect_t - target_rect_b, target_color_r, target_color_g, target_color_b, m_renderer, *m_player);
    m_bounds = new Bounds((bounds_l + bounds_r) * 0.5f, (bounds_b + bounds_t) * 0.5f, bounds_r - bounds_l, bounds_t - bounds_b, b_r, b_g, b_b, m_renderer);

    // Rects
    auto rects_arr = json["rects"];
    for (auto &rect : rects_arr)
    {
        float rect_l = rect[0].get<float>();
        float rect_r = rect[1].get<float>();
        float rect_b = rect[2].get<float>();
        float rect_t = rect[3].get<float>();

        m_rects.push_back(new Rect((rect_l + rect_r) * 0.5f, (rect_b + rect_t) * 0.5f, rect_r - rect_l, rect_t - rect_b, m_fr, m_fg, m_fb, m_renderer));
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

    auto DoCollisions = [this, &colliders]()
    {
        for (auto &c : colliders)
        {
            Collider *collider = c.first;
            Rigid *rigid = c.second;

            Collision collision;
            collider->CheckCollision(*m_player, collision);

            if (collision.colliding)
            {
                // Resolve collision
                m_player->x += collision.nx * collision.penetration_depth;
                m_player->y += collision.ny * collision.penetration_depth;
                
                if (collision.nx) m_player->vx = 0.0f;
                if (collision.ny) m_player->vy = 0.0f;
                if (collision.ny > 0.0f)
                    m_player->SetHitGround(rigid->GetFriction());
            }
        }
    };

    DoCollisions();
    DoCollisions();

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