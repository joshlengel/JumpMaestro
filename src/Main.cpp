#include"App.h"
#include"Map.h"

static const int WIDTH = 800;
static const int HEIGHT = 600;

int main()
{
    App app(WIDTH, HEIGHT, "Jump Maestro");
    app.Start();
}