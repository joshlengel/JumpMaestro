#pragma once

#include"GameObject.h"
#include"Collider.h"

#include<map>
#include<string>

class Player;
class Target;

#define NEW_RECT(name) \
class name##Rect : public Rect \
{ \
public: \
    name##Rect(float x, float y, float width, float height, float r, float g, float b, QuadRenderer &renderer); \
 \
    virtual void HandleCollision(Player &player, const Collision &collision) override; \
    virtual void Render(SDL_Window *window, float camera_x, float camera_y) override; \
};

class QuadRenderer;

class Rigid : public GameObject
{
public:
    Rigid(float friction, float bounciness, float r, float g, float b, QuadRenderer &renderer);

    virtual void Update(float dt) override;
    virtual void Render(SDL_Window *window, float camera_x, float camera_y) = 0;

    virtual void HandleCollision(Player &player, const Collision &collision);
    float GetFriction() const;
    float GetBounciness() const;

protected:
    float r, g, b;
    QuadRenderer &renderer;

    float friction;
    float bounciness;
};

class Rect : public AABB, public Rigid
{
public:
    enum class Type
    {
        NORMAL,
        ICE,
        STICKY,
        BOUNCY
    };

    static Type GetType(const std::string &str);
    static Rect *ForType(Type type, float left, float right, float bottom, float top, float r, float g, float b, QuadRenderer &renderer);

    Rect(float left, float right, float bottom, float top, float r, float g, float b, QuadRenderer &renderer);

    virtual void Render(SDL_Window *window, float camera_x, float camera_y) override;

private:
    static const std::map<std::string, Type> TYPES;
};

class Bounds : public AABBInverse, public Rigid
{
public:
    Bounds(float x, float y, float width, float height, float r, float g, float b, QuadRenderer &renderer);

    virtual void Render(SDL_Window *window, float camera_x, float camera_y) override;
};

NEW_RECT(Ice)
//NEW_RECT(Sticky)
NEW_RECT(Bouncy)