#include "Level.hpp"

template <typename T>
Level::Level(T* levelName, char* spritesheetPath, C3D_RenderTarget* top, C3D_RenderTarget* bottom) : level(levelName), top(top), bottom(bottom) {
    this->spritesheet = C2D_SpriteSheetLoad(spritesheetPath);
    size_t numImages = C2D_SpriteSheetCount(spritesheet);

    for (size_t i = 0; i < numImages; i++) {
        Sprite* thisSprite = &sprites[i];
        float x = SCREEN_WIDTH / 2;
        float y = SCREEN_HEIGHT / 2;

        C2D_SpriteFromSheet(&thisSprite->spr, spritesheet, i);
        C2D_SpriteSetCenter(&thisSprite->spr, 0.5f, 0.5f);
        thisSprite->setPosition(x, y);
    }
}

void Level::run() {
    if (!hasBeenSetup) {
        level->setup();
        this->hasBeenSetup = true;
    }
    level->update();

    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(top, C3D_CLEAR_COLOR);
        C2D_SceneBegin(top);
        level->drawTop();

        C2D_TargetClear(bottom, C3D_CLEAR_COLOR);
        C2D_SceneBegin(bottom);
        level->drawBottom();
    C3D_FrameEnd(0);
}   

void Level::cleanup() {
    C2D_SpriteSheetFree(spriteSheet);
}