#include"App.h"
#include"Map.h"

#include<SDL2/SDL.h>

static const int WIDTH = 800;
static const int HEIGHT = 600;

static Map *map;

void Loop(float dt, SDL_Window *window, SDL_Renderer *renderer)
{
    map->Update(dt);
    map->Render(window, renderer, 0.0f, 0.0f);

    SDL_RenderPresent(renderer);
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