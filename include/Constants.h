#pragma once

#include<map>
#include<string>

struct PlayerInfo
{
    float speed;
    float jump_speed;
};

struct RectInfo
{
    float friction;
};

class Constants
{
public:
    static void Init(const std::string &file);

    static float EPSILON;
    static std::map<std::string, PlayerInfo> PLAYERS;
    static std::map<std::string, RectInfo> RECTS;
};