#pragma once

struct Collision
{
    bool colliding;
    float cx, cy;
    float nx, ny;
    float penetration_depth;
};

class Collider
{
public:
    Collider(float x, float y, float vx, float vy);
    virtual ~Collider() = default;

    enum class Type
    {
        AABB,
        AABB_INVERSE
    };

    virtual void CheckCollision(const Collider &other, Collision &collision) const = 0;
    constexpr virtual Type GetType() const = 0;

protected:
    float x, y;
    float vx, vy;
};

class AABB : public Collider
{
public:
    AABB(float x, float y, float vx, float vy, float width, float height);

    virtual void CheckCollision(const Collider &other, Collision &collision) const override;
    virtual Type GetType() const override;

protected:
    float width, height;
};

class AABBInverse : public Collider
{
public:
    AABBInverse(float x, float y, float vx, float vy, float width, float height);

    virtual void CheckCollision(const Collider &other, Collision &collision) const override;
    virtual Type GetType() const override;

protected:
    float width, height;
};