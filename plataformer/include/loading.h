#pragma once
#include "sprites.h"
//#define TileMap = 1;

void UpdateLoad();

void InitResourses();

void FinalizeLoad();

void UnloadAll();

const SpriteSheet& GetSpriteSheet();

const Texture2D& GetTexture(int id);

const Font& GetFont(int id);



