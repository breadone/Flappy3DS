#include "../Bakery2D/Level/Level.hpp"

#define SPRITES_H

#define SPR_BIRD 0

#define SPR_BG 1
#define SPR_FLOOR 2
#define SPR_LOGO 3
#define SPR_PIPEBOTTOM 4
#define SPR_PIPETOP 5

#define SPR_SCORECARD 6
#define SPR_BOTHPIPES 7

#define NUM_PIPES 5

struct MainLevel: Level {
    float a = 0.4; // acceleration
	float v = 0; // velocity
    float pipeSpeed = -1;

    Sprite pipes[NUM_PIPES];

    int score = 0;
    bool gameOver = false;

    C2D_TextBuf g_staticBuf;
    C2D_Text scoreText;
    C2D_Font font;

    MainLevel(C3D_RenderTarget* top, C3D_RenderTarget* bottom): Level(this, "romfs:/gfx/sprites.t3x", top, bottom) {}

    void setup() {
        //* SPRITE INIT *//
        // set bird consts
        sprites[SPR_BIRD].setPosition(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 3);
        sprites[SPR_BIRD].setHitbox(17, 12);

        // set bottom screen scorecard
        sprites[SPR_SCORECARD].setCenter(0.0, 0.0);

        sprites[SPR_SCORECARD].setPosition(2, 40);

        sprites[SPR_PIPETOP].setPosition(280, 3);
        sprites[SPR_PIPEBOTTOM].setPosition(280, 280);

        // set bg properties
        sprites[SPR_BG].setPosition(200, 120);
        C2D_SpriteSetScale(&sprites[SPR_BG].spr, 2.7778, 2.7907); // scale image to 400x240 (3ds screen res)

        // if theres a better way to do this,,, i dont know it
        for (int i = 0; i < NUM_PIPES; i++) {
            memcpy(&pipes[i], &sprites[SPR_BOTHPIPES], sizeof(sprites[SPR_BOTHPIPES]));
            pipes[i].setCenter(0.5f, 0.5f);
            pipes[i].setPosition(SCREEN_WIDTH + 40 + i * (100), (rand() % 150) + 50);
            pipes[i].setHitbox(26, 403);
        }

        //* FONT INIT *//
        // Load fonts and text
        g_staticBuf = C2D_TextBufNew(4096);
        font = C2D_FontLoad("romfs:/gfx/fbfont.bcfnt");
        C2D_TextFontParse(&scoreText, font, g_staticBuf, "0");
        C2D_TextOptimize(&scoreText);
    }

    void update() {
        hidScanInput();

        // Respond to user input
        u32 kDown = hidKeysDown();
        if (kDown & KEY_START)
            svcBreak(USERBREAK_USER); // break in order to return to hbmenu


		// gravity calcs
		v += a;
        sprites[SPR_BIRD].move(0, v);
		C2D_SpriteSetRotationDegrees(&sprites[SPR_BIRD].spr, v*9.8);

		if (!gameOver && (kDown & KEY_A)) {
			v = -5.5;
            sprites[SPR_BIRD].move(0, v, true);
		}

        // bird hitting ground is game over
        if (sprites[SPR_BIRD].getPosY() == SCREEN_HEIGHT) {
            gameOver = true;
        }

        if (!gameOver) {
            for (int i = 0; i < NUM_PIPES; i++) {
                pipes[i].move(pipeSpeed, 0, true);

                // collision detection
                if (pipes[i].isCollidingWith(sprites[SPR_BIRD])) {
                    gameOver = true;
                    break;
                }

                // inc score
                if (pipes[i].getPosX() == sprites[SPR_BIRD].getPosX())
                    score++;          

                // send pipes back to front of screen
                if (pipes[i].getPosX() < -(SCREEN_WIDTH/NUM_PIPES)) {
                    pipes[i].setPosition(SCREEN_WIDTH, (rand() % 150) + 50);
                }    
            }
        }

        // make score text
        char scoreString[(((sizeof score) * CHAR_BIT) + 2)/3 + 2];
        sprintf(scoreString, "%d", score);
        C2D_TextFontParse(&scoreText, font, g_staticBuf, scoreString);
        C2D_TextOptimize(&scoreText);
    }

    void drawTop() {
        sprites[SPR_BG].draw();
        sprites[SPR_BIRD].draw();

        for (int i = 0; i < NUM_PIPES; i++) {
            pipes[i].draw();
        }
    }

    void drawBottom() {
        sprites[SPR_SCORECARD].draw();
        C2D_DrawText(&scoreText, 0, 240, 88, 0.0f, 0.9f, 0.9f);
    }

    void cleanup() override {
        C2D_SpriteSheetFree(spritesheet);
        C2D_TextBufDelete(g_staticBuf);
        C2D_FontFree(font);
    }

};