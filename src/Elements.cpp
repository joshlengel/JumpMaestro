#include"Elements.h"
#include"Constants.h"
#include"Quad.h"
#include"Player.h"

#include<iostream>

Rigid::Rigid(float friction, float bounciness, float r, float g, float b, QuadRenderer &renderer):
    r(r), g(g), b(b),
    renderer(renderer),
    friction(friction),
    bounciness(bounciness)
{}

void Rigid::Update(float dt)
{}

void Rigid::HandleCollision(Player &player, const Collision &collision)
{
    // Resolve collision
    if (collision.nx) player.vx = -bounciness * std::abs(player.vx) * collision.nx;
    if (collision.ny) player.vy = -bounciness * std::abs(player.vy) * collision.ny;

    if (collision.ny < 0.0f)
        player.SetHitGround(friction);
}

float Rigid::GetFriction() const { return friction; }

const std::map<std::string, Rect::Type> Rect::TYPES =
{
    { "normal", Rect::Type::NORMAL },
    { "ice",    Rect::Type::ICE    },
    { "sticky", Rect::Type::STICKY },
    { "bouncy", Rect::Type::BOUNCY },
};

Rect::Type Rect::GetType(const std::string &str) { return TYPES.at(str); }
Rect *Rect::ForType(Type type, float left, float right, float bottom, float top, float r, float g, float b, QuadRenderer &renderer)
{
    switch (type)
    {
        case Rect::Type::NORMAL: return new Rect(left, right, bottom, top, r, g, b, renderer);
        case Rect::Type::ICE:    return new IceRect(left, right, bottom, top, r, g, b, renderer);
        //case Rect::Type::STICKY: return new S(left, right, bottom, top, r, g, b, renderer);
        case Rect::Type::BOUNCY: return new BouncyRect(left, right, bottom, top, r, g, b, renderer);
        default:
            std::cerr << "Error creating Rect. No type for Rect::Type '" << static_cast<int>(type) << "'" << std::endl;
            exit(EXIT_FAILURE);
    }
}

Rect::Rect(float left, float right, float bottom, float top, float r, float g, float b, QuadRenderer &renderer):
    AABB(left, right, bottom, top),
    Rigid(Constants::RECTS["default"].friction, Constants::RECTS["default"].bounciness, r, g, b, renderer)
{}

void Rect::Render(GLFWwindow *window, float camera_x, float camera_y)
{
    Quad quad;
    quad.x = (AABB::l + AABB::r) * 0.5f - camera_x;
    quad.y = (AABB::b + AABB::t) * 0.5f - camera_y;
    quad.width = AABB::r - AABB::l;
    quad.height = AABB::t - AABB::b;
    quad.r = Rigid::r;
    quad.g = Rigid::g;
    quad.b = Rigid::b;
    quad.a = 1.0f;
    renderer.Add(quad);
}

Bounds::Bounds(float left, float right, float bottom, float top, float r, float g, float b, QuadRenderer &renderer):
    AABBInverse(left, right, bottom, top),
    Rigid(Constants::RECTS["default"].friction, Constants::RECTS["default"].bounciness, r, g, b, renderer)
{}

void Bounds::Render(GLFWwindow *window, float camera_x, float camera_y)
{
    Quad quad;
    quad.x = (AABBInverse::l + AABBInverse::r) * 0.5f - camera_x;
    quad.y = (AABBInverse::b + AABBInverse::t) * 0.5f - camera_y;
    quad.width = AABBInverse::r - AABBInverse::l;
    quad.height = AABBInverse::t - AABBInverse::b;
    quad.r = Rigid::r;
    quad.g = Rigid::g;
    quad.b = Rigid::b;
    quad.a = 1.0f;
    renderer.Add(quad);
}

// New rects
IceRect::IceRect(float left, float right, float bottom, float top, float r, float g, float b, QuadRenderer &renderer):
    Rect(left, right, bottom, top, r, g, b, renderer)
{
    friction = Constants::RECTS["ice"].friction;
}

void IceRect::HandleCollision(Player &player, const Collision &collision)
{
    Rect::HandleCollision(player, collision);
}

void IceRect::Render(GLFWwindow *window, float camera_x, float camera_y)
{
    Rect::Render(window, camera_x, camera_y);
}

/*StickyRect::StickyRect(float x, float y, float width, float height, float r, float g, float b, QuadRenderer &renderer):
    Rect(x, y, width, height, r, g, b, renderer)
{
    friction = Constants::RECTS["sticky"].friction;
}

void StickyRect::HandleCollision(Player &player, const Collision &collision)
{
    Rect::HandleCollision(player, collision);

    if (collision.colliding && collision.nx != 0.0f)
    {
        player.SetStuck();
    }
}

void StickyRect::Render(SDL_Window *window, float camera_x, float camera_y)
{
    Rect::Render(window, camera_x, camera_y);
}*/

BouncyRect::BouncyRect(float left, float right, float bottom, float top, float r, float g, float b, QuadRenderer &renderer):
    Rect(left, right, bottom, top, r, g, b, renderer)
{
    friction = Constants::RECTS["bouncy"].friction;
    bounciness = Constants::RECTS["bouncy"].bounciness;
}

void BouncyRect::HandleCollision(Player &player, const Collision &collision)
{
    Rect::HandleCollision(player, collision);
}

void BouncyRect::Render(GLFWwindow *window, float camera_x, float camera_y)
{
    Rect::Render(window, camera_x, camera_y);
}