/*
    Flappy Bird for 3DS

    Created by breadone on 2023-03-05

*/
#include <citro2d.h>

#include <stdlib.h>
#include <time.h>
#include <limits.h>

#include "MainLevel.cpp"

int main(int argc, char* argv[]) {

    // Init libs
    romfsInit();
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    // consoleInit(GFX_BOTTOM, NULL);

    // Create screens and level
    C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
    Level* level;

    MainLevel* game = new MainLevel(top, bottom);
    level = game;

    // Main loop
    while (aptMainLoop() && level->isActive) {
        level->run();
    }
    
    level->cleanup();

    // Deinit libs
    C2D_Fini();
    C3D_Fini();
    gfxExit();
    romfsExit();
    return 0;
}
