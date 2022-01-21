#include <raylib.h>

#include <include/sprites.h>
#include <include/loading.h>
#include <vector>
#include <raymath.h>
#include <math.h>

struct SpriteInfo
{
	int textureId = -1;
	Rectangle Bounds = { 0,0,0,0 };
	Vector2 Origin = { 0,0 };
};

std::vector<SpriteInfo> Sprites;

SpriteSheet LoadSpriteSheet(int textureid,int startRow,int endRows, int cols, int rows)
{
	SpriteSheet sheet = { 0 };
	sheet.SheetTexture = GetTexture(textureid);
	if (sheet.SheetTexture.id >= 0)
	{
		float w = (float)sheet.SheetTexture.width / cols;
		float h = (float)sheet.SheetTexture.height / rows;

		for (int y = startRow; y < 1; y++)
		{
			for (int x = 0; x < endRows; x++)
			{
				sheet.Frames.emplace_back(Rectangle{ x * w,y * h,w,h });
			}
		}
	}

	return std::move(sheet);
}

void LoadSprites(int TextureId, int filas, int columnas, int espaciado) {
	
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

void AnimDrawSprite(AnimSpriteInstance& sprite)
{
	if (sprite.Sheet == nullptr || sprite.Sheet->SheetTexture.id == 0 || sprite.Animation == nullptr)
		return;

	Rectangle frame = sprite.Sheet->Frames[sprite.CurrentFrame];
	DrawTexturePro(sprite.Sheet->SheetTexture, frame, Rectangle{ sprite.Position.x,sprite.Position.y,fabsf(frame.width),fabsf(frame.height) }, sprite.Offset, 0, WHITE);
}

void SetSpriteAnimation(AnimSpriteInstance& sprite, const SpriteAnimation& animation)
{
	sprite.Animation = &animation;
	sprite.CurrentFrame = animation.StartFrame;
	sprite.FrameLifetime = 0;
	sprite.AnimationDone = false;
}

void UpdateSpriteAnimation(AnimSpriteInstance& sprite)
{
	if (sprite.Animation == nullptr)
		return;

	sprite.FrameLifetime += GetFrameTime();
	if (sprite.FrameLifetime > 1.0f / sprite.Animation->FPS)
	{
		sprite.FrameLifetime = 0;
		sprite.CurrentFrame++;
		sprite.AnimationDone = false;
		if (sprite.CurrentFrame > sprite.Animation->EndFrame)
		{
			if (sprite.Animation->Loops)
				sprite.CurrentFrame = sprite.Animation->StartFrame;
			else
			{
				sprite.AnimationDone = true;
				sprite.CurrentFrame--;
			}
		}
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