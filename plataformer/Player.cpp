#include "include/Player.h"
#include "include/game.h"
#include "include/map.h"
#include "include/tile_map.h"
#include "include/timer.h"
#include "include/loading.h"
#include <vector>
#include <queue>
#define JUMPVEL 0.02
#define DASHVEl 0.03
#define MAXVEL 0.019
#define VEL 0.017

std::queue<KeyboardKey> inputbuffer;

void CoumputeRaysColitions(Entidad* Player, frWorld* world) {

    frRaycastHit hitsC[300];
    frRaycastHit hitsR[300];
    frRaycastHit hitsL[300];

    frRaycastHit hitsTR[300];
    frRaycastHit hitsTL[300];


    int countC = frComputeWorldRaycast(world, Player->Rays[0], hitsC);
    int countR = frComputeWorldRaycast(world, Player->Rays[1], hitsR);
    int countL = frComputeWorldRaycast(world, Player->Rays[2], hitsL);

    int countTR = frComputeWorldRaycast(world, Player->Rays[3], hitsTR);
    int countTL = frComputeWorldRaycast(world, Player->Rays[4], hitsTL);



    for (int i = 0; i < countC; i++) {
        if (frGetBodyFlags(hitsC[i].body) == FR_FLAG_WALL && frVec2Distance(hitsC[0].point, hitsC[i].point) < frNumberPixelsToMeters(7)) {
            Player->RayOnGroundC = true;
        }
        else
        {
            Player->RayOnGroundC = false;
        }
        if (frGetBodyFlags(hitsC[i].body) == FR_FLAG_PICO) {
            float dis = frVec2Distance(hitsC[0].point, hitsC[i].point);
            if (dis <= frNumberPixelsToMeters(2)) {
                Player->IsDead = true;
            }
        }
        
    }
    for (int i = 0; i < countR; i++) {
        if (frGetBodyFlags(hitsR[i].body) == FR_FLAG_WALL && frVec2Distance(hitsR[0].point, hitsR[i].point) < frNumberPixelsToMeters(7)) {
            Player->RayOnGroundR = true;
        }
        else
        {
            Player->RayOnGroundR = false;
        }
        if (frGetBodyFlags(hitsR[i].body) == FR_FLAG_PICO) {
            float dis = frVec2Distance(hitsR[0].point, hitsR[i].point);
            if (dis <= frNumberPixelsToMeters(2)) {
                Player->IsDead = true;
            }
        }
        
    }
    for (int i = 0; i < countL; i++) {
        if (frGetBodyFlags(hitsL[i].body) == FR_FLAG_WALL && frVec2Distance(hitsL[0].point, hitsL[i].point) < frNumberPixelsToMeters(7)) {
            Player->RayOnGroundL = true;
        }
        else
        {
            Player->RayOnGroundL = false;
        }
        if (frGetBodyFlags(hitsL[i].body) == FR_FLAG_PICO) {
            float dis = frVec2Distance(hitsL[0].point, hitsL[i].point);
            if (dis <= frNumberPixelsToMeters(2)) {
                Player->IsDead = true;
            }
        }

    }


    for (int i = 0; i < countTR; i++) {
        if (frGetBodyFlags(hitsTR[i].body) == FR_FLAG_WALL && frVec2Distance(hitsTR[0].point, hitsTR[i].point) < frNumberPixelsToMeters(2)) {
            Player->RayTouchingL = true;
        }
        else {
            Player->RayTouchingL = false;
        }

        if (frGetBodyFlags(hitsTR[i].body) == FR_FLAG_PICO) {
            float dis = frVec2Distance(hitsTR[0].point, hitsTR[i].point);
            if (dis <= frNumberPixelsToMeters(2)) {
                Player->IsDead = true;
            }
        }
        
    }

    for (int i = 0; i < countTL; i++) {
        if (frGetBodyFlags(hitsTL[i].body) == FR_FLAG_WALL && frVec2Distance(hitsTL[0].point, hitsTL[i].point) < frNumberPixelsToMeters(2)) {
            Player->RayTouchingR = true;
        }
        else {
            Player->RayTouchingR = false;
        }

        if (frGetBodyFlags(hitsTL[i].body) == FR_FLAG_PICO) {
            float dis = frVec2Distance(hitsTL[0].point, hitsTL[i].point);
            if (dis <= frNumberPixelsToMeters(2)) {
                Player->IsDead = true;
            }
        }
    }


    

}

