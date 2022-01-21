#pragma once
#include <raylib.h>
#include "Player.h"
#include "map.h"
#include <stdint.h>
#include <string>
#include <vector>

#define SpriteFlipNone 0
#define SpriteFlipX 0x02
#define SpriteFlipY 0x04
#define SpriteFlipDiagonal 0x08

 struct SpriteSheet
{
    Texture2D SheetTexture = { 0 };
    std::vector<Rectangle> Frames;
};

 struct SpriteAnimation
{
    std::string Name;
    int StartFrame = -1;
    int EndFrame = -1;
    float FPS = 3;
    bool Loops = true;
};

 struct AnimSpriteInstance
{
    Vector2 Position = { 0 };

    Vector2 Offset = { 0,0 };
    const SpriteSheet* Sheet = nullptr;
    const SpriteAnimation* Animation = nullptr;
    bool AnimationDone = false;
    int CurrentFrame = -1;
    float FrameLifetime = 0;
};


SpriteSheet LoadSpriteSheet(int textureid, int startRow, int endRows, int cols, int rows);

void LoadSprites(int TextureId, int filas, int columnas, int espaciado);

void AnimDrawSprite( AnimSpriteInstance& sprite);

void SetSpriteAnimation( AnimSpriteInstance& sprite, const SpriteAnimation& animation);

void UpdateSpriteAnimation(AnimSpriteInstance& sprite);

void SetCustomSpriteOrigin(int spriteId, Vector2 origin);

//int AddFippedFrames(SpriteSheet& sheet, int start, int end, bool flipX, int flipY);
//void CenterSprite(int spriteId);

void DrawSprite(int spriteId, float x, float y, float rotation, float scale, Color tint, uint8_t flip);
