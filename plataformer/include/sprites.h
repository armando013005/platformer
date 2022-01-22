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


void LoadSprites(int TextureId, int filas, int columnas, int espaciado);


void SetCustomSpriteOrigin(int spriteId, Vector2 origin);
;//void CenterSprite(int spriteId);

void DrawSprite(int spriteId, float x, float y, float rotation, float scale, Color tint, uint8_t flip);