void computeSideRays(Entidad* Player, frWorld* world) {

    frRaycastHit hitsSR[300];
    frRaycastHit hitsSL[300];
    int countSR = frComputeWorldRaycast(world, Player->Rays[5], hitsSR);
    int countSL = frComputeWorldRaycast(world, Player->Rays[6], hitsSL);


    for (int i = 1; i < countSR; i++) {
        if (frGetBodyFlags(hitsSR[i].body) == FR_FLAG_WALL && frVec2Distance(hitsSR[0].point, hitsSR[i].point) <= frNumberPixelsToMeters(2)) {
            Player->RayTouchingSR = true;
        }
        else {
            Player->RayTouchingSR = false;
        }

        if (frGetBodyFlags(hitsSR[i].body) == FR_FLAG_PICO) {
            float dis = frVec2Distance(hitsSR[0].point, hitsSR[i].point);
            if (dis <= frNumberPixelsToMeters(2)) {
                Player->IsDead = true;
            }
        }
        
    }

    for (int i = 1; i < countSL; i++) {

        if (frGetBodyFlags(hitsSL[i].body) == FR_FLAG_WALL && frVec2Distance(hitsSL[0].point, hitsSL[i].point) <= frNumberPixelsToMeters(2)) {
            Player->RayTouchingSL = true;
        }
        else {
            Player->RayTouchingSL = false;
        }

        if (frGetBodyFlags(hitsSL[i].body) == FR_FLAG_PICO ) {
            float dis = frVec2Distance(hitsSL[0].point, hitsSL[i].point);
            if (dis <= frNumberPixelsToMeters(2)) {
                Player->IsDead = true;
            }
        }
        

    }
}
void UpdatePlayer(Entidad* Player, frWorld* world) {


    frVec2Normalize(Player->WallJumpAngle);

    frSetBodyRotation(Player->Body, 0);

    Player->levelPassed = false;
    Player->RayTouchingSR = false;
    Player->RayTouchingSL = false;
    Player->WantToMoveL = false;
    Player->WantToMoveR = false;
    Vector2 vel = frGetBodyVelocity(Player->Body);
    Player->CanMove -= GetFrameTime();

    if (!(Player->CanMove > 0)) {
        if (IsKeyDown(KEY_RIGHT)) {
            if (!(Player->WallRemember > 0)) {
                if (vel.x > MAXVEL) {
                    frSetBodyVelocity(Player->Body, { MAXVEL,vel.y });
                }
                else {
                    frApplyImpulse(Player->Body, { VEL,0.0f });

                }
            }
            else {
                frApplyImpulse(Player->Body, { VEL,0.0f });

            }
            Player->WantToMoveR = true;
        }

        if (IsKeyDown(KEY_LEFT)) {
            if (!(Player->WallRemember > 0)) {
                if (vel.x < -MAXVEL) {
                    frSetBodyVelocity(Player->Body, { -MAXVEL,vel.y });
                }
                else {
                    frApplyImpulse(Player->Body, { -VEL,0.0f });
                }

            }
            else {
                frApplyImpulse(Player->Body, { -VEL,0.0f });
            }
            Player->WantToMoveL = true;
        }
    }


    Player->Player_Pos = frGetBodyPosition(Player->Body);
    frShape* Player_Shape = frGetBodyShape(Player->Body);
    Rectangle Bounds = frGetBodyAABB(Player->Body);
    //bottom
    Player->Rays[0] = { {Player->Player_Pos.x,Player->Player_Pos.y + frNumberPixelsToMeters(2)},{0,Player->Player_Pos.y },2.f };
    Player->Rays[1] = { {Player->Player_Pos.x + (frGetBodyAABB(Player->Body).width / 2),Player->Player_Pos.y + frNumberPixelsToMeters(4.5)}, {0,Player->Player_Pos.y },2.f };
    Player->Rays[2] = { {Player->Player_Pos.x - (frGetBodyAABB(Player->Body).width / 2),Player->Player_Pos.y + frNumberPixelsToMeters(4.5)},{0,Player->Player_Pos.y },2.f };

    //top
    Player->Rays[3] = { {Player->Player_Pos.x + (frGetBodyAABB(Player->Body).width / 2) - frNumberPixelsToMeters(0.5) ,Player->Player_Pos.y - frNumberPixelsToMeters(2)}, {0,-Player->Player_Pos.y },2.f };
    Player->Rays[4] = { {Player->Player_Pos.x - (frGetBodyAABB(Player->Body).width / 2) + frNumberPixelsToMeters(0.5),Player->Player_Pos.y - frNumberPixelsToMeters(2)},{0,-Player->Player_Pos.y },2.f };

    //side
    Player->Rays[5] = { {Player->Player_Pos.x + (frGetBodyAABB(Player->Body).width / 2) - frNumberPixelsToMeters(0.5) ,Player->Player_Pos.y}, {Player->Player_Pos.x,0},.90f };
    Player->Rays[6] = { {Player->Player_Pos.x - (frGetBodyAABB(Player->Body).width / 2) + frNumberPixelsToMeters(0.5),Player->Player_Pos.y },{-Player->Player_Pos.x,0 },.90f };

    CoumputeRaysColitions(Player, world);

    float delta = GetFrameTime();
    Player->GroundedRemember -= delta;

    if (Player->RayOnGroundC || Player->RayOnGroundL || Player->RayOnGroundR) {
        if (!(Player->GroundedRemember > 0) && (vel.y > 0)) {
            PlaySound(GetSound(2));
        }
        Player->GroundedRemember = Player->GroundedRememberTime;

    }

    if (Player->RayTouchingL && !Player->RayTouchingR) {
        frSetBodyPosition(Player->Body, { Player->Player_Pos.x - 0.2f,Player->Player_Pos.y });
    }

    if (Player->RayTouchingR && !Player->RayTouchingL) {
        frSetBodyPosition(Player->Body, { Player->Player_Pos.x + 0.2f,Player->Player_Pos.y });
    }

    computeSideRays(Player, world);

    vel = frGetBodyVelocity(Player->Body);

    if (Player->RayTouchingSL || Player->RayTouchingSR) {
        Player->TouchingWall = true;
    }
    else {
        Player->TouchingWall = false;
    }

    Player->RememberJump -= delta;

    if (IsKeyPressed(KEY_C)) {
        
        Player->RememberJump = Player->RememberJumpTime;
    }
    if (IsKeyUp(KEY_C) && !(Player->WallRemember > 0)) {
        if (vel.y < 0) {
            frSetBodyVelocity(Player->Body, { vel.x, -JUMPVEL * 0.02 });
        }
    }

    Player->WallRemember -= delta;
    if (Player->TouchingWall && !(Player->GroundedRemember > 0) && vel.y > 0 && (Player->WantToMoveL || Player->WantToMoveR)) {
        Player->WallRemember = Player->WallRememberTime;
        Player->WallSliding = true;
    }
    else {

        Player->WallSliding = false;
    }

    if (Player->WallSliding)
    {
        frSetBodyVelocity(Player->Body, { vel.x,Player->WallSlideSpeed });
    }

    if (Player->WantToMoveL && Player->WantToMoveR) {
        if (Player->RayTouchingSL) {
            Player->WallJumpDirection = 1;
        }
        else if (Player->RayTouchingSR) {
            Player->WallJumpDirection = -1;
        }
    }
    else if (Player->WantToMoveL) {
        Player->WallJumpDirection = 1;
    }
    else if (Player->WantToMoveR) {
        Player->WallJumpDirection = -1;
    }
    else {
        if (Player->RayTouchingSL) {
            Player->WallJumpDirection = 1;
        }
        else if (Player->RayTouchingSR) {
            Player->WallJumpDirection = -1;
        }
    }



    if ((Player->RememberJump > 0) && (Player->GroundedRemember > 0)) {

        frSetBodyVelocity(Player->Body, { vel.x,-JUMPVEL });
        Player->RememberJump = 0;
        Player->GroundedRemember = 0;

    }


    if ((Player->WallRemember > 0) && (Player->RememberJump > 0) && (Player->GroundedRemember < 0)) {
        Player->RememberJump = 0;
        Player->WallRemember = 0;
        frApplyImpulse(Player->Body, { Player->WallJumpForce * Player->WallJumpDirection * Player->WallJumpAngle.x, Player->WallJumpForce * Player->WallJumpAngle.y });
    }



    if ((!Player->WantToMoveL && !Player->WantToMoveR) || (Player->WantToMoveL && Player->WantToMoveR)) {
        if (vel.y != 0) {
            if (vel.x > 0) {

                frSetBodyVelocity(Player->Body, { float(vel.x - vel.x * 0.3),frGetBodyVelocity(Player->Body).y });
            }
            else if (vel.x < 0) {

                frSetBodyVelocity(Player->Body, { float(vel.x - vel.x * 0.3),frGetBodyVelocity(Player->Body).y });
            }

        }
    }

    if (CheckCollisionRecs(Player->Exit,frRecMetersToPixels(frGetBodyAABB(Player->Body)))) {
        Player->levelPassed = true;
    }
    else {
        Player->levelPassed = false;
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