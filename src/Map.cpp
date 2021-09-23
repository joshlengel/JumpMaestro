#include"Map.h"
#include"Player.h"

#include<nlohmann/json.hpp>
#include<SDL2/SDL.h>

#include<fstream>
#include<iostream>

Rect::Rect(float x, float y, float width, float height, uint8_t r, uint8_t g, uint8_t b):
    AABB(x, y, 0.0f, 0.0f, width, height),
    r(r), g(g), b(b)
{}

void Rect::Update(float dt)
{}

void Rect::Render(SDL_Window *window, SDL_Renderer *renderer, float camera_x, float camera_y)
{
    int ww, wh;
    SDL_GetWindowSize(window, &ww, &wh);
    float ar = static_cast<float>(ww) / static_cast<float>(wh);

    SDL_SetRenderDrawColor(renderer, r, g, b, 255);

    float vx = (x - camera_x) / ar;
    float vy = (y - camera_y);
    float hw = width * 0.5f / ar;
    float hh = height * 0.5f;

    SDL_Rect rect{ static_cast<int>((vx - hw + 1.0f) * 0.5f * ww), static_cast<int>((1.0f - (vy + hh)) * 0.5f * wh), static_cast<int>(hw * ww), static_cast<int>(hh * wh) };
    SDL_RenderFillRect(renderer, &rect);
}

Bounds::Bounds(float x, float y, float width, float height, uint8_t r, uint8_t g, uint8_t b):
    AABBInverse(x, y, 0.0f, 0.0f, width, height),
    r(r), g(g), b(b)
{}

void Bounds::Update(float dt)
{}

void Bounds::Render(SDL_Window *window, SDL_Renderer *renderer, float camera_x, float camera_y)
{
    int ww, wh;
    SDL_GetWindowSize(window, &ww, &wh);
    float ar = static_cast<float>(ww) / static_cast<float>(wh);

    SDL_SetRenderDrawColor(renderer, r, g, b, 255);

    float vx = (x - camera_x) / ar;
    float vy = (y - camera_y);
    float hw = width * 0.5f / ar;
    float hh = height * 0.5f;

    SDL_Rect rect{ static_cast<int>((vx - hw + 1.0f) * 0.5f * ww), static_cast<int>((1.0f - (vy + hh)) * 0.5f * wh), static_cast<int>(hw * ww), static_cast<int>(hh * wh) };
    SDL_RenderFillRect(renderer, &rect);
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
    uint8_t player_color_r = player_color_arr[0].get<uint8_t>();
    uint8_t player_color_g = player_color_arr[1].get<uint8_t>();
    uint8_t player_color_b = player_color_arr[2].get<uint8_t>();

    auto &foreground_arr = json["foreground"];
    m_fr = foreground_arr[0].get<uint8_t>();
    m_fg = foreground_arr[1].get<uint8_t>();
    m_fb = foreground_arr[2].get<uint8_t>();

    auto &background_arr = json["background"];
    uint8_t b_r = background_arr[0].get<uint8_t>();
    uint8_t b_g = background_arr[1].get<uint8_t>();
    uint8_t b_b = background_arr[2].get<uint8_t>();

    auto &bounds_arr = json["bounds"];
    float bounds_l = bounds_arr[0].get<float>();
    float bounds_r = bounds_arr[1].get<float>();
    float bounds_b = bounds_arr[2].get<float>();
    float bounds_t = bounds_arr[3].get<float>();

    m_player = new Player(player_pos_x, player_pos_y, player_scale, player_color_r, player_color_g, player_color_b);
    m_bounds = new Bounds((bounds_l + bounds_r) * 0.5f, (bounds_b + bounds_t) * 0.5f, bounds_r - bounds_l, bounds_t - bounds_b, b_r, b_g, b_b);

    auto rects_arr = json["rects"];
    for (auto &rect : rects_arr)
    {
        float rect_l = rect[0].get<float>();
        float rect_r = rect[1].get<float>();
        float rect_b = rect[2].get<float>();
        float rect_t = rect[3].get<float>();

        m_rects.push_back(new Rect((rect_l + rect_r) * 0.5f, (rect_b + rect_t) * 0.5f, rect_r - rect_l, rect_t - rect_b, m_fr, m_fg, m_fb));
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
}

void Map::Render(SDL_Window *window, SDL_Renderer *renderer, float camera_x, float camera_y)
{
    SDL_SetRenderDrawColor(renderer, m_fr, m_fg, m_fb, 255);
    SDL_RenderClear(renderer);

    m_bounds->Render(window, renderer, camera_x, camera_y);

    for (Rect *rect : m_rects)
    {
        rect->Render(window, renderer, camera_x, camera_y);
    }

    m_player->Render(window, renderer, camera_x, camera_y);
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