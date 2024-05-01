#include "include/map.h"

#include "include/sprites.h"
#include "include/tile_map.h"

#include "raylib.h"

#include <math.h>
#include <unordered_map>
#include <list>


class EffectInstance
{
public:
	Vector2 Position = { 0,0 };
	EffectType Effect = EffectType::Fade;
	int SpriteId = -1;
	float Lifetime = 1;
	float MaxLifetime = 1;
};

std::list<EffectInstance> Effects;

Camera2D MapCamera = { 0 };
TileMap CurrentMap;

std::unordered_map<int, SpriteInstance> SpriteInstances;
int NextSpriteId = 0;

Rectangle MapBounds = { 0,0,0,0 };

Camera2D& GetMapCamera()
{
	return MapCamera;
}

void LoadMap(const char* file)
{
	ClearSprites();
	ReadTileMap(file, CurrentMap);

	MapCamera.offset.x = 800/2;
	MapCamera.offset.y= 600/2;

	MapCamera.rotation = 0;
	MapCamera.zoom = 1.8f;

	MapCamera.target.x = 0;
	MapCamera.target.y = 0;

	MapBounds = Rectangle{ 0,0,0,0 };

	if (!CurrentMap.TileLayers.empty())
	{
		int index = CurrentMap.TileLayers.rbegin()->first;

		MapBounds.width = (CurrentMap.TileLayers[index]->Size.x * CurrentMap.TileLayers[index]->TileSize.x);
		MapBounds.height = (CurrentMap.TileLayers[index]->Size.y * CurrentMap.TileLayers[index]->TileSize.y);

		MapCamera.target.x =  MapBounds.width / 2;
		MapCamera.target.y =  MapBounds.height / 2;
	}
}

void ClearMap()
{
	CurrentMap.ObjectLayers.clear();
	CurrentMap.TileLayers.clear();
	ClearSprites();
	Effects.clear();
}

void DrawMap()
{
	if (CurrentMap.TileLayers.empty())
		return;

	BeginMode2D(GetMapCamera());
	DrawTileMap(MapCamera, CurrentMap);

	for (const auto& entry : SpriteInstances)
	{
		const SpriteInstance& sprite = entry.second;
		if (sprite.Active)
		{
			float offset = 0;
			if (sprite.Bobble)
				offset = fabsf(sinf(float(GetTime() * 5)) * 3);

			if (sprite.Shadow)
				DrawSprite(sprite.SpriteFrame, sprite.Position.x + 2, sprite.Position.y + 2 + offset, 0.0f, 1.0f, ColorAlpha(BLACK, 0.5f), SpriteFlipNone);

			DrawSprite(sprite.SpriteFrame, sprite.Position.x, sprite.Position.y + offset, 0.0f, 1.0f, sprite.Tint, SpriteFlipNone);
		}
	}

	for (std::list<EffectInstance>::iterator effect = Effects.begin(); effect != Effects.end();)
	{
		effect->Lifetime -= GetFrameTime();

		if (effect->Lifetime < 0)
		{
			effect = Effects.erase(effect);
			continue;
		}

		float param = effect->Lifetime / effect->MaxLifetime;
		float rotation = 0;
		float alpha = 1;
		float scale = 1;

		Vector2 pos = effect->Position;

		switch (effect->Effect)
		{
		case EffectType::Fade:
			alpha = param;
			break;

		case EffectType::RiseFade:
			alpha = param;
			pos.y -= (1.0f - param) * 30;
			break;

		case EffectType::RotateFade:
			rotation = (1.0f - param) * 360;
			alpha = param;
			break;

		case EffectType::ScaleFade:
			alpha = param;
			scale = 1 + (1.0f - param);
			break;
		}

		DrawSprite(effect->SpriteId, pos.x, pos.y, rotation, scale, ColorAlpha(WHITE, alpha), SpriteFlipNone);

		effect++;
	}

	EndMode2D();
}

std::vector<const TileObject*> GetMapObjectsOfType(const char* objType, TileObject::SubTypes requiredType)
{
	std::vector<const TileObject*> objects;
	if (CurrentMap.ObjectLayers.empty())
		return objects;

	for (const auto& layerInfo : CurrentMap.ObjectLayers)
	{
		for (const auto& object : layerInfo.second->Objects)
		{
			if (object->Type == objType && (requiredType == TileObject::SubTypes::None || object->SubType == requiredType))
				objects.push_back(object.get());
		}
	}

	return objects;
}

const TileObject* GetFirstMapObjectOfType(const char* objType, TileObject::SubTypes requiredType)
{
	std::vector<const TileObject*> objects;
	if (CurrentMap.ObjectLayers.empty())
		return nullptr;

	for (const auto& layerInfo : CurrentMap.ObjectLayers)
	{
		for (const auto& object : layerInfo.second->Objects)
		{
			if (object->Type == objType && (requiredType == TileObject::SubTypes::None || object->SubType == requiredType))
				return object.get();
		}
	}

	return nullptr;
}

/*SpriteInstance* AddSprite(int frame, const Vector2& position)
{
	NextSpriteId++;
	return &(SpriteInstances.insert_or_assign(NextSpriteId, SpriteInstance{ NextSpriteId, true, frame, position }).first->second);
}*/

void UpdateSprite(int spriteId, const Vector2& position)
{
	auto itr = SpriteInstances.find(spriteId);
	if (itr == SpriteInstances.end())
		return;

	itr->second.Position = position;
}

void RemoveSprite(SpriteInstance* sprite)
{
	if (sprite != nullptr)
		RemoveSprite(sprite->Id);
}

void RemoveSprite(int spriteId)
{
	auto itr = SpriteInstances.find(spriteId);
	if (itr != SpriteInstances.end())
		SpriteInstances.erase(itr);
}

void ClearSprites()
{
	SpriteInstances.clear();
	NextSpriteId = 0;
}
