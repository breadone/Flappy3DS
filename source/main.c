// Simple citro2d sprite drawing example
// Images borrowed from:
//   https://kenney.nl/assets/space-shooter-redux
#include <citro2d.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SPRITES   768
#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240

// Simple sprite struct
typedef struct
{
	C2D_Sprite spr;
	float x, y; // pos
} Sprite;

// init spritesheet
static C2D_SpriteSheet spriteSheet;
static Sprite sprites[MAX_SPRITES];

//---------------------------------------------------------------------------------
static void initSprites() {
//---------------------------------------------------------------------------------
	// size_t numImages = C2D_SpriteSheetCount(spriteSheet);
	// srand(time(NULL));

	// for (size_t i = 0; i < MAX_SPRITES; i++)
	// {	
	// 	Sprite* thisSprite = &sprites[i];

	// 	// Random image, position, rotation and speed
	// //  C2D_SpriteFromSheet(*Sprite, C2D_SpriteSheet, int);
	// 	C2D_SpriteFromSheet(&thisSprite->spr, spriteSheet, i);
	// 	C2D_SpriteSetCenter(&thisSprite->spr, 0.5f, 0.5f);
	// 	C2D_SpriteSetPos(&thisSprite->spr, rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT);
	// 	// C2D_SpriteSetRotation(&sprite->spr, C3D_Angle(rand()/(float)RAND_MAX));
	// 	thisSprite->dx = rand()*4.0f/RAND_MAX - 2.0f;
	// 	thisSprite->dy = rand()*4.0f/RAND_MAX - 2.0f;
	// }

	// for (size_t i = 0; i < MAX_SPRITES; i++) {
	// 	Sprite* thisSprite = &sprites[i];
	// 	float x = rand() % SCREEN_WIDTH;
	// 	float y = rand() % SCREEN_HEIGHT;

	// 	C2D_SpriteFromSheet(&thisSprite->spr, spriteSheet, i);
	// 	C2D_SpriteSetCenter(&thisSprite->spr, 0.5f, 0.5f);
	// 	C2D_SpriteSetPos(&thisSprite->spr, x, y);
	// 	thisSprite->x = x;
	// 	thisSprite->y = y;
	// }

		Sprite* thisSprite = &sprites[0];
		float x = rand() % SCREEN_WIDTH;
		float y = rand() % SCREEN_HEIGHT;

		C2D_SpriteFromSheet(&thisSprite->spr, spriteSheet, 0);
		C2D_SpriteSetCenter(&thisSprite->spr, 0.5f, 0.5f);
		C2D_SpriteSetPos(&thisSprite->spr, x, y);
		thisSprite->x = x;
		thisSprite->y = y;


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

int main(int argc, char* argv[]) {

	// Init libs
	romfsInit();
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	consoleInit(GFX_BOTTOM, NULL);

	// Create screens
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	// C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

	// Load graphics
	spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	if (!spriteSheet) {
		printf("No");
		// svcBreak(USERBREAK_PANIC);
	}

	// Initialize sprites
	initSprites();


	// Main loop
	while (aptMainLoop())
	{
		hidScanInput();

		// Respond to user input
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu

		// get circlepad position
		circlePosition cpos;
		hidCircleRead(&cpos);
		// move spr based on it
		moveSprite(&sprites[0], cpos.dx, cpos.dy);

		printf("Bird Pos: %f, %f\n", sprites[0].x, sprites[0].y);
			

		// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, C2D_Color32f(0.2352941176f, 0.8392156863f, 0.8392156863f, 1.0f));
		C2D_SceneBegin(top);

		C2D_DrawSprite(&sprites[0].spr);
		C3D_FrameEnd(0);

		// // Flush and swap framebuffers
		// gfxFlushBuffers();
		// gfxSwapBuffers();

		// //Wait for VBlank
		// gspWaitForVBlank();
	}

	// Delete graphics
	C2D_SpriteSheetFree(spriteSheet);

	// Deinit libs
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();
	return 0;
}
