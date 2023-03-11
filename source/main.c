/*
    Flappy Bird for 3DS

    Created by breadone on 2023-03-05

*/
#include <citro2d.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#include "sprites.h"

#define MAX_SPRITES   768
#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240


typedef struct {
	C2D_Sprite spr;
	float x, y; // pos
} Sprite;

// init spritesheet
static C2D_SpriteSheet spriteSheet;
static Sprite sprites[MAX_SPRITES];

static void initSprites() {
	size_t numImages = C2D_SpriteSheetCount(spriteSheet);

	for (size_t i = 0; i < numImages; i++) {
		Sprite* thisSprite = &sprites[i];
		float x = SCREEN_WIDTH / 2;
		float y = SCREEN_HEIGHT / 2;

		C2D_SpriteFromSheet(&thisSprite->spr, spriteSheet, i);
		C2D_SpriteSetCenter(&thisSprite->spr, 0.5f, 0.5f);
		C2D_SpriteSetPos(&thisSprite->spr, x, y);
		thisSprite->x = x;
		thisSprite->y = y;
	}

    // set bird consts
    C2D_SpriteSetPos(&sprites[SPR_BIRD].spr, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 3);

    // set bottom screen scorecard
    C2D_SpriteSetCenter(&sprites[SPR_SCORECARD].spr, 0.0, 0.0);
    C2D_SpriteSetPos(&sprites[SPR_SCORECARD].spr, 2, 40);
}

void moveSprite(Sprite *spr, s16 dx, s16 dy) {
	// stop sprite going OOB
	if (spr->x >= SCREEN_WIDTH || spr->x < 1 || spr->y >= SCREEN_HEIGHT || spr->x < 1) {
		return;
	}

	spr->x += dx;
	spr->y += dy;

	C2D_SpriteSetPos(&spr->spr, spr->x, spr->y);
}

void drawSprite(int id) {
    C2D_DrawSprite(&sprites[id].spr);
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
    font = C2D_FontLoad("romfs:/fbfont.bcfnt");
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

	// set bg properties
	C2D_SpriteSetPos(&sprites[SPR_BG].spr, 200, 120); // center bg
	C2D_SpriteSetScale(&sprites[SPR_BG].spr, 2.7778, 2.7907); // scale image to 400x240 (3ds screen res)

    int score = 0;

    // Main loop
    while (aptMainLoop()) {
        hidScanInput();

        // Respond to user input
        u32 kDown = hidKeysDown();
        if (kDown & KEY_START)
            break; // break in order to return to hbmenu


		// gravity calcs
		v += a;
		moveSprite(&sprites[SPR_BIRD], 0, v);
		C2D_SpriteSetRotationDegrees(&sprites[SPR_BIRD].spr, v*9.8);

		if (kDown & KEY_A) {
			v = -5.5;
			moveSprite(&sprites[SPR_BIRD], 0, v);
            // temp score = num of flaps
            score++;
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
            drawSprite(SPR_BG);
            drawSprite(SPR_BIRD);

            C2D_TargetClear(bottom, C2D_Color32f(0.3294f, 0.7529f, 0.7882f, 1.0f));
            C2D_SceneBegin(bottom);
            drawSprite(SPR_SCORECARD);
            // C2D_DrawSprite(&scoreSprite->spr);
            

            C2D_DrawText(&scoreText, 0, 255, 90, 0.0f, 1.0f, 1.0f);
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
