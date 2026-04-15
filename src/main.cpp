// 코드 업데이트 전 CLAUDE.md 참고, 필요하면 CLAUDE.md 업데이트

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
