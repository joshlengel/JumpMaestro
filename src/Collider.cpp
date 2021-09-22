#include"Collider.h"

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
    // TODO
    collision.colliding = false;
}

void AABBInverse::CheckCollision(const Collider &other, Collision &collision) const
{
    // TODO
    collision.colliding = false;
}