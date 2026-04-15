#define STB_IMAGE_IMPLEMENTATION
#include "app.h"

int main()
{
    App app;
    if (!app.init())
        return 1;

    while (!app.shouldClose())
    {
        app.update();
        app.render();
    }

    return 0;
}
