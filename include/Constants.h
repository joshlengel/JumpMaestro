#pragma once

#include<map>
#include<string>

struct PlayerInfo
{
    float acceleration;
    float max_speed;
    float jump_speed;
};

struct RectInfo
{
    float friction;
    float bounciness;
};

class Constants
{
public:
    static void Init(const std::string &file);

    static float EPSILON;
    static std::map<std::string, PlayerInfo> PLAYERS;
    static std::map<std::string, RectInfo> RECTS;
};