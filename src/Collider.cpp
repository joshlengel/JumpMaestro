#include"Collider.h"

#include<iostream>
#include<cmath>

Collider::Collider(float x, float y, float vx, float vy):
    x(x), y(y),
    vx(vx), vy(vy)
{}

Collider::Type AABB::GetType() const { return Type::AABB; }
Collider::Type AABBInverse::GetType() const { return Type::AABB_INVERSE; }

AABB::AABB(float x, float y, float vx, float vy, float width, float height):
    Collider(x, y, vx, vy),
    width(width), height(height)
{}

AABBInverse::AABBInverse(float x, float y, float vx, float vy, float width, float height):
    Collider(x, y, vx, vy),
    width(width), height(height)
{}

void AABB::CheckCollision(const Collider &other, Collision &collision) const
{
    switch (other.GetType())
    {
        case Type::AABB:         CheckCollision(reinterpret_cast<const AABB&>(other), collision); break;
        case Type::AABB_INVERSE: CheckCollision(reinterpret_cast<const AABBInverse&>(other), collision); break;
        default:
            std::cerr << "Error checking collision. Unknown Collider::Type '" << static_cast<int>(other.GetType()) << "'" << std::endl;
            exit(EXIT_FAILURE);
    }
}

void AABB::CheckCollision(const AABB &other, Collision &collision) const
{
    // check for intersection
    float hw = width * 0.5f;
    float hh = height * 0.5f;
    float o_hw = other.width * 0.5f;
    float o_hh = other.height * 0.5f;

    float l = x - hw;
    float r = x + hw;
    float b = y - hh;
    float t = y + hh;
    float o_l = other.x - o_hw;
    float o_r = other.x + o_hw;
    float o_b = other.y - o_hh;
    float o_t = other.y + o_hh;

    if (r <= o_l || o_r <= l || t <= o_b || o_t <= b)
    {
        // No intersection
        collision.colliding = false;
    }
    else
    {
        // Colliding
        collision.colliding = true;

        float d1 = r - o_l;
        float d2 = o_r - l;
        float d3 = t - o_b;
        float d4 = o_t - b;

        collision.penetration_depth = INFINITY;

        if (d1 < collision.penetration_depth)
        {
            collision.penetration_depth = d1;
            collision.cx = o_l + collision.penetration_depth * 0.5f;
            collision.cy = (y + other.y) * 0.5f;
            collision.nx = 1.0f;
            collision.ny = 0.0f;
        }

        if (d2 < collision.penetration_depth)
        {
            collision.penetration_depth = d2;
            collision.cx = l + collision.penetration_depth * 0.5f;
            collision.cy = (y + other.y) * 0.5f;
            collision.nx = -1.0f;
            collision.ny = 0.0f;
        }

        if (d3 < collision.penetration_depth)
        {
            collision.penetration_depth = d3;
            collision.cx = (x + other.x) * 0.5f;
            collision.cy = o_b + collision.penetration_depth * 0.5f;
            collision.nx = 0.0f;
            collision.ny = 1.0f;
        }

        if (d4 < collision.penetration_depth)
        {
            collision.penetration_depth = d4;
            collision.cx = (x + other.x) * 0.5f;
            collision.cy = b + collision.penetration_depth * 0.5f;
            collision.nx = 0.0f;
            collision.ny = -1.0f;
        }
    }
}

void AABB::CheckCollision(const AABBInverse &other, Collision &collision) const
{
    // check for intersection
    float hw = width * 0.5f;
    float hh = height * 0.5f;
    float o_hw = other.width * 0.5f;
    float o_hh = other.height * 0.5f;

    float l = x - hw;
    float r = x + hw;
    float b = y - hh;
    float t = y + hh;
    float o_l = other.x - o_hw;
    float o_r = other.x + o_hw;
    float o_b = other.y - o_hh;
    float o_t = other.y + o_hh;

    if (l >= o_l && r <= o_r && b >= o_b && t <= o_t)
    {
        // No intersection
        collision.colliding = false;
    }
    else
    {
        // Colliding
        collision.colliding = true;

        float d1 = r - o_r;
        float d2 = o_l - l;
        float d3 = t - o_t;
        float d4 = o_b - b;
        float max_d = d1;

        if (d1 > 0)
        {
            collision.penetration_depth = d1;
            collision.cx = o_r + collision.penetration_depth * 0.5f;
            collision.cy = y;
            collision.nx = 1.0f;
            collision.ny = 0.0f;
        }
        
        if (d2 > 0 && d2 > max_d)
        {
            max_d = d2;
            collision.penetration_depth = d2;
            collision.cx = l + collision.penetration_depth * 0.5f;
            collision.cy = y;
            collision.nx = -1.0f;
            collision.ny = 0.0f;
        }
        
        if (d3 > 0 && d3 > max_d)
        {
            max_d = d3;
            collision.penetration_depth = d3;
            collision.cx = x;
            collision.cy = o_t + collision.penetration_depth * 0.5f;
            collision.nx = 0.0f;
            collision.ny = 1.0f;
        }
        
        if (d4 > 0 && d4 > max_d)
        {
            max_d = d4;
            collision.penetration_depth = d4;
            collision.cx = x;
            collision.cy = b + collision.penetration_depth * 0.5f;
            collision.nx = 0.0f;
            collision.ny = -1.0f;
        }
    }
}

void AABBInverse::CheckCollision(const Collider &other, Collision &collision) const
{
    switch (other.GetType())
    {
        case Type::AABB:
            other.CheckCollision(*this, collision);
            collision.nx = -collision.nx;
            collision.ny = -collision.ny;
            break;
        case Type::AABB_INVERSE:
            std::cerr << "Cannot collide type Collider::Type::AABB_INVERSE with Collider::Type::AABB_INVERSE" << std::endl;
            exit(EXIT_FAILURE);
        default:
            std::cerr << "Error checking collision. Unknown Collider::Type '" << static_cast<int>(other.GetType()) << "'" << std::endl;
            exit(EXIT_FAILURE);
    }
}