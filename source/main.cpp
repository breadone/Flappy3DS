/*
    Flappy Bird for 3DS

    Created by breadone on 2023-03-05

*/
#include <citro2d.h>

#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include "Sprite.hpp"

#include "SpriteList.h"

#define MAX_SPRITES   768
#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240
#define NUM_PIPES 4

// init spritesheet
static C2D_SpriteSheet spriteSheet;
Sprite sprites[MAX_SPRITES];
Sprite pipes[NUM_PIPES];

static void initSprites() {
    srand(time(NULL));
	size_t numImages = C2D_SpriteSheetCount(spriteSheet);

	for (size_t i = 0; i < numImages; i++) {
		Sprite* thisSprite = &sprites[i];
		float x = SCREEN_WIDTH / 2;
		float y = SCREEN_HEIGHT / 2;

		C2D_SpriteFromSheet(&thisSprite->spr, spriteSheet, i);
		C2D_SpriteSetCenter(&thisSprite->spr, 0.5f, 0.5f);
        thisSprite->setPosition(x, y);
	}

    // set bird consts
    sprites[SPR_BIRD].setPosition(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 3);

    // set bottom screen scorecard
    sprites[SPR_SCORECARD].setCenter(0.0, 0.0);

    sprites[SPR_SCORECARD].setPosition(2, 40);

    sprites[SPR_PIPETOP].setPosition(280, 3);
    sprites[SPR_PIPEBOTTOM].setPosition(280, 280);

    // if theres a better way to do this,,, i dont know it
    for (int i = 0; i < NUM_PIPES; i++) {
        memcpy(&pipes[i], &sprites[SPR_BOTHPIPES], sizeof(sprites[SPR_BOTHPIPES]));
        pipes[i].setCenter(0.5f, 0.5f);
        pipes[i].setPosition(SCREEN_WIDTH + 20 + i * (SCREEN_WIDTH/NUM_PIPES), (rand() % (SCREEN_HEIGHT-60)) + 60);
    }
}


int main(int argc, char* argv[]) {

    // Init libs
    romfsInit();
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    // consoleInit(GFX_BOTTOM, NULL);

    // Create screens
    C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

    // Load fonts and text
    C2D_TextBuf g_staticBuf;
    C2D_Text scoreText;
    C2D_Font font;

    g_staticBuf = C2D_TextBufNew(4096);
    font = C2D_FontLoad("romfs:/gfx/fbfont.bcfnt");
    C2D_TextFontParse(&scoreText, font, g_staticBuf, "0");
    C2D_TextOptimize(&scoreText);

    // Load graphics
    spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
    if (!spriteSheet) {
        svcBreak(USERBREAK_PANIC);
    }

	// Initialize sprites
    initSprites();

	// init gravity calc vars
	float a = 0.4; // acceleration
	float v = 0; // velocity
    float pipeSpeed = -1;

	// set bg properties
    sprites[SPR_BG].setPosition(200, 120);
	C2D_SpriteSetScale(&sprites[SPR_BG].spr, 2.7778, 2.7907); // scale image to 400x240 (3ds screen res)

    size_t score = 0;

    // Main loop
    while (aptMainLoop()) {
        hidScanInput();

        // Respond to user input
        u32 kDown = hidKeysDown();
        if (kDown & KEY_START)
            break; // break in order to return to hbmenu


		// gravity calcs
		v += a;
        sprites[SPR_BIRD].move(0, v);
		C2D_SpriteSetRotationDegrees(&sprites[SPR_BIRD].spr, v*9.8);

        

		if (kDown & KEY_A) {
			v = -5.5;
            sprites[SPR_BIRD].move(0, v);
		}

        for (int i = 0; i < NUM_PIPES; i++) {
            pipes[i].move(pipeSpeed, 0, true);

            // inc score
            if (pipes[i].getPosX() == sprites[SPR_BIRD].getPosX())
                score++;          

            // send pipes back to front of screen
            if (pipes[i].getPosX() < -(SCREEN_WIDTH/NUM_PIPES)) {
                pipes[i].setPosition(SCREEN_WIDTH, (rand() % (SCREEN_HEIGHT-60)) + 60);
            }

            
        }

       

        // make score text
        char scoreString[(((sizeof score) * CHAR_BIT) + 2)/3 + 2];
        sprintf(scoreString, "%d", score);
        C2D_TextFontParse(&scoreText, font, g_staticBuf, scoreString);
        C2D_TextOptimize(&scoreText);


        // Render the scene
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
            C2D_TargetClear(top, C3D_CLEAR_COLOR);
            C2D_SceneBegin(top);
            sprites[SPR_BG].draw();
            sprites[SPR_BIRD].draw();

            for (int i = 0; i < NUM_PIPES; i++) {
                pipes[i].draw();
            }

            C2D_TargetClear(bottom, C2D_Color32f(0.3294f, 0.7529f, 0.7882f, 1.0f));
            C2D_SceneBegin(bottom);
            // drawSprite(SPR_SCORECARD);
            sprites[SPR_SCORECARD].draw();
            // C2D_DrawSprite(&scoreSprite->spr);
            

            C2D_DrawText(&scoreText, 0, 240, 88, 0.0f, 0.9f, 0.9f);
        C3D_FrameEnd(0);

        
    }

    // Delete graphics
    C2D_SpriteSheetFree(spriteSheet);
    C2D_TextBufDelete(g_staticBuf);
    C2D_FontFree(font);

    // Deinit libs
    C2D_Fini();
    C3D_Fini();
    gfxExit();
    romfsExit();
    return 0;
}
