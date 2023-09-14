/*
    Level.hpp
    Bakery2D
    Created by breadone
*/

#pragma once
#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <citro2d.h>
#include "../Sprite/Sprite.hpp"

#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240
#define MAX_SPRITES   768
/**
 The Level class, the backdrops for your game, and the gateway to how they are controlled.

  Example: 
  ```cpp
    struct mainLevel: Level {
        mainLevel(): Level(this, "romfs:/gfx/mainlevel/sprites.t3x");

        int score = 0;
        bool gameOver = false;

        void setup() {
            // set background sprite to correct location and scale
            sprites[0].setPosition(200, 120);
            C2D_SpriteSetScale(&sprites[SPR_BG].spr, 2.7778, 2.7907); // scale image to 400x240 (3ds screen res)
        }

        void update() {
            sprites[1].move(0, -0.4); // move bird down
            sprites[2].move(-1, 0); // move pipe left

            // other game logic goes here...
        }

        void drawTop() {
            sprites[0].draw(); // draw background
            sprites[1].draw(); // draw bird
            sprites[2].draw(); // draw pipe
        }

        void drawBottom() {
            sprites[3].draw(); // draw scorecard
        }
    };
  ```
 */
class Level {
public:
    Sprite sprites[MAX_SPRITES];
    C2D_SpriteSheet spritesheet;
    C3D_RenderTarget* top;
    C3D_RenderTarget* bottom;
    Level* level;
    bool hasBeenSetup = false;

    virtual void setup() = 0;
    virtual void update() = 0;
    virtual void drawTop() = 0;
    virtual void drawBottom() = 0;

    template <typename T>
    Level(T* levelName, char* spritesheetPath, C3D_RenderTarget* top, C3D_RenderTarget* bottom);

    // sets up the level if it hasnt been, then updates the logic, and draws the frame
    void run();

    virtual void cleanup();
};
#endif  
