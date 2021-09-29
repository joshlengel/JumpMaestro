#include"Constants.h"

#include<nlohmann/json.hpp>
#include<fstream>
#include<iostream>

float Constants::EPSILON;
std::map<std::string, PlayerInfo> Constants::PLAYERS;
std::map<std::string, RectInfo> Constants::RECTS;

void Constants::Init(const std::string &file)
{
    std::ifstream f(file);
    if (!f)
    {
        std::cerr << "Error opening file '" << file << "'" << std::endl;
        exit(EXIT_FAILURE);
    }

    nlohmann::json json;
    f >> json;

    EPSILON = json["epsilon"].get<float>();
    auto &player_objs = json["players"];
    auto &rect_objs = json["rects"];

    for (auto &entry : player_objs.items())
    {
        auto &value = entry.value();
        PLAYERS[entry.key()] = { .speed=value["speed"].get<float>(), .jump_speed=value["jump_speed"].get<float>() };
    }

    for (auto &entry : rect_objs.items())
    {
        auto &value = entry.value();
        RECTS[entry.key()] = { .friction=value["friction"].get<float>() };
    }
}