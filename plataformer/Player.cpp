#include "include/Player.h"
#include "include/game.h"
#include "include/map.h"
#include "include/tile_map.h"
#include <vector>

void UpdatePlayer(Entidad* Player) {
    frSetBodyRotation(Player->Body, 0);
    Player->canJump = true;

    if (IsKeyDown(KEY_RIGHT)) frApplyImpulse(Player->Body, {0.02f,0.0f});
    if (IsKeyDown(KEY_LEFT)) frApplyImpulse(Player->Body, {-0.02f,0.f});
    if (IsKeyPressed(KEY_X) && Player->canJump) {
        Player->canJump = false;
        frApplyImpulse(Player->Body, { 0.0f,-0.2 });
        
    }

    
   
}

void UpdateCameraPlayerBoundsPush(Camera2D* camera, Entidad* player, int width, int height)
{

    static Vector2 bbox = { 0.5f, .6f };

    Vector2 bboxWorldMin = GetScreenToWorld2D({ (1 - bbox.x) * 0.5f * width, (1 - bbox.y) * 0.5f * height }, *camera);
    Vector2 bboxWorldMax = GetScreenToWorld2D({ (1 + bbox.x) * 0.5f * width, (1 + bbox.y) * 0.5f * height }, *camera);
    camera->offset = { (1 - bbox.x) * 0.5f * width, (1 - bbox.y) * 0.5f * height };
    Rectangle hitbox = frRecMetersToPixels(frGetBodyAABB(player->Body));
    Vector2 Position = frVec2MetersToPixels(frGetBodyPosition(player->Body));
    if (hitbox.x < bboxWorldMin.x) camera->target.x = Position.x;
    if (hitbox.y < bboxWorldMin.y) camera->target.y = 16;
    if (hitbox.x > bboxWorldMax.x) camera->target.x = bboxWorldMin.x + (Position.x - bboxWorldMax.x);
    if (hitbox.y > bboxWorldMax.y) camera->target.y = bboxWorldMin.y + (Position.y - bboxWorldMax.y);
}