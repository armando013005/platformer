#include "include/Player.h"
#include "include/game.h"
#include "include/map.h"
#include "include/tile_map.h"
#include "include/timer.h"
#include <vector>

#define JUMPVEL 0.02
#define DASHVEl 0.03
#define MAXVEL 0.019
#define VEL 0.017

void UpdatePlayer(Entidad* Player, frWorld* world) {

    frSetBodyRotation(Player->Body, 0);
    Player->WantToMoveL = false;
    Vector2 vel = frGetBodyVelocity(Player->Body);

    /*if (IsKeyPressed(KEY_C) && !Player->WeDashed) {
        if (vel.x < 0) {
            frApplyImpulse(Player->Body, { -DASHVEl,0.0f });
            Player->WeDashed = true;
        }
        else if (vel.x > 0) {
            frApplyImpulse(Player->Body, { DASHVEl,0.0f });
            Player->WeDashed = true;
        }

        Player->WantToMoveL = true;
        
    }*/

    if (IsKeyDown(KEY_RIGHT)) {
        if (vel.x > MAXVEL && !Player->WeDashed) {
            frSetBodyVelocity(Player->Body, { MAXVEL,vel.y });
        }
        else {
            frApplyImpulse(Player->Body, { VEL,0.0f });

        }
        Player->WantToMoveL = true;
    }
 
    
    if (IsKeyDown(KEY_LEFT)) {
        if (vel.x < -MAXVEL && !Player->WeDashed) {
            frSetBodyVelocity(Player->Body, { -MAXVEL,vel.y });
        }
        else {
            frApplyImpulse(Player->Body, { -VEL,0.0f });
        }
        Player->WantToMoveL = true;
    }

   

    Vector2 Player_Pos = frGetBodyPosition(Player->Body);
    frShape* Player_Shape = frGetBodyShape(Player->Body);
    Rectangle Bounds = frGetBodyAABB(Player->Body);

    frRay Rays[3]{
          {{Player_Pos.x,Player_Pos.y + frNumberPixelsToMeters(2)},//center
          {0,Player_Pos.y + frNumberPixelsToMeters(1)},
          0.5f},
          {{Player_Pos.x + (frGetBodyAABB(Player->Body).width / 2) ,Player_Pos.y + frNumberPixelsToMeters(2)},
          {0,Player_Pos.y + frNumberPixelsToMeters(1)}, //right
          0.5f},
          {{Player_Pos.x - (frGetBodyAABB(Player->Body).width / 2),Player_Pos.y + frNumberPixelsToMeters(2)},
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
  
    Player->GroundedRemember -= GetFrameTime();
    
    if (RayOnGroundC || RayOnGroundL || RayOnGroundR) {
        Player->GroundedRemember = Player->GroundedRememberTime;

    }

    Player->RememberJump -= GetFrameTime();
    if (IsKeyPressed(KEY_C)) {
        Player->RememberJump = Player->RememberJumpTime;
    }
    
    if (IsKeyUp(KEY_C)) {
        if (vel.y < 0) {
            frSetBodyVelocity(Player->Body, { vel.x, -JUMPVEL * 0.02 });
        }
    }
    if ((Player->RememberJump > 0) && (Player->GroundedRemember > 0) ) {
        Player->RememberJump = 0;
        Player->GroundedRemember = 0;
        frSetBodyVelocity(Player->Body, {vel.x,-JUMPVEL });
    }
   // if (!Player->WantToMoveL) {
     //  frSetBodyVelocity(Player->Body, {0,frGetBodyVelocity(Player->Body).y});
    //}
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