#include "Sprite.hpp"

Sprite::Sprite() {
    // C2D_SpriteSetCenter(&this->spr, 0.5f, 0.5f);
    // C2D_SpriteSetPos(&this->spr, 400/2, 240/2);
    this->x = 200; this->y = 120;
}

Sprite::Sprite(C2D_Sprite* spr, float x, float y) {
    this->x = x; this->y = y; this->spr = *spr;

    C2D_SpriteSetCenter(&this->spr, 0.5f, 0.5f);
    C2D_SpriteSetPos(&this->spr, x, y);
}

void Sprite::setPosition(float x, float y) {
    this->x = x;
    this->y = y;

    C2D_SpriteSetPos(&this->spr, x, y);
}

void Sprite::move(float dx, float dy, bool overrideOOB) {
    if (!overrideOOB) {
        // make sure translate doesnt move to OOB
        const float newX = this->x + dx;
        const float newY = this->y + dy;
        if (!((unsigned)(newX-1) <= 398 && (unsigned)(newY-1) <= 238)) {
            return;
	    }
    }

	this->x += dx;
	this->y += dy;

	C2D_SpriteSetPos(&this->spr, this->x, this->y);
}

void Sprite::setCenter(float x, float y) {
    C2D_SpriteSetCenter(&this->spr, x, y);
}

void Sprite::draw() {
    C2D_DrawSprite(&this->spr);
}

float Sprite::getPosX() {
    return this->x;
}

float Sprite::getPosY() {
    return this->y;
}

C2D_Sprite* Sprite::getSpr() {
    return &this->spr;
}

void Sprite::setSpr(C2D_Sprite* newSpr) {
    this->spr = *newSpr;
}