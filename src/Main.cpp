#include"App.h"
#include"Map.h"

#include<SDL2/SDL.h>

static const int WIDTH = 800;
static const int HEIGHT = 600;

static Map *map;

void Loop(SDL_Window *window, SDL_Renderer *renderer)
{
    map->Update(0.0f);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    map->Render(window, renderer, 0.0f, 0.0f);

    SDL_RenderPresent(renderer);
}

void Event(SDL_Event &event)
{}

int main()
{
    App app(WIDTH, HEIGHT, "Jump Maestro");

    map = new Map;

    app.Show();
    app.Start(&Loop, &Event);

    delete map;
}