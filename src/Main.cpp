#include"App.h"
#include"Map.h"

struct SDL_Window;

static const int WIDTH = 800;
static const int HEIGHT = 600;

static Map *map;

void Loop(float dt, SDL_Window *window)
{
    map->Update(dt);
    map->Render(window, 0.0f, 0.0f);
}

void Event(SDL_Event &event)
{
    map->HandleEvent(event);
}

int main()
{
    App app(WIDTH, HEIGHT, "Jump Maestro");

    map = new Map("assets/map.json");

    app.Show();
    app.Start(&Loop, &Event);

    delete map;
}