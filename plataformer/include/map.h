#pragma once

#include "raylib.h"
#include "include/tile_map.h"

#include <vector>

// map basics
void LoadMap(const char* file);
void ClearMap();
void DrawMap();

Camera2D& GetMapCamera();

// tile map objects
std::vector<const TileObject*> GetMapObjectsOfType(const char* objType, TileObject::SubTypes requiredType = TileObject::SubTypes::None);

const TileObject* GetFirstMapObjectOfType(const char* objType, TileObject::SubTypes requiredType = TileObject::SubTypes::None);


// map sprites
struct SpriteInstance
{
	int Id = -1;
	bool Active = true;
	int SpriteFrame = -1;
	Vector2 Position = { 0,0 };
	Color Tint = WHITE;
	bool Bobble = false;
	bool Shadow = true;
};

//SpriteInstance* AddSprite(int frame, const Vector2& position);
void UpdateSprite(int spriteId, const Vector2& position);
void RemoveSprite(SpriteInstance* sprite);
void RemoveSprite(int id);
void ClearSprites();

// Effects
enum class EffectType
{
	Fade,
	RiseFade,
	RotateFade,
	ScaleFade,
};

// common object types
constexpr char PlayerSpawnType[] = "player_spawn";
constexpr char MobSpawnType[] = "mob_spawn";
constexpr char ChestType[] = "chest";
constexpr char ExitType[] = "exit";