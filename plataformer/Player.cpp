#include "include/Player.h"
#include "include/game.h"
#include "include/map.h"
#include "include/tile_map.h"
#include "include/timer.h"
#include <vector>

#define JUMPVEL Vector2{0,-0.03}
#define MAXVELR 0.03
#define MAXVELL -0.03
float airTime;
float jumptime;

void UpdatePlayer(Entidad* Player, frWorld* world) {
    frSetBodyRotation(Player->Body, 0);

    Vector2 vel = frGetBodyVelocity(Player->Body);

    
    if (IsKeyDown(KEY_RIGHT)) {
        if (IsKeyPressed(KEY_C)) {
            frApplyImpulse(Player->Body, { 0.155f,0.0f });
        }
        else if(vel.x > MAXVELR) {
            frSetBodyVelocity(Player->Body, { MAXVELR,vel.y });
        }
        else {
            frApplyImpulse(Player->Body, { 0.02f,0.0f });

        }
    }
    if (IsKeyDown(KEY_LEFT)) {
        if (IsKeyPressed(KEY_C)) {
            frApplyImpulse(Player->Body, { -0.155f,0.0f });
            
        }
        else if (vel.x < MAXVELL) {
            frSetBodyVelocity(Player->Body, {MAXVELL,vel.y});
        }
        else {
            frApplyImpulse(Player->Body, { -0.02f,0.0f });
        }
    }

    if (IsKeyDown(KEY_X)  && jumptime < 0.06f) {
        jumptime += GetFrameTime();
        if (Player->canJump) {
            Player->canJump = false;
            frApplyImpulse(Player->Body, JUMPVEL);
        }
        else {
            if (airTime < 0.075f) {
                Player->canJump = false;
                frApplyImpulse(Player->Body, JUMPVEL);
            }
        }
        
    }

    

    Vector2 Player_Pos = frGetBodyPosition(Player->Body);
    frShape* Player_Shape = frGetBodyShape(Player->Body);
    Rectangle Bounds = frGetBodyAABB(Player->Body);

    frRay Rays[3]{
          {{Player_Pos.x,Player_Pos.y + frNumberPixelsToMeters(2)},//center
          {0,Player_Pos.y + frNumberPixelsToMeters(1)},
          0.5f},
          {{Player_Pos.x + (frGetBodyAABB(Player->Body).width / 2)-0.05 ,Player_Pos.y + frNumberPixelsToMeters(2)},
          {0,Player_Pos.y + frNumberPixelsToMeters(1)}, //right
          0.5f},
          {{Player_Pos.x - (frGetBodyAABB(Player->Body).width / 2)+0.05 ,Player_Pos.y + frNumberPixelsToMeters(2)},
          {0,Player_Pos.y + frNumberPixelsToMeters(1)},//left
          0.5f}
    };
    bool RayOnGroundC = false;
    bool RayOnGroundR = false;
    bool RayOnGroundL = false;

    frRaycastHit hitsC[300];
    frRaycastHit hitsR[300];
    frRaycastHit hitsL[300];

    int countC = frComputeWorldRaycast(world, Rays[0], hitsC);
    int countR = frComputeWorldRaycast(world, Rays[1], hitsR);
    int countL = frComputeWorldRaycast(world, Rays[2], hitsL);

    for (int i = 0; i < countC; i++) {
        if (frGetBodyFlags(hitsC[i].body) == FR_FLAG_WALL) {
            DrawText("FLOOR", 400 - MeasureText("FLOOR", 20), 20, 20, BLACK);
            RayOnGroundC = true;
        }
        else
        {
            RayOnGroundC = false;
        }
    }
    for (int i = 0; i < countR; i++) {
        if (frGetBodyFlags(hitsR[i].body) == FR_FLAG_WALL) {
            DrawText("FLOOR", 400 - MeasureText("FLOOR", 20), 20, 20, BLACK);
            RayOnGroundR = true;
        }
        else
        {
            RayOnGroundR = false;
        }
    }
    for (int i = 0; i < countL; i++) {
        if (frGetBodyFlags(hitsL[i].body) == FR_FLAG_WALL) {
            DrawText("FLOOR", 400 - MeasureText("FLOOR", 20), 20, 20, BLACK);
            RayOnGroundL = true;
        }
        else
        {
            RayOnGroundL = false;
        }
    }
  
    if (RayOnGroundC || RayOnGroundL || RayOnGroundR) {
        Player->canJump = true; 
        airTime = 0;
        jumptime = 0;
    }
    else {
        Player->canJump = false;
        airTime += GetFrameTime();
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