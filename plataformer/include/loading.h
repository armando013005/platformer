#pragma once
#include "sprites.h"
//#define TileMap = 1;

void UpdateLoad();

void InitResourses();

void FinalizeLoad();

void UnloadAll();


const Texture2D& GetTexture(int id);

const Music& GetMusic(int id);

const Sound& GetSound(int id);

const Font& GetFont(int id);



