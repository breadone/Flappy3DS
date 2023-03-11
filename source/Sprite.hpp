#pragma once

#ifndef SPRITE_H
#define SPRITE_H

#include <citro2d.h>

class Sprite {
public:
    Sprite();

    Sprite(C2D_Sprite* spr, float x, float y);

    void setPosition(float x, float y);

    void move(float dx, float dy, bool overrideOOB=false);

    void setCenter(float x, float y);

    void draw();

    float getPosX();

    float getPosY();

    float* getCenter();

    C2D_Sprite* getSpr();

    void setSpr(C2D_Sprite* newSpr);

    C2D_Sprite spr;    

private:
    float x, y;
    float center[2];
};

#endif