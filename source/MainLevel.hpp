#pragma once
#ifndef MAINLEVEL_HPP
#define MAINLEVEL_HPP

#include "Level.hpp"

#define SPR_BIRD 0

#define SPR_BG 1
#define SPR_FLOOR 2
#define SPR_LOGO 3
#define SPR_PIPEBOTTOM 4
#define SPR_PIPETOP 5

#define SPR_SCORECARD 6
#define SPR_BOTHPIPES 7

#define NUM_PIPES 5

class MainLevel: public Level {
public:
    float a = 0.4; // acceleration
	float v = 0; // velocity
    float pipeSpeed = -1;

    Sprite pipes[NUM_PIPES];

    int score = 0;
    bool gameOver = false;

    C2D_TextBuf g_staticBuf;
    C2D_Text scoreText;
    C2D_Font font;

    MainLevel(C3D_RenderTarget* top, C3D_RenderTarget* bottom);

    void setup();

    void update();

    void drawTop();

    void drawBottom();

    void cleanup();

};


#endif