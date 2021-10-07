#include"Collider.h"
#include"Player.h"
#include"Constants.h"

#include<iostream>
#include<cmath>

AABB::AABB(float l, float r, float b, float t):
    l(l), r(r), b(b), t(t)
{}

AABBInverse::AABBInverse(float l, float r, float b, float t):
    l(l), r(r), b(b), t(t)
{}

void AABB::CheckCollision(const Player &player, Collision &coll) const
{
    // check for intersection
    float p_hs = player.scale * 0.5f;
    float p_l = player.x - p_hs;
    float p_r = player.x + p_hs;
    float p_b = player.y - p_hs;
    float p_t = player.y + p_hs;

    // r = p_l + player.vx * t
    // => t = (r - p_l) / player.vx
    if (player.vx < 0.0f && p_l >= r)
    {
        float time = (r - p_l) / player.vx;
        if (time < coll.dt)
        {
            float _p_b = p_b + player.vy * time;
            float _p_t = p_t + player.vy * time;

            if (_p_b < t && _p_t > b)
            {
                // Collision occurred
                coll.dt = time;
                coll.x = player.x + player.vx * time + Constants::EPSILON;
                coll.y = player.y + player.vy * time;
                coll.nx = -1.0f;
                coll.ny =  0.0f;
                coll.collider = this;
            }
        }
    }

    // l = p_r + player.vx * t
    // => t = (l - p_r) / player.vx
    if (player.vx > 0.0f && p_r <= l)
    {
        float time = (l - p_r) / player.vx;
        if (time < coll.dt)
        {
            float _p_b = p_b + player.vy * time;
            float _p_t = p_t + player.vy * time;

            if (_p_b < t && _p_t > b)
            {
                // Collision occurred
                coll.dt = time;
                coll.x = player.x + player.vx * time - Constants::EPSILON;
                coll.y = player.y + player.vy * time;
                coll.nx =  1.0f;
                coll.ny =  0.0f;
                coll.collider = this;
            }
        }
    }

    // t = p_b + player.vy * t
    // => t = (t - p_b) / player.vy
    if (player.vy < 0.0f && p_b >= t)
    {
        float time = (t - p_b) / player.vy;
        if (time < coll.dt)
        {
            float _p_l = p_l + player.vx * time;
            float _p_r = p_r + player.vx * time;

            if (_p_l < r && _p_r > l)
            {
                // Collision occurred
                coll.dt = time;
                coll.x = player.x + player.vx * time;
                coll.y = player.y + player.vy * time + Constants::EPSILON;
                coll.nx =  0.0f;
                coll.ny = -1.0f;
                coll.collider = this;
            }
        }
    }

    // b = p_t + player.vy * t
    // => t = (b - p_t) / player.vy
    if (player.vy > 0.0f && p_t <= b)
    {
        float time = (b - p_t) / player.vy;
        if (time < coll.dt)
        {
            float _p_l = p_l + player.vx * time;
            float _p_r = p_r + player.vx * time;

            if (_p_l < r && _p_r > l)
            {
                // Collision occurred
                coll.dt = time;
                coll.x = player.x + player.vx * time;
                coll.y = player.y + player.vy * time - Constants::EPSILON;
                coll.nx =  0.0f;
                coll.ny =  1.0f;
                coll.collider = this;
            }
        }
    }
}

void AABBInverse::CheckCollision(const Player &player, Collision &coll) const
{
    // check for intersection
    float p_hs = player.scale * 0.5f;

    float p_l = player.x - p_hs;
    float p_r = player.x + p_hs;
    float p_b = player.y - p_hs;
    float p_t = player.y + p_hs;

    // l = p_l + player.vx * t
    // => t = (l - p_l) / player.vx
    if (player.vx < 0.0f && p_l >= l)
    {
        float time = (l - p_l) / player.vx;
        if (time < coll.dt)
        {
            float _p_b = p_b + player.vy * time;
            float _p_t = p_t + player.vy * time;

            if (_p_b <= t && _p_t >= b)
            {
                // Collision occurred
                coll.dt = time;
                coll.x = player.x + player.vx * time + Constants::EPSILON;
                coll.y = player.y + player.vy * time;
                coll.nx = -1.0f;
                coll.ny =  0.0f;
                coll.collider = this;
            }
        }
    }

    // r = p_r + player.vx * t
    // => t = (r - p_r) / player.vx
    if (player.vx > 0.0f && p_r <= r)
    {
        float time = (r - p_r) / player.vx;
        if (time < coll.dt)
        {
            float _p_b = p_b + player.vy * time;
            float _p_t = p_t + player.vy * time;

            if (_p_b <= t && _p_t >= b)
            {
                // Collision occurred
                coll.dt = time;
                coll.x = player.x + player.vx * time - Constants::EPSILON;
                coll.y = player.y + player.vy * time;
                coll.nx =  1.0f;
                coll.ny =  0.0f;
                coll.collider = this;
            }
        }
    }

    // b = p_b + player.vy * t
    // => t = (b - p_b) / player.vy
    if (player.vy < 0.0f && p_b >= b)
    {
        float time = (b - p_b) / player.vy;
        if (time < coll.dt)
        {
            float _p_l = p_l + player.vx * time;
            float _p_r = p_r + player.vx * time;

            if (_p_l <= r && _p_r >= l)
            {
                // Collision occurred
                coll.dt = time;
                coll.x = player.x + player.vx * time;
                coll.y = player.y + player.vy * time + Constants::EPSILON;
                coll.nx =  0.0f;
                coll.ny = -1.0f;
                coll.collider = this;
            }
        }
    }

    // t = p_t + player.vy * t
    // => t = (t - p_t) / player.vy
    if (player.vy > 0.0f && p_t <= t)
    {
        float time = (t - p_t) / player.vy;
        if (time < coll.dt)
        {
            float _p_l = p_l + player.vx * time;
            float _p_r = p_r + player.vx * time;

            if (_p_l <= r && _p_r >= l)
            {
                // Collision occurred
                coll.dt = time;
                coll.x = player.x + player.vx * time;
                coll.y = player.y + player.vy * time - Constants::EPSILON;
                coll.nx =  0.0f;
                coll.ny =  1.0f;
                coll.collider = this;
            }
        }
    }
}