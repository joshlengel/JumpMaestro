#pragma once

class Collider;

struct Collision
{
    float dt;
    float x, y;
    float nx, ny;
    const Collider *collider;
};

class Player;

class Collider
{
public:
    virtual ~Collider() = default;

    virtual void CheckCollision(const Player &player, Collision &coll) const = 0;
};

class AABB : public Collider
{
public:
    AABB(float l, float r, float b, float t);

    virtual void CheckCollision(const Player &player, Collision &coll) const override;

public:
    float l, r, b, t;
};

class AABBInverse : public Collider
{
public:
    AABBInverse(float l, float r, float b, float t);

    virtual void CheckCollision(const Player &player, Collision &coll) const override;

public:
    float l, r, b, t;
};