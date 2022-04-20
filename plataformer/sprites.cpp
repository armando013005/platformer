#include <raylib.h>

#include <include/sprites.h>
#include <include/loading.h>
#include <vector>
#include <raymath.h>

struct SpriteInfo
{
	int textureId = -1;
	Rectangle Bounds = { 0,0,0,0 };
	Vector2 Origin = { 0,0 };
};

std::vector<SpriteInfo> Sprites;

void LoadSprites(int TextureId, float columnas, float filas, int espaciado) {
	
	if (columnas == 0 || filas == 0) {
		return;
	}

	const Texture2D& Texture = GetTexture(TextureId);

	int itemWidth = (Texture.width + espaciado) / columnas;
	int itemHeight = (Texture.height + espaciado) / filas;

	SpriteInfo Info;

	Info.textureId = TextureId;
	Info.Bounds.width = float(itemWidth - espaciado);
	Info.Bounds.height = float(itemHeight - espaciado);

	for (int y = 0; y < filas; ++y)
	{
		Info.Bounds.x = 0;
		for (int x = 0; x < columnas; ++x)
		{
			Sprites.push_back(Info);

			Info.Bounds.x += itemWidth;
		}

		Info.Bounds.y += itemHeight;
	}

}

void SetCustomSpriteOrigin(int spriteId,Vector2 origin)
{
	if (spriteId < 0 || spriteId >= int(Sprites.size()))
		return;

	SpriteInfo& sprite = Sprites[spriteId];
	sprite.Origin.x = origin.x;
	sprite.Origin.y = origin.y;
}

void DrawSprite(int spriteId, float x, float y, float rotation, float scale, Color tint, uint8_t flip)
{
	if (spriteId < 0 || spriteId >= int(Sprites.size()))
		return;

	SpriteInfo& sprite = Sprites[spriteId];

	Rectangle source = sprite.Bounds;

	if (flip & SpriteFlipDiagonal)
		rotation -= 90;
	if (flip & SpriteFlipX)
		source.width *= -1;
	if (flip & SpriteFlipY)
		source.height *= -1;

	Rectangle destination = { x, y, sprite.Bounds.width * scale,sprite.Bounds.height * scale };

	if (flip & SpriteFlipDiagonal)
		destination.y += destination.height;

	DrawTexturePro(GetTexture(sprite.textureId), source, destination, Vector2Scale(sprite.Origin, scale), rotation, tint);
}